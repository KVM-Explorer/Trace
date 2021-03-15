

#include "Trace.hpp"

float Algorithm::Trace::GetDrivingDirection(LidarDriver::LidarScannerData &data) {
    lidar_data.Clear();
    int state;
    float angle=255;
    int left_ID=0,right_ID;
    //rebuild and store data
    //log: exist a bug:  turn right or left and move in straight corridor direction is 255
    //                  right is obvious ,left seem none
    //reason:   Datafilter filte <0.15m data thus there are no data 150~180 then cause the problem

    for(int i=0;i<data.GetSize();i++)
    {
        if(data.angle[i]>=180)break;
        lidar_data.angle.push_back(data.angle[i]);
        lidar_data.distance.push_back(data.distance[i]);
    }
    right_ID=lidar_data.GetSize()-1;
    int is_continous=-1;//-1 no 1 left 2 right
    for(int i=1;i<lidar_data.GetSize();i++)
    {
        if(std::abs(lidar_data.distance[i]-lidar_data.distance[i-1])>0.1)break;
        if(lidar_data.angle[i]>30){
            is_continous=1;
            left_ID=i-1;
            break;
        }
    }
    for(int i=lidar_data.GetSize()-2;i>=0;i--)
    {
        if(std::abs(lidar_data.distance[i]-lidar_data.distance[i+1])>0.1)break;;
        if(lidar_data.angle[i]<150){
            is_continous=2;
            right_ID= i+1;
            break;
        }
    }
    switch (is_continous) {
        case -1:        //无连续
            break;
        case 1:         //左连续
            state=IsApprochingWall(lidar_data.distance[left_ID],LastLeft);
            if(state==0){angle= 90;}
            if(state==1){angle= 45;}
            if(state==-1){angle= 135;}
            break;
        case 2:         //右连续
            state=IsApprochingWall(lidar_data.distance[right_ID],LastRight);
            if(state==0){angle= 90;}
            if(state==1){angle= 135;}
            if(state==-1){angle= 45;}
            break;
        default:
            break;
    }
    //updata Last position1
    LastLeft=lidar_data.distance[0];
    LastRight=lidar_data.distance[lidar_data.GetSize()-1];
    std::cout<<"LastLeft="<<LastLeft<<std::endl;
    if(LastIsStraight == true){angle=90;}
    if(angle==90)
    {
        bool Flag = false; // whether exist a interval that distance > MinDiatance
        float start_angle;
        float end_angle;
        float max_delta_theta=0;
        float result=-1;
        for(int i=0;i<lidar_data.GetSize();i++)
        {
            if(lidar_data.angle[i]<30||lidar_data.angle[i]>150)continue;
            //select accessible interval
            if(lidar_data.distance[i]>MinDistance&&!Flag){
                Flag= true;
                start_angle=lidar_data.angle[i];
            }

            if(lidar_data.distance[i]<MinDistance&&Flag)
            {
                Flag= false;
                end_angle=lidar_data.angle[i];
                //select best(max) interval and through move horizontally to align the accessible area
                if(end_angle-start_angle>max_delta_theta
//                &&IsPassGate(start_angle,end_angle)
                    ){
                    max_delta_theta=end_angle-start_angle;
                    result = (start_angle+end_angle)/2.0;
                }
            }
        }
        //LastIsStraight is a state lock about horizontal movement
        if(result!=-1){
            std::cout<<"Recommend:"<<result<<std::endl;
            if(result>95)   {LastIsStraight=true;  return 180;}
            if(result<85)   {LastIsStraight=true; return 0;}
                             LastIsStraight= false; return 90;
        }else{
            //there is no accessible way to go
            return 255;
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
/**
 * @brief 判断当前angle1-angle2 范围内是否可通行
 * @param angle1
 * @param angle2
 * @return
 */
bool Algorithm::Trace::IsPassGate(float angle1,float angle2) {
    float delta_angle = angle2-angle1;
    float width = 2*MinDistance*sin(delta_angle/2*M_PI/180);
    if(width>=CarWidth) {
        return true;
    }
    return false;
}
