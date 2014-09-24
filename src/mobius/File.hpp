#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <sstream>

void loadFileIntoString(const std::string& pFileName, std::istringstream* pStream);
void saveStringIntoFile(const std::string& pFileName, const std::string& pString);

#endif