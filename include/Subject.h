#ifndef SUBJECT_H
#define SUBJECT_H
#include <string>
using namespace std;


class Subject{
    private:
    string sub_code;
    string sub_name;
    int sub_hours;

    public:
    Subject(string sub_code,string sub_name,int sub_hours);

    string getSubjectCode();
    string getSubjectName();
    int getSubjectHours();

    void display();

};

#endif