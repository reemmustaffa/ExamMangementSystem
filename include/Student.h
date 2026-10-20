#ifndef STUDENT_H
#define STUDENT_H
#include "User.h"
#include "ExamReservation.h"
#include "Exam.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class Student : public User {
    private:
    vector <ExamReservation> reservations;
    map<string, Exam> exames;

    public:
    Student(string username, string firstName, string lastName, string passwordHash);
    void viewReservations();
    bool showExamDetails(string examCode);
    void selectExam();
    void startExam();

};
#endif