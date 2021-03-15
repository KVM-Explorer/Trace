

#include "ShowLidarData.hpp"
#include <opencv2/opencv.hpp>

#include <iostream>
using namespace LidarDriver;

void ShowLidarData::ShowLidarChart(LidarScannerData lidar_data)
{
//    ReviseData(lidar_data);
    cv::Mat image(800,800,CV_8UC3,cv::Scalar(0,0,0));
    int sx,sy;
    int maxx=10;
    sx=sy=400;
    cv::line(image,cv::Point(400,400),cv::Point(700,100),cv::Scalar(0,0,255));
    cv::line(image,cv::Point(400,400),cv::Point(100,100),cv::Scalar(0,0,255));
    cv::line(image,cv::Point(100,400),cv::Point(700,400),cv::Scalar(0,0,255));
    for(int i=0;i<lidar_data.GetSize();i++)
    {
        float angle = lidar_data.angle[i];
        float distance = 400*lidar_data.distance[i]/float(maxx);
//        std::cout<<distance<<std::endl;
        int h=sin(angle*3.14/180.0)*distance;
        int w=cos(angle*3.14/180.0)*distance;
        cv::circle(image,cv::Point(sy-w,sx-h),1,cv::Scalar(22,122,234),-1);

    }

    cv::imshow("Map",image);
    cv::waitKey(1);
}

void ShowLidarData::ReviseData(LidarScannerData &lidar_data)
{
    for(int i=0;i<lidar_data.GetSize();i++)
    {
        lidar_data.angle[i]-=180;
        if(lidar_data.angle[i]<0)lidar_data.angle[i]+=360;
    }

}

void ShowLidarData::ShowCurrentArrow(float angle) {
    cv::Mat image(800,800,CV_8UC3,cv::Scalar(0,0,0));
    int sx,sy;
    int maxx=10;
    sx=sy=400;
    int distance=400;
    int h=sin(angle*3.14/180.0)*distance;
    int w=cos(angle*3.14/180.0)*distance;
    cv::line(image,cv::Point(sx,sy),cv::Point(sy-w,sx-h),cv::Scalar(0,0,255),2);
    cv::putText(image,std::to_string(angle),cv::Point(100,100),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(255,255,255));
    cv::imshow("CurrentArrow",image);
    cv::waitKey(1);
}

auto ShowLidarData::GetTime_MS() {
    return  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

