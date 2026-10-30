#ifndef RUNNINGANSWER_H
#define RUNNINGANSWER_H

#include <string>
using namespace std;

// سطر واحد من exam_running_ans.txt:  res_id|quest_no|ans_no|corr_ans|user_ans
class RunningAnswer {
private:
    string reservationId;
    int position;       // quest_no
    int answerId;       // ans_no
    bool correct;       // corr_ans
    bool selected;      // user_ans

public:
    RunningAnswer(string reservationId, int position, int answerId, bool correct, bool selected = false);

    string getReservationId() const;
    int getPosition() const;
    int getAnswerId() const;
    bool isCorrect() const;
    bool isSelected() const;

    void setSelected(bool selected);
    string toFileLine() const;
};

#endif
