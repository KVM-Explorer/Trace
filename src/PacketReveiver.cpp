#include "PacketReveiver.hpp"
#include <iostream>
#include <stdexcept>


namespace LidarDriver{
    #define PACKET_HEADER1 0xAA
    #define PACKET_HEADER2 0x00

    PacketReceiver::PacketReceiver() {
        State=PacketContentState::Header1;

    }

    bool PacketReceiver::ReceivePacket(Packet &packet) {
        if( DeviceConnection->GetStatus() != SerialConnection::Status::StatusOpened)
        {
            throw std::logic_error("PacketReceiver::ReceiverPacket deviec isn't opened\n");
            return false;
        }
        unsigned char ch;
        while(1)
        {
            ch=DeviceConnection->Read(1)[0];
            PacketState packet_state = ReadPacket(packet,ch);
            switch (packet_state)
            {
                case PacketState::GettingPacket: break;
                case PacketState::GotPacket:
                {
                    ResetReceiveState();
                    return true;
                }
                case PacketState::FailedGetPacket:
                {
                    ResetReceiveState();
                    return false;
                }
                default:
                    throw std::logic_error("PacketReceiver::ReceivePacket fail to interpret information"+std::to_string(int(packet_state)));

            }

        }
    }

    PacketReceiver::PacketState PacketReceiver::ReadPacket(Packet &packet, unsigned char ch)
    {
        PacketState packet_state =PacketState::GettingPacket;

        switch (State)
        {
            case PacketContentState::Header1: packet_state = ProcessHeader1(packet,ch); break;
            case PacketContentState::Header2: packet_state = ProcessHeader2(packet,ch); break;
            case PacketContentState::Length: packet_state = ProcessLength(packet,ch); break;
            case PacketContentState::AcquistionData: packet_state = ProcessAcquistionData(packet,ch); break;
            default:
                throw std::logic_error("PackerReceiver::ReadPacket data can't be processed correctly!");
                break;

        }
        return packet_state;
    }

    PacketReceiver::PacketState PacketReceiver::ProcessHeader1(Packet &packet, unsigned char ch)
    {
        if(ch==PACKET_HEADER1)
        {
            packet.Reset();
            packet.Push(ch);
            State=PacketContentState::Header2;
        }
        return PacketState::GettingPacket;
    }

    PacketReceiver::PacketState PacketReceiver::ProcessHeader2(Packet &packet, unsigned char ch)
    {

        if(ch==PACKET_HEADER2)
        {
            packet.Push(ch);
            State=PacketContentState::Length;
        }
        else
        {
            ResetReceiveState();
            std::logic_error("PacketReceiver find error header2 "+std::to_string(int(ch))+"\n");

        }
        return PacketState::GettingPacket;
    }
    PacketReceiver::PacketState PacketReceiver::ProcessLength(Packet &packet, unsigned char ch)
    {
        if(ch<6 || ch>250)
        {
            ResetReceiveState();
            std::logic_error("PacketReceiver find error Length is "+std::to_string(int(ch))+"\n");
            return PacketState::GettingPacket;
        }
        packet.Push(ch);
        State=PacketContentState::AcquistionData;
        packet.SetLength(int(ch)+2);
        return PacketState::GettingPacket;
    }

    PacketReceiver::PacketState PacketReceiver::ProcessAcquistionData(Packet &packet, unsigned char ch)
    {

        packet.Push(ch);
        if(packet.GetSize() == packet.GetMaxLength())//length =CRC +参数 -帧头
        {
            ResetReceiveState();
            if(packet.CheckSum()){
                return PacketState::GotPacket;
            }
            else
            {
                std::cout<<("PackerReceiver::ProcessAcquistionData CRC verify wrong!\n");
                return PacketState::FailedGetPacket;
            }
        }
        return PacketState::GettingPacket;
    }

    void PacketReceiver::ResetReceiveState()
    {
        State = PacketContentState::Header1;

    }
}





