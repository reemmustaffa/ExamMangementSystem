#ifndef MULTIPLECHOICEQUESTION_H
#define MULTIPLECHOICEQUESTION_H

#include "Question.h"

class MultipleChoiceQuestion : public Question {
public:
    MultipleChoiceQuestion(string subjectCode, string sectionCode, int number, string header);

    int getType() const override;
    void display(int position) const override;
    double grade(const set<int>& selected) const override;
    void validateSelection(const set<int>& selected) const override;
    void validateDefinition() const override;
};

#endif
