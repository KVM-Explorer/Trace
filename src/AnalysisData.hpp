#pragma once
#include <vector>
#include "Unpacket.hpp"
#include "PacketReveiver.hpp"
namespace LidarDriver
{
    struct LidarScannerData
    {
        LidarScannerData():angle(),distance(),signal(){};

        void Insert(LidarScannerData &scan_data)
        {
            this->angle.insert(this->angle.end(),scan_data.angle.begin(),scan_data.angle.end());
            this->distance.insert(this->distance.end(),scan_data.distance.begin(),scan_data.distance.end());
            this->signal.insert(this->signal.end(), scan_data.signal.begin(),scan_data.signal.end());
        }

        void Clear()
        {
            angle.clear();
            distance.clear();
            signal.clear();
        }

        size_t GetSize() {return angle.size();}
        std::vector<float> angle;
        std::vector<float> distance;
        std::vector<float> signal;
        std::vector<int> index;
    };

    enum class LidarCommandID
    {
        LidarDistance   = 0xA9,
        LidarHealth     = 0xAB,
        LidarSpeed      = 0xAE,
        LidarNewDistance= 0xAD
    };

    enum class ScannerState
    {
        LidarGarbFirst,
        LidarGarbing,
        LidarGarbSucess,
        LidarGarbError,
        LidarGarbElse
    };
    class AnalysisData {

    public:
        AnalysisData();
        ~AnalysisData(){};
        bool IsFirstFrame(ScanningFrameData &scan_frame);
        ScannerState  CombineFrameInformation(ScanningFrameData &scan_frame);
        ScannerState AnalysisPacket(Packet &packet);
        ScannerState GetScanData();
        bool InitAnalysizer(SerialConnection *serial_connection);
        LidarScannerData GetLidarScanData();

    private:

        ScannerState AnalysisLidarFrame(Packet &packet);
        ScannerState AnalysisLidarSpeed(Packet &packet);
        ScannerState AnalysisHealthInformation(Packet &packet);
        ScannerState AnalysisLiadrFrameWithDebug(Packet &packet);
        void ResetScanGarb();
        void ScanningFrameData2LidarScannerData(ScanningFrameData &frame_data,LidarScannerData &lidar_data);

    private:

        ScannerState        State=ScannerState::LidarGarbFirst;
        LidarScannerData    AllLidarScanData;
        float               LastAngle;
        PacketReceiver      packetReceiver;
//        SerialConnection    *serialConnection;
        ScanningFrameData   firstFrameData;
        bool                StartPoint;
        float               MaxFrame=16;
        int                 GarbFrameAmount=0;
    };
}



