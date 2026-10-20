#include "Subject.h"
#include <iostream>
using namespace std;

Subject::Subject(string sub_code, string sub_name, int sub_hours){
    this->sub_code=sub_code;
    this->sub_name=sub_name;
    this->sub_hours=sub_hours;
}

string Subject::getSubjectCode(){
    return sub_code;
}

string Subject::getSubjectName(){
    return sub_name;
}

int Subject::getSubjectHours(){
    return sub_hours;
}
