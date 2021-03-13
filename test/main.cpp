#include <iostream>
#include "../src/SerialConnection.hpp"
#include "../src/AnalysisData.hpp"
#include "../src/DataFilter.hpp"
#include "../src/ShowLidarData.hpp"

int main() {

    std::string PortName1 = "/dev/ttyUSB0";

    LidarDriver::SerialConnection serial_connection;
    LidarDriver::AnalysisData analysis_data;



    serial_connection.FastOpen(PortName1);

    analysis_data.InitAnalysizer(&serial_connection);

    while (true)
    {
        using namespace LidarDriver;
        ScannerState result_state = analysis_data.GetScanData();
        switch (result_state) {
            case ScannerState::LidarGarbing:
            {
                break;
            }
            case ScannerState::LidarGarbSucess:
            {
                LidarScannerData lidar_data=analysis_data.GetLidarScanData();
                //todo 设置加速度和当前速度;
                lidar_data=DataFilter::LidarDataFilter(lidar_data);
                float angle;

                ShowLidarData::ShowLidarChart(lidar_data);
                std::cout<<angle<<std::endl;

                break;
            }
            case ScannerState::LidarGarbError:
            {
                break;
            }
            case ScannerState::LidarGarbElse:
            {
                break;
            }
        }

    }

    return 0;
}
