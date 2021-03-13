#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "AnalysisData.hpp"
#include "chrono"

namespace LidarDriver{
    class ShowLidarData {
    public:
        ShowLidarData(){};
        ~ShowLidarData(){};

        static void ShowLidarChart(LidarScannerData lidar_data);

        static void ReviseData(LidarScannerData &lidar_data);

        static void ShowCurrentArrow(float angle);



        static auto GetTime_MS();


    };
}



