#include "SerialPort.h"
#include <iostream>

SerialPort::SerialPort(const std::string& port, unsigned int baud_rate)
    : io(), serial(io, port) {
    serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}

SerialPort::~SerialPort() {
    serial.close();
}

void SerialPort::write(const std::string& data) {
    boost::asio::write(serial, boost::asio::buffer(data));
}

std::string SerialPort::read() {
    char buf[1024];
    size_t len = boost::asio::read(serial, boost::asio::buffer(buf));
    return std::string(buf, len);
}