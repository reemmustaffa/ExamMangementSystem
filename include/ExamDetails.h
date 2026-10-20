#ifndef EXAM_DETAILS_H
#define EXAM_DETAILS_H
#include <string>
using namespace std;

class ExamDetails{
    private:
    string exam_code;
    string sub_code;
    string sec_code;
    int no_ques;

    public:
    ExamDetails(string exam_code, string sub_code, string sec_code, int no_ques);

    string getExamCode();
    string getSubCode();
    string getSecCode();
    int getNoQues();

    void display();
    void validate();

};

#endif