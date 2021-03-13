#pragma once
#include<vector>
#include "Packet.hpp"
namespace LidarDriver
{
    struct ScanningFrameData
    {
        std::vector<float> distance;
        std::vector<int> signal;//调试信息解析后不可用
        float lidar_speed;
        float angle;
        size_t GetSize() {return distance.size();}
        float GetAngle() {return angle;}
    };
    enum class LidarError
    {
        LidarErrorNone,
        LidarErrorCCD,
        LidarErrorCalibrationParameters,
        LidarErrorOpticalPowerParameters,
        LidarErrorUnknow
    };
    class Unpacket {
    public:
        Unpacket(){};
        ~Unpacket(){};

        static ScanningFrameData GetLidarScan(Packet &packet);

        static LidarError GetHealthInformation(Packet &packet);

        static int GetLidarSpeed(Packet &packet);

        static ScanningFrameData GetLidarScanWithDebug(Packet &packet);
    };
}



