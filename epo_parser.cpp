#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <cstdint>

// Constants
constexpr uint32_t PREAMBLE = 0x2404;
constexpr uint32_t COMMAND = 0x02d2;
constexpr uint32_t EOW = 0x0a0d;
constexpr uint32_t UART_CMD = 253;
constexpr uint32_t SATS_PER_EPO = 32;
constexpr uint32_t SECONDS_PER_HOUR = 3600;
constexpr uint32_t GPS_OFFSET_SECONDS = 315964786;
constexpr uint32_t SECONDS_PER_WEEK = 604800;
constexpr uint32_t HOURS_PER_WEEK = 168;

// Convert GPS hour to UTC time string
std::string Convert2UTC(int32_t GPSHour) {
    time_t gpsTime = GPSHour * SECONDS_PER_HOUR + GPS_OFFSET_SECONDS;
    struct tm* tm = gmtime(&gpsTime);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    return std::string(buffer);
}

// Convert GPS hour to local time string
std::string Convert2Local(int32_t GPSHour) {
    time_t gpsTime = GPSHour * SECONDS_PER_HOUR + GPS_OFFSET_SECONDS;
    struct tm* tm = localtime(&gpsTime);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    return std::string(buffer);
}

// Calculate CRC8 checksum
uint8_t crc8(const std::vector<uint8_t>& data) {
    uint8_t crc = 0;
    for (uint8_t b : data) {
        crc ^= b;
    }
    return crc;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <EPO_File>" << std::endl;
        return 1;
    }

    const std::string inputFile = argv[1];
    std::ifstream fi(inputFile, std::ios::binary);
    if (!fi) {
        std::cerr << "Error: Could not open file " << inputFile << std::endl;
        return 1;
    }

    // Read the header (75 bytes)
    std::vector<uint8_t> header(75);
    fi.read(reinterpret_cast<char*>(header.data()), header.size());

    // Determine EPO type based on header
    size_t EPO_SET_SIZE = 0;
    if (memcmp(header.data(), header.data() + 60, 3) == 0) {
        std::cout << "Opening EPO Type I file" << std::endl;
        EPO_SET_SIZE = 1920;
    } else if (memcmp(header.data(), header.data() + 72, 3) == 0) {
        std::cout << "Opening EPO Type II file" << std::endl;
        EPO_SET_SIZE = 2304;
    } else {
        std::cerr << "Error: Unknown file type." << std::endl;
        return 1;
    }

    // Read and process EPO sets
    int sets = 0;
    int32_t start = 0;
    while (true) {
        std::vector<uint8_t> epo_set(EPO_SET_SIZE);
        fi.read(reinterpret_cast<char*>(epo_set.data()), epo_set.size());
        if (fi.gcount() == 0) {
            break; // End of file
        }
        sets++;

        // Extract GPS hour from the first 3 bytes
        int32_t epo_start = static_cast<int32_t>(epo_set[0]) |
                            (static_cast<int32_t>(epo_set[1]) << 8) |
                            (static_cast<int32_t>(epo_set[2]) << 16);

        if (sets == 1) {
            start = epo_start;
        }

        // Print EPO set information
        std::cout << "Set: " << std::setw(4) << sets << ".  GPS Wk: " << std::setw(4) << (epo_start / HOURS_PER_WEEK)
                  << "  Hr: " << std::setw(3) << (epo_start % HOURS_PER_WEEK)
                  << "  Sec: " << std::setw(6) << ((epo_start % HOURS_PER_WEEK) * SECONDS_PER_HOUR)
                  << "  " << Convert2UTC(epo_start) << " to " << Convert2UTC(epo_start + 6) << " UTC" << std::endl;
    }

    fi.close();

    // Print summary
    std::cout << std::setw(4) << sets << " EPO sets.  Valid from " << Convert2UTC(start)
              << " to " << Convert2UTC(start + 6 * sets) << " UTC" << std::endl;

    return 0;
}