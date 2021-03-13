#pragma once
#include "AnalysisData.hpp"

namespace LidarDriver
{
    class DataFilter {
    public:

        DataFilter(){};
        ~DataFilter(){};

        static LidarScannerData LidarDataFilter(LidarScannerData &lidar_data);

    };
}

