#ifndef STUDENT_H
#define STUDENT_H
#include "User.h"
#include "ExamReservation.h"
#include "ExamDetails.h"
#include "RunQuestion.h"
#include "Exam.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class Student : public User {
    private:
    vector <ExamReservation> reservations;
    map<string, Exam> exames;
    vector<ExamDetails> details;

    public:
    Student(string username, string firstName, string lastName, string passwordHash);
    void viewReservations();
    bool showExamDetails(string examCode);
    void selectExam();

    void generateExam(ExamReservation& res, vector<ExamDetails> details); 
    void startExam(ExamReservation& res, vector<ExamDetails> details);



void loadRunning(ExamReservation& res, vector<RunQuestion>& qs);
void showQuestion(RunQuestion& q, int total, int remTime);
void runExam(ExamReservation& res);
void showResult(ExamReservation& res, vector<RunQuestion>& qs, vector<ExamDetails>& details);

};
#endif