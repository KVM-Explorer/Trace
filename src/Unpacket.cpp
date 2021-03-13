#include "Unpacket.hpp"
#include <iostream>
using namespace LidarDriver;

ScanningFrameData Unpacket::GetLidarScan(Packet &packet)
{
    ScanningFrameData frame_scan;
    unsigned short length = packet.GetParameterLength();
    unsigned char *buffer = packet.GetParameterPointer();
    unsigned short distance_number=0;

    unsigned short angle=0;
    unsigned short distance=0;

    angle =Packet::CompositeNumber(buffer);
    frame_scan.angle = float (angle)/100.0;

    distance_number = (length -2)/2;
    frame_scan.distance.resize(distance_number);

    for(size_t i=0;i<distance_number;i++)
    {
        distance=Packet::CompositeNumber(buffer+2*i+2);
        frame_scan.distance[i]= float (distance)/4.0f/1000.0f;
    }
    return  frame_scan;
}

LidarError Unpacket::GetHealthInformation(Packet &packet)
{
    unsigned char health_infomation = packet.GetParameter();
    return LidarError(health_infomation);
}

int Unpacket::GetLidarSpeed(Packet &packet)
{
    int lidar_speed =packet.GetParameter();
    return lidar_speed;
}

ScanningFrameData Unpacket::GetLidarScanWithDebug(Packet &packet) {

    ScanningFrameData frame_data;
    unsigned char *buffer = packet.GetParameterPointer();
    unsigned short length = packet.GetParameterLength();
    unsigned speed =0;
    unsigned short angle=0;
    unsigned short distance =0;
    int distance_number =0;
    speed = buffer[0];
    angle = Packet::CompositeNumber(&buffer[3]);

    frame_data.lidar_speed=speed*0.05f;
    frame_data.angle=angle*0.01f;

    distance_number=(length-5)/3;
    frame_data.distance.resize(distance_number);
    for(int i=0;i<distance_number;i++)
    {
        //5+3N  signal d1 d2
        distance = Packet::CompositeNumber(&buffer[i*3+5+1]);
        frame_data.distance[i]=float (distance)/4.0f/1000.0f;
//        std::cout<<frame_data.distance[i]<<std::endl;
    }
    return frame_data;


    return ScanningFrameData();
}


