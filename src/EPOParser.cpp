#include "EPOParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<EPOSet> EPOParser::parseEPOFile(const std::string& filename) {
    std::vector<EPOSet> epo_sets;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        EPOSet set;
        std::istringstream iss(line);
        iss >> set.gps_week >> set.hour >> set.second >> set.valid_from >> set.valid_to;
        epo_sets.push_back(set);
    }

    return epo_sets;
}
