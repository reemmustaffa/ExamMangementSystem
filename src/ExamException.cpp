#include "ExamException.h"
using namespace std;

ExamException::ExamException(string msg) : runtime_error(msg) {
}

InvalidCredentialsException::InvalidCredentialsException() : ExamException("Invalid username or password.") {
}

FileException::FileException(string msg) : ExamException(msg) {
}