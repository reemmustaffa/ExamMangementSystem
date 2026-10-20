#include "Exam.h"
#include <iostream>
#include "ExamException.h"
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

void Exam::addDetail(ExamDetails d) {
    d.validate();
    for(int i=0;i<details.size();i++){
        if(details[i].getSubCode()==d.getSubCode() && details[i].getSecCode()==d.getSecCode()){
            throw ExamException("Duplicate subject and section in exam details.");
        }
    }
    details.push_back(d);
}


vector<ExamDetails> Exam::getDetails() {
    return details;
}

int Exam::getTotalQuestions(){
    int total;
    for(int i=0;i<details.size();i++){
        total+=details[i].getNoQues();
    }
    return total;
}