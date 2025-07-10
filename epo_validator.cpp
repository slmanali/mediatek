#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cstdint>

// Function to convert GPS time (week number and seconds) to UTC
std::string gps_time_to_utc(uint32_t gps_week, uint32_t gps_seconds) {
    // GPS epoch starts on January 6, 1980
    std::tm gps_epoch = {0, 0, 0, 6, 0, 80}; // January 6, 1980
    std::time_t epoch_time = std::mktime(&gps_epoch);

    // Calculate total seconds since GPS epoch
    uint32_t seconds_per_week = 604800; // 7 days * 24 hours * 3600 seconds
    std::time_t gps_time = epoch_time + (gps_week * seconds_per_week) + gps_seconds;

    // Convert to UTC string
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::gmtime(&gps_time));
    return std::string(buffer);
}

// Function to validate EPO file structure
bool validate_epo_file(const std::string& file_path, size_t set_size) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return false;
    }

    // Get file size
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Check if file size is a multiple of set_size
    if (file_size % set_size != 0) {
        std::cerr << "Invalid file size. Expected a multiple of " << set_size << std::endl;
        return false;
    }

    std::cout << "File is valid. Contains " << file_size / set_size << " sets." << std::endl;
    return true;
}

// Function to extract timestamps from EPO file
void extract_timestamps(const std::string& file_path, size_t set_size) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }

    std::vector<char> buffer(set_size);
    size_t set_number = 0;

    while (file.read(buffer.data(), set_size)) {
        // Adjust the offset based on the EPO file format
        size_t offset = 0; // Change this to the correct offset for your EPO file
        uint32_t gps_week = static_cast<uint8_t>(buffer[offset]) |
                            (static_cast<uint8_t>(buffer[offset + 1]) << 8) |
                            (static_cast<uint8_t>(buffer[offset + 2]) << 16) |
                            (static_cast<uint8_t>(buffer[offset + 3]) << 24);

        uint32_t gps_seconds = static_cast<uint8_t>(buffer[offset + 4]) |
                               (static_cast<uint8_t>(buffer[offset + 5]) << 8) |
                               (static_cast<uint8_t>(buffer[offset + 6]) << 16) |
                               (static_cast<uint8_t>(buffer[offset + 7]) << 24);
        
        gps_week = gps_week & 0x3FF; // Mask to 10 bits

        // Print raw GPS week and seconds for debugging
        std::cout << "Set " << ++set_number << " - GPS Week: " << gps_week << ", GPS Seconds: " << gps_seconds << std::endl;

        // Convert GPS time to UTC and print
        std::cout << "Set " << set_number << " valid from: " << gps_time_to_utc(gps_week, gps_seconds) << std::endl;
    }

    if (!file.eof()) {
        std::cerr << "Error reading file: " << file_path << std::endl;
    }
}

int main() {
    std::string file_path = "EPO.DAT";
    size_t set_size = 2304; // Adjust for Type I (1920 bytes) or Type II (2304 bytes)

    // Validate EPO file structure
    if (!validate_epo_file(file_path, set_size)) {
        return 1;
    }

    // Extract and print timestamps
    extract_timestamps(file_path, set_size);

    return 0;
}
// g++ -o epo_validator epo_validator.cpp
