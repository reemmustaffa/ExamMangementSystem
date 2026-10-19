#include "ExamReservation.h"
using namespace std;

ExamReservation::ExamReservation(string reservationId, string username, string examDate, string examCode, int cur_question, int score, int status, int rem_time, int pass_score) {
    this->reservationId = reservationId;
    this->username = username;
    this->examDate = examDate;
    this->examCode = examCode;
    this->cur_question = cur_question;
    this->score = score;
    this->status = status;
    this->rem_time = rem_time;
    this->pass_score = pass_score;
}

string ExamReservation::getReservationId() { return reservationId; }
string ExamReservation::getUsername() { return username; }
string ExamReservation::getExamDate() { return examDate; }
string ExamReservation::getExamCode() { return examCode; }
int ExamReservation::getCurQuestion() { return cur_question; }
int ExamReservation::getScore() { return score; }
int ExamReservation::getStatus() { return status; }
int ExamReservation::getRemTime() { return rem_time; }
int ExamReservation::getPassScore() { return pass_score; }