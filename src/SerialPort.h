#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <boost/asio.hpp>
#include <string>

class SerialPort {
public:
    SerialPort(const std::string& port, unsigned int baud_rate);
    ~SerialPort();

    void write(const std::string& data);
    std::string read();

private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
};

#endif // SERIALPORT_H