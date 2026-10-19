#ifndef EXAM_H
#define EXAM_H
#include <string>
using namespace std;

class Exam{
    private:
    string examCode;
    string examName;
    int examDuration;
    int maxScore;
    int passScore;

    public:
    Exam(string examCode, string examName, int examDuration, int maxScore, int passScore);

    string getExamCode();
    string getExamName();
    int getExamDuration();
    int getMaxScore();
    int getPassScore();

    void addDetail();
    void display();
};

#endif