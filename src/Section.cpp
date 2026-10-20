#include "Section.h"
#include <iostream>
using namespace std;

Section::Section(string sub_code, string sec_code, string sec_name){
    this->sub_code=sub_code;
    this->sec_code=sec_code;
    this->sec_name=sec_name;
}
string Section::getSubjectCode(){
    return sub_code;
}

string Section::getSectionCode(){
    return sec_code;
}
string Section::getSectionName(){
    return sec_name;
}
