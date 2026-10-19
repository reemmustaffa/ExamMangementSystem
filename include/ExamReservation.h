#ifndef EXAM_RESERVATION_H
#define EXAM_RESERVATION_H
#include <string>
using namespace std;

class ExamReservation {
    private:
    string reservationId;
    string username;
    string examDate;
    string examCode;
    int cur_question;
    int score;
    int status;
    int rem_time;
    int pass_score;

    public:
    ExamReservation(string reservationId, string username, string examDate, string examCode, int cur_question, int score, int status, int rem_time, int pass_score);

    string getReservationId();
    string getUsername();
    string getExamDate();
    string getExamCode();
    int getCurQuestion();
    int getScore();
    int getStatus();
    int getRemTime();
    int getPassScore();

    void start();
    void finish();
};

#endif