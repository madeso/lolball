#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

// data formated in a bad way
class BadDataException : public std::runtime_error {
public:
	BadDataException(const std::string& pCause) : std::runtime_error(pCause) { }
};

// thrown when missing a required resource
class MissingResourceException : public std::runtime_error {
public:
	MissingResourceException(const std::string& pCause) : std::runtime_error(pCause) { }
};

// throws during init, if a library fails to start
class InitException : public std::runtime_error {
public:
	InitException(const std::string& pCause) : std::runtime_error(pCause) { }
};

void handleExceptions();

#endif