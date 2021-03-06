#pragma once
#include "AnalysisData.hpp"
namespace Algorithm
{
    class Trace {
    public:
        Trace(){
            //todo MinWidth;
            MinWidth=0.2;
            LastLeft=1 ;
            LastRight=1 ;
            LastIsStraight= false;
            MinInterval = 0.05;
            MinDistance = 2;
            CarWidth = 0.5;
        }
        ~Trace(){};
        float GetDrivingDirection(LidarDriver::LidarScannerData &data);
        int IsApprochingWall(float current_distance, float last_distance);
        bool IsPassGate(float angle1,float angle2);
    private:
        LidarDriver::LidarScannerData lidar_data;
        float MinWidth;
        float CarWidth;
        float LastLeft,LastRight;
        float MinInterval ;//min acceptable distance about current postion and last postion
        float MinDistance;
        bool LastIsStraight;


    };
}



