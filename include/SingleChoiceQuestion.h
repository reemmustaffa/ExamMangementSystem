#ifndef SINGLECHOICEQUESTION_H
#define SINGLECHOICEQUESTION_H

#include "Question.h"

class SingleChoiceQuestion : public Question {
public:
    SingleChoiceQuestion(string subjectCode, string sectionCode, int number, string header);

    int getType() const override;
    void display(int position) const override;
    double grade(const set<int>& selected) const override;
    void validateSelection(const set<int>& selected) const override;
    void validateDefinition() const override;
};

#endif
