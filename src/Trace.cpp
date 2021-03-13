

#include "Trace.hpp"

float Algorithm::Trace::GetDrivingDirection(LidarDriver::LidarScannerData &data) {
    lidar_data.Clear();
    int state;
    float angle=255;
    for(int i=0;i<data.GetSize();i++)
    {
        if(data.angle[i]>180)break;
        lidar_data.angle[i]=data.angle[i];
        lidar_data.distance[i]=data.distance[i];
    }
    int is_continous=-1;
    for(int i=1;i<lidar_data.GetSize();i++)
    {
        if(std::abs(lidar_data.distance[i]-lidar_data.distance[i-1])>0.1)break;
        if(lidar_data.angle[i]>30){
            is_continous=1;
            break;
        }
    }
    for(int i=lidar_data.GetSize()-2;i>=0;i--)
    {
        if(std::abs(lidar_data.distance[i]-lidar_data.distance[i+1])>0.1)break;;
        if(lidar_data.angle[i]<150){
            is_continous=2;
            break;
        }
    }
    switch (is_continous) {
        case -1:        //无连续
            break;
        case 1:         //左连续
            state=IsApprochingWall(lidar_data.distance[0],LastLeft);
            if(state==0){angle= 90;}
            if(state==1){angle= 135;}
            if(state==-1){angle= 45;}
            break;
        case 2:         //右连续
            state=IsApprochingWall(lidar_data.distance[0],LastLeft);
            if(state==0){angle= 90;}
            if(state==1){angle= 45;}
            if(state==-1){angle= 135;}
            break;
        default:

            break;
    }
    if(angle==90)
    {
        bool Flag =false; // whether exist a interval that distance > MinDiatance
        for(int i=0;i<lidar_data.GetSize();i++)
        {
            if(lidar_data.angle[i]<30||lidar_data.angle[i]>150)continue;
            if(lidar_data.distance[i]>MinDistance&&lidar_data)
        }
    }
    return angle;
}
/**
 * @brief 判断当前位置和墙壁之间距离关系 做出方向决策
 * @param current_distance
 * @param last_distance
 * @return -1 迫近 0 直行 1 远离
 */
int Algorithm::Trace::IsApprochingWall(float current_distance, float last_distance) {
    if(std::abs(current_distance-last_distance)<MinInterval)
    {
        return 0;
    }
    if(current_distance>last_distance)
    {
        return 1;
    }
    return -1;
}
