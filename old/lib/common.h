#ifndef COMMON__H
#define COMMON__H

#include <vector>
#include <string>

std::vector<std::string> tokenize(std::string s, const char delimiter=' ') {
    size_t start;
    size_t end = 0;

    std::vector<std::string> output;

    while ((start = s.find_first_not_of(delimiter, end)) != std::string::npos) {
        end = s.find(delimiter, start);
        output.push_back(s.substr(start, end - start));
    }

    return output;
}

#endif