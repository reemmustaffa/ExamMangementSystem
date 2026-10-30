#ifndef RUNNINGQUESTION_H
#define RUNNINGQUESTION_H

#include <string>
#include <vector>
#include <set>
#include "Question.h"
#include "RunningAnswer.h"
using namespace std;

// سطر من exam_running.txt:  res_id|quest_no|subject_code|quest_real_no|quest_score
class RunningQuestion {
private:
    string reservationId;
    int position;                    // رقم السؤال في الامتحان (Q1, Q2, ...)
    const Question* question;        // Aggregation: بنشاور على السؤال بس، مش مالكينه
    int score;
    vector<RunningAnswer> answers;   // Composition: إجابات الطالب مملوكة للـ RunningQuestion

public:
    RunningQuestion(string reservationId, int position, const Question* question, int score = 0);

    string getReservationId() const;
    int getPosition() const;
    const Question* getQuestion() const;
    int getScore() const;
    const vector<RunningAnswer>& getAnswers() const;

    set<int> getSelectedIds() const;
    bool isAnswered() const;

    void setSelectedAnswers(const set<int>& ids);       // اختيار الطالب (يتفحص قبل ما يتخزن)
    void restoreAnswer(int answerId, bool selected);    // عند الـ Resume: نرجّع الإجابة من الملف

    int calculateScore();                               // 1 لو صح، 0 لو غلط
    string toFileLine() const;                          // سطر exam_running.txt
};

#endif
