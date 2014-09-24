#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <vector>

void splitString(const std::string& splitString, const std::string& str, std::vector<std::string>* numbers);
std::string trim(const std::string& s,const std::string& drop = " \n\r\t");

#endif