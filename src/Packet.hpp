#pragma once
#include<vector>
namespace LidarDriver
{

    class Packet
    {

    private:
        std::vector<unsigned char> buffer;
        int Length;
    public:
        void Push(unsigned char);
        int GetSize();
        int GetMaxLength();
        void Reset();
        void SetLength(int length);
        bool CheckSum();
        unsigned short CalculateSum(int start_address,int end_address);
        unsigned short CalculateSumXor(int start_address,int end_address);
        // unsafe
        unsigned char GetParameter(){return buffer[8];}
        unsigned short GetParameterLength();
        unsigned char* GetParameterPointer();
        static unsigned short CompositeNumber(unsigned char * data);
        unsigned char GetCommandID();
        unsigned char GetVerifyType(){return buffer[3];}
        //CRC 校验计算

    };
}



