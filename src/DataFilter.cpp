
#include "DataFilter.hpp"

LidarDriver::LidarScannerData LidarDriver::DataFilter::LidarDataFilter(LidarScannerData &lidar_data)
{
    LidarScannerData filted_data;
    filted_data.Clear();
    int length = lidar_data.GetSize();
    for(int i=0;i<length; i ++)
    {
        if(lidar_data.distance[i]<0.15 || lidar_data.distance[i] >7.0f)
        {
            continue;
        }
        filted_data.angle.push_back(lidar_data.angle[i]);
        filted_data.distance.push_back(lidar_data.distance[i]);
    }
    return filted_data;
}