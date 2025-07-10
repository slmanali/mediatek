#include "GPSCommands.h"
#include "EPOParser.h" // Add this line
#include "SerialPort.h" // Add this line
#include <iomanip>
#include <sstream>

std::string GPSCommands::calculateChecksum(const std::string& command) {
    uint8_t checksum = 0;
    for (char c : command) {
        checksum ^= c;
    }
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)checksum;
    return ss.str();
}

void GPSCommands::sendCommand(SerialPort& port, const std::string& command) {
    std::string full_command = "$" + command + "*" + calculateChecksum(command) + "\r\n";
    port.write(full_command);
}

void GPSCommands::clearEPOData(SerialPort& port) {
    sendCommand(port, "PMTK127");
}

void GPSCommands::uploadEPOData(SerialPort& port, const std::vector<EPOSet>& epo_sets) {
    for (const auto& set : epo_sets) {
        std::string command = "PMTK" + std::to_string(set.gps_week) + "," + std::to_string(set.hour) + "," + std::to_string(set.second);
        sendCommand(port, command);
    }
}