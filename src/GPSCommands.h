#ifndef GPSCOMMANDS_H
#define GPSCOMMANDS_H

#include <string>
#include <vector> // Add this line

class GPSCommands {
public:
    static std::string calculateChecksum(const std::string& command);
    static void sendCommand(class SerialPort& port, const std::string& command);
    static void clearEPOData(class SerialPort& port);
    static void uploadEPOData(class SerialPort& port, const std::vector<struct EPOSet>& epo_sets);
};

#endif // GPSCOMMANDS_H