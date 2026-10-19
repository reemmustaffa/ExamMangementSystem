#ifndef STUDENT_H
#define STUDENT_H
#include "User.h"
#include "ExamReservation.h"
#include <vector>
#include <string>
using namespace std;

class Student : public User {
    private:
    vector <ExamReservation> reservations;
    public:
    Student(string username, string firstName, string lastName, string passwordHash);
    void viewReservations();
    void startExam();

};
#endif