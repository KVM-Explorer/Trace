#pragma once
#include "Packet.hpp"
#include "SerialConnection.hpp"


namespace LidarDriver
{
    class PacketReceiver
    {
    public:
        PacketReceiver();

        ~PacketReceiver(){};

        bool ReceivePacket(Packet &packet);

        void SetDeviceConnection(LidarDriver::SerialConnection *device_connection)
        {
            DeviceConnection=device_connection;
        }
        LidarDriver::SerialConnection* GetDeviceConnection()
        {
            return DeviceConnection;
        }


        enum class PacketContentState
        {
            Header1,
            Header2,
            Length,
            AcquistionData
        };

        enum class PacketState
        {
            GettingPacket,
            GotPacket,
            FailedGetPacket
        };
    private:
        void ResetReceiveState();
        PacketState ReadPacket(Packet &packet,unsigned char ch);
        PacketState ProcessHeader1(Packet &packet,unsigned char ch);
        PacketState ProcessHeader2(Packet &packet,unsigned char ch);
        PacketState ProcessLength(Packet &packet,unsigned char ch);
        PacketState ProcessAcquistionData(Packet &packet,unsigned char ch);




    private:
        LidarDriver::SerialConnection *DeviceConnection;
        PacketContentState State;
    };
}



