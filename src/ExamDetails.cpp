#include "ExamDetails.h"
#include "ExamException.h"
#include <iostream>
using namespace std;

ExamDetails::ExamDetails(string exam_code, string sub_code, string sec_code, int no_ques) {
    this->exam_code = exam_code;
    this->sub_code = sub_code;
    this->sec_code = sec_code;
    this->no_ques = no_ques;
}

string ExamDetails::getExamCode() {
    return exam_code;
}

string ExamDetails::getSubCode() {
    return sub_code;
}

string ExamDetails::getSecCode() {
    return sec_code;
}

int ExamDetails::getNoQues() {
    return no_ques;
}

void ExamDetails::validate() {
    if(this->no_ques <= 0){
        throw ExamException("Question count must be greater than zero.");
    }
   
}