#ifndef EXAMEXCEPTION_H
#define EXAMEXCEPTION_H
#include <stdexcept>
#include <string>
using namespace std;

class ExamException : public runtime_error {
    public:
    ExamException(string msg);
};

class InvalidCredentialsException : public ExamException {
    public:
    InvalidCredentialsException();
};

class FileException : public ExamException {
    public:
    FileException(string msg);
};

class InvalidSelectionException : public ExamException {
    public:
    InvalidSelectionException();
};

class ExamNotAvailableException : public ExamException {
    public:
    ExamNotAvailableException(string msg);
};

#endif