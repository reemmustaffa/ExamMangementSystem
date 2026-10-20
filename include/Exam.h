#ifndef EXAM_H
#define EXAM_H
#include <string>
#include <vector>
#include "ExamDetails.h"
using namespace std;

class Exam{
    private:
    string examCode;
    string examName;
    int examDuration;
    int maxScore;
    int passScore;
    vector<ExamDetails> details;

    public:
    Exam(string examCode, string examName, int examDuration, int maxScore, int passScore);

    string getExamCode();
    string getExamName();
    int getExamDuration();
    int getMaxScore();
    int getPassScore();

    void addDetail(ExamDetails d);
    vector <ExamDetails> getDetails();
    int getTotalQuestions();
    void display();
};

#endif