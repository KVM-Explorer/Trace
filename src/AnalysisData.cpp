#include "AnalysisData.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace LidarDriver;




AnalysisData::AnalysisData()
{
    ResetScanGarb();
    MaxFrame=16;

}

bool AnalysisData::IsFirstFrame(ScanningFrameData &scan_frame)
{
    return scan_frame.angle<0.0001? true: false;
}


ScannerState AnalysisData::CombineFrameInformation(ScanningFrameData &scan_frame)
{
    switch (State) {
        case ScannerState::LidarGarbFirst : {
            if (IsFirstFrame(scan_frame)) {
                ResetScanGarb();
                GarbFrameAmount = 1;
                LastAngle = scan_frame.angle;
                LidarScannerData lidar_data;
                ScanningFrameData2LidarScannerData(scan_frame, lidar_data);
                AllLidarScanData.Insert(lidar_data);
                State = ScannerState::LidarGarbElse;
            }

            return ScannerState::LidarGarbing;
        }
        case ScannerState::LidarGarbElse :
        {

            LidarScannerData lidar_data;
            ScanningFrameData2LidarScannerData(scan_frame, lidar_data);
            AllLidarScanData.Insert(lidar_data);

            LastAngle = scan_frame.angle;
            GarbFrameAmount++;
//            std::cout<<scan_frame.angle<<std::endl;
            if(scan_frame.angle==337.5) {
                State = ScannerState::LidarGarbFirst;
                return ScannerState::LidarGarbSucess;
            }
            return ScannerState::LidarGarbing;
        }
        default:
            std::logic_error("Analysis::CombineFrameInformation meet error\n");
            break;
    }
    return ScannerState::LidarGarbError;
}

ScannerState AnalysisData::AnalysisLidarSpeed(Packet &packet)
{
    double lidar_error_speed = Unpacket::GetLidarSpeed(packet)*0.05f;
    std::logic_error("AnalysisLidarSpeed Lidar error speed is "+std::to_string(lidar_error_speed)+"\n");
    return ScannerState::LidarGarbError;
}
ScannerState AnalysisData::AnalysisHealthInformation(Packet &packet)
{
    LidarError lidar_error = Unpacket::GetHealthInformation(packet);
    std::logic_error("AnalysisHealthInformation lidar error is "+std::to_string(int(lidar_error))+"\n");
    return ScannerState::LidarGarbError;
}
void AnalysisData::ResetScanGarb()
{
    AllLidarScanData.Clear();
    StartPoint = false;
    LastAngle=0.0;
    State=ScannerState::LidarGarbFirst;
    GarbFrameAmount=0;
}

ScannerState AnalysisData::AnalysisPacket(Packet &packet) {
    LidarCommandID command_id = LidarCommandID(packet.GetCommandID());

    switch (command_id) {
        case LidarCommandID::LidarDistance:     return AnalysisLidarFrame(packet);
        case LidarCommandID::LidarHealth:       return AnalysisHealthInformation(packet);
        case LidarCommandID::LidarSpeed:        return AnalysisLidarSpeed(packet);
        case LidarCommandID::LidarNewDistance:  return AnalysisLiadrFrameWithDebug(packet);
        default:
            throw std::logic_error("AnalysisData::AnalysisPacket Error Command ID\n");
            return ScannerState::LidarGarbElse;

    }
}

ScannerState AnalysisData::AnalysisLidarFrame(Packet &packet) {

    ScanningFrameData frame_scan;
    frame_scan = Unpacket::GetLidarScan(packet);
    return CombineFrameInformation(frame_scan);
}

ScannerState AnalysisData::AnalysisLiadrFrameWithDebug(Packet &packet)
{
    ScanningFrameData frame_scan;
    frame_scan = Unpacket::GetLidarScanWithDebug(packet);
    return CombineFrameInformation(frame_scan);
}

ScannerState AnalysisData::GetScanData() {

    Packet packet;
    while(1)
    {

        if(StartPoint)
        {
            ResetScanGarb();
            CombineFrameInformation(firstFrameData);
        }
        else
        {
            if(packetReceiver.ReceivePacket(packet))
            {

                return AnalysisPacket(packet);
            }
        }

    }

}

bool AnalysisData::InitAnalysizer(SerialConnection *serial_connection)
{
    if(serial_connection == nullptr || serial_connection->GetStatus() ==SerialConnection::Status::StatusOpenedError)
    {
        std::logic_error("AnalysisData::InitAnalysiser init fail\n");
        return false;
    }
    else
    {
        packetReceiver.SetDeviceConnection(serial_connection);
        return true;
    }
}

void AnalysisData::ScanningFrameData2LidarScannerData(ScanningFrameData &frame_data, LidarScannerData &lidar_data) {
    size_t size = frame_data.GetSize();
    float first_angle = frame_data.GetAngle();

    float angle_step = 360.0f /float(MaxFrame) /float(size);
    lidar_data.angle.resize(size);
    lidar_data.distance.resize(size);

    for(size_t i=0;i<size ; i++)
    {
        lidar_data.angle[i]=first_angle+i*angle_step;
        lidar_data.distance[i]=frame_data.distance[i];
    }
}

LidarScannerData AnalysisData::GetLidarScanData() {

    for(int i=0;i<AllLidarScanData.GetSize();i++)
    {
//        std::cout<<"Origin: "<<AllLidarScanData.angle[i]<<"   ";
        AllLidarScanData.angle[i]-=180.0f;
        if(AllLidarScanData.angle[i]<0)AllLidarScanData.angle[i]+=360.0f;
//        std::cout<<"Now: "<<AllLidarScanData.angle[i]<<std::endl;
    }

    for(int i=0;i< AllLidarScanData.GetSize();i++)
    {
        for(int j=i;j<AllLidarScanData.GetSize();j++)
        {
            if(AllLidarScanData.angle[j]<AllLidarScanData.angle[i])
            {
                float c=AllLidarScanData.angle[i];
                AllLidarScanData.angle[i]=AllLidarScanData.angle[j];
                AllLidarScanData.angle[j]=c;

                c=AllLidarScanData.distance[i];
                AllLidarScanData.distance[i]=AllLidarScanData.distance[j];
                AllLidarScanData.distance[j]=c;
            }
        }
    }

    return AllLidarScanData;
}




