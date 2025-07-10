#ifndef EPOPARSER_H
#define EPOPARSER_H

#include <vector>
#include <string>

struct EPOSet {
    int gps_week;
    int hour;
    int second;
    std::string valid_from;
    std::string valid_to;
};

class EPOParser {
public:
    static std::vector<EPOSet> parseEPOFile(const std::string& filename);
};

#endif // EPOPARSER_H