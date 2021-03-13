#pragma once
#include <boost/asio.hpp>
#include <string>
#include <stdexcept>
#include<iostream>
namespace LidarDriver
{

    class SerialConnection
    {

    protected:
        //boost
        boost::asio::io_context Context;
        //串口对象
        boost::asio::serial_port Port;
    public:
        //当前串口状态
        enum class Status
        {
            StatusNeverOpened,
            StatusOpened,
            StatusOpenedError,
            StatusClosed,
            StatusClosedError
        };

        ///构造 析构函数
        SerialConnection();
        //显示调用构造函数 防止隐式转换
        explicit SerialConnection(std::string device_name);

        virtual ~SerialConnection() noexcept;

        //设定缓存区大小
        std::size_t BufferSize{ 1024 };


        void SetDeviceName(std::string device_name)
        {
            DeviceName=device_name;
        }
        /**
         * @brief 调用boost::asio实现对设备串口的开启
         */
        bool Open();
        /**
         * @brief 调用boost::asio 实现对设备串口的关闭
         */

        bool Close();

        /**
         * @brief 判断当前串口是否开启
         * @return false关闭 true开启
         */
        bool IsOpened() const noexcept
        {
            return Port.is_open();
        }

        /**
         * @brief 获取当前设备的状态
         * @return 参见enum class Status
         */
        Status GetStatus();

        /**
         * @brief 输出字符数组文本
         * @param data
         * @details 本质上调用Write(const void* start_address,std::size_t Length)
         */
        void Write(const std::vector<unsigned char> &data);
        /**
         * @brief 输出字符串文本
         * @param text
         * @details Write(const void* start_address,std::size_t Length)
         */
        void Write(const std::string &text);
        /**
         * @brief 通过指针调用指定长度内容的输出
         * @param start_address 内容的起始地址
         * @param length 内容的长度
         */
        void Write(const void* start_address,std::size_t length);

        /**
         * @brief 读取单个字符
         * @return
         */
        std::vector<unsigned char> Read();

        /**
         * @brief 读取指定长度的字符读
         * @param length
         * @return
         */
        std::vector<unsigned char> Read(std::size_t length);

        /**
         * @brief 读取字符串
         * @return
         */
        std::string ReadString();

        /**
         * @brief 读取指定长度字符串
         * @param length
         * @return
         */
        std::string ReadString(std::size_t length);

        //设置波特率
        void SetBaudRate(int baud_rate);
        //设置流控制
        using flow_control = boost::asio::serial_port::flow_control;
        void SetFlowControlType(flow_control::type type = flow_control::none);
        //设置奇偶校验
        using parity = boost::asio::serial_port::parity;
        void SetParityType(parity::type type=parity::none);
        //设置停止位
        using stop_bits = boost::asio::serial_port::stop_bits;
        void SetStopBits(stop_bits::type type= stop_bits::one);
        //设置单词传输的长度
        void SetCharacterSize(unsigned int size = 8);

        void SendData(float data);

        bool FastOpen(std::string PortName);
    private:
        Status SerialPortStatus;
        std::string DeviceName;
    };
}
