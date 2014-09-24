#include "StringUtils.hpp"

#include <cassert>
#include "boost/tokenizer.hpp"

void splitString(const std::string& splitString, const std::string& str, std::vector<std::string>* numbers) {
	assert(numbers);
	boost::char_separator<char> sep(splitString.c_str());
	boost::tokenizer< boost::char_separator<char> > tok(str, sep);
	std::copy(tok.begin(), tok.end(), std::back_inserter(*numbers));
}

std::string trim(const std::string& s,const std::string& drop) {
 std::string r = s;
 r=r.erase(s.find_last_not_of(drop)+1);
 return r.erase(0,r.find_first_not_of(drop));
}