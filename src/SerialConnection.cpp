#include "SerialConnection.hpp"


namespace LidarDriver
{

    SerialConnection::SerialConnection():Context(),Port(Context)
    {
        SerialPortStatus=Status::StatusNeverOpened;
    }

    SerialConnection::~SerialConnection() noexcept
    {
        if(IsOpened())
        {
            try
            {
                Port.close();
                SerialPortStatus=Status::StatusClosed;
            }
            catch (boost::system::system_error &error)
            {
                std::clog <<"SerialConnection::SerialPort Failed to close device:"<<DeviceName<<std::endl;
                SerialPortStatus=Status::StatusClosedError;
            }
        }
    }


    bool SerialConnection::Open()
    {
        if(SerialPortStatus==Status::StatusNeverOpened)
        {
            try{
                Port.open(DeviceName);
                SerialPortStatus=Status::StatusOpened;
                return true;
            }
            catch (boost::system::system_error &error)
            {
                SerialPortStatus=Status::StatusOpenedError;
                std::clog <<"SerialConnection::SerialPort Failed to open device:"<<DeviceName<<std::endl;
                return false;
            }

        }
        else
        {
            if(SerialPortStatus==Status::StatusOpened) return true;
            SerialPortStatus=Status::StatusNeverOpened;
            return Open();
        }
    }

    bool SerialConnection::Close()
    {
        try
        {
            Port.close();
            return true;
        }
        catch (boost::system::system_error &error)
        {
            SerialPortStatus=Status::StatusClosedError;
            std::clog <<"SerialConnection::SerialPort Failed to close device:"<<DeviceName<<std::endl;
        }
    }

    SerialConnection::Status SerialConnection::GetStatus()
    {
        return SerialPortStatus;
    }

    void SerialConnection::Write(const std::vector<unsigned char> &data)
    {
        Write(static_cast<const void*>(data.data()),data.size());
    }

    void SerialConnection::Write(const std::string &text)
    {
        Write(static_cast<const void*>(text.data()),text.size());
    }

    void SerialConnection::Write(const void *start_address, std::size_t length)
    {
        if(Port.is_open())
        {
            boost::asio::write(Port,boost::asio::buffer(start_address,length));
        }
        else
        {
            throw std::logic_error("SerialConnection::Write Device is not opened");
        }
    }

    std::vector<unsigned char> SerialConnection::Read()
    {
        if(Port.is_open())
        {
            std::vector<unsigned char> buffer;
            buffer.resize(BufferSize);
            //读取完成后返回
            std::size_t length = Port.read_some(boost::asio::buffer(buffer.data(),buffer.size()));
            buffer.resize(length);
            return buffer;
        }
        else
        {
            throw std::logic_error("SerialConnection::Read Device is not opened");
        }
    }

    std::vector<unsigned char> SerialConnection::Read(std::size_t length)
    {
        if(Port.is_open())
        {
            std::vector<unsigned char> buffer;
            buffer.resize(length);
            //此处阻塞至缓冲区填满
            boost::asio::read(Port,
                              boost::asio::buffer(buffer.data(),buffer.size()));
            return buffer;
        }
        else
        {
            throw std::logic_error("SerialConnection::Read Device is not opened");
        }
    }

    std::string SerialConnection::ReadString()
    {
        if(Port.is_open())
        {
            std::string buffer;
            buffer.resize(BufferSize);
            std::size_t  length  =Port.read_some(boost::asio::buffer(buffer.data(),buffer.size()));
            buffer.resize(length);
            return buffer;
        }
        else
        {
            throw std::logic_error("SerialConnection::ReadString Device is not opened");
        }
    }

    std::string SerialConnection::ReadString(std::size_t length)
    {
        if(Port.is_open())
        {
            std::string buffer;
            buffer.resize(length);
            // 此处会阻塞直到缓冲区被填满
            boost::asio::read(Port, boost::asio::buffer(buffer.data(), buffer.size()));
            return buffer;
        }
        else
        {
            throw std::logic_error("SerialConnection::ReadString Device is not opened");
        }
    }



    void SerialConnection::SetBaudRate(int baud_rate)
    {
        if(Port.is_open())
        {
            Port.set_option(boost::asio::serial_port::baud_rate(baud_rate));
        }
        else
        {
            throw std::logic_error("SerialConnection::SetBaudRate Device is not opened");
        }
    }

    void SerialConnection::SetFlowControlType(boost::asio::serial_port_base::flow_control::type type)
    {
        if(Port.is_open())
        {
            Port.set_option(boost::asio::serial_port::flow_control(type));
        }
        else
        {
            throw std::logic_error("SerialConnection::SetFlowControlType Device is not opened");
        }

    }

    void SerialConnection::SetParityType(parity::type type)
    {
        if(Port.is_open())
        {
            Port.set_option(boost::asio::serial_port::parity(type));
        }
        else
        {
            throw std::logic_error("SerialConnection::SetParityType Device is not opened");
        }
    }

    void SerialConnection::SetStopBits(stop_bits::type type)
    {
        if(Port.is_open())
        {
            Port.set_option(boost::asio::serial_port::stop_bits(type));
        }
        else
        {
            throw std::logic_error("SerialConnection::SetStopBits Device is not opened");
        }
    }

    void SerialConnection::SetCharacterSize(unsigned int size)
    {
        if(Port.is_open())
        {
            Port.set_option(boost::asio::serial_port::character_size(size));
        }
        else
        {
            throw std::logic_error("SerialConnection::SetCharacterSize Device is not opened");
        }
    }

    void SerialConnection::SendData(float data)
    {
        std::string text;
        text.clear();

        unsigned char angle_int = int(data)%1000;
        unsigned char angle_tail = int(data*100)%100;

        text.push_back(angle_int);
        text.push_back(angle_tail);

        Write(text);


    }
    bool SerialConnection::FastOpen(std::string PortName)
    {
        SetDeviceName(PortName);
        Open();
        SetBaudRate(230400);
        SetCharacterSize();
        SetFlowControlType();
        SetStopBits();
        SetParityType();
    }
}

