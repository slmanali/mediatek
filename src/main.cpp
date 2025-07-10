#include "SerialPort.h"
#include "EPOParser.h"
#include "GPSCommands.h"
#include <iostream>

int main() {
    try {
        SerialPort port("/dev/ttymxc1", 115200);

        // Clear existing EPO data
        GPSCommands::clearEPOData(port);

        // Parse EPO file
        std::vector<EPOSet> epo_sets = EPOParser::parseEPOFile("epo_files/MTK14.EPO");

        // Upload EPO data
        GPSCommands::uploadEPOData(port, epo_sets);

        std::cout << "EPO data uploaded successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}