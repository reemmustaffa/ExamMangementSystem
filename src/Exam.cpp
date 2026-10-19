#include "Exam.h"
#include <iostream>
using namespace std;

Exam::Exam(string examCode, string examName, int examDuration, int maxScore, int passScore) {
    this->examCode = examCode;
    this->examName = examName;
    this->examDuration = examDuration;
    this->maxScore = maxScore;
    this->passScore = passScore;
}

string Exam::getExamCode() {
    return examCode;
}

string Exam::getExamName() {
    return examName;
}

int Exam::getExamDuration() {
    return examDuration;
}

int Exam::getMaxScore() {
    return maxScore;
}

int Exam::getPassScore() {
    return passScore;
}

void Exam::display() {
    // Screen 3 later
}