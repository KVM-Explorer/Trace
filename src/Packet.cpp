#include "Packet.hpp"
#include <iostream>
namespace LidarDriver
{
    void Packet::Reset()
    {
        buffer.clear();
    }

    void Packet::Push(unsigned char ch)
    {
        buffer.push_back(ch);
    }

    int Packet::GetSize()
    {
        return buffer.size();
    }

    void Packet::SetLength(int length)
    {
        Length=length;
    }
    int Packet::GetMaxLength()
    {
        return Length;
    }
    bool Packet::CheckSum()
    {
        unsigned char number2=buffer[Length-2];
        unsigned char number1=buffer[Length-1];
        unsigned short check_sum = (number2<<8)|number1;

        unsigned short calculate_sum ;

        calculate_sum=CalculateSum(0,Length-2);

        unsigned char ch=buffer[3];

        if(check_sum == calculate_sum)
        {
            return true;
        }
        else
        {
            return false;
        }

    }

    unsigned short Packet::CalculateSum(int start_address, int end_address)
    {
        unsigned short sum=0;
        for(int i=start_address;i<end_address;i++)
        {
            unsigned short data = buffer[i];
            sum+=data;
        }
        return sum;
    }

    unsigned short Packet::CalculateSumXor(int start_address, int end_address)
    {

    }
    unsigned short Packet::GetParameterLength() {
        unsigned short length = CompositeNumber(&buffer[6]);
        return length;
    }

    unsigned char *Packet::GetParameterPointer()
    {
        return &buffer[8];
    }

    unsigned short Packet::CompositeNumber(unsigned char *data)
    {
        unsigned short res = (data[0]<<8)|(data[1]);
        return res;

    }

    unsigned char Packet::GetCommandID() {
        return buffer[5];
    }
}