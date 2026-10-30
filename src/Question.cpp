#include "Question.h"
#include "ExamException.h"
#include <iostream>
using namespace std;

Question::Question(string subjectCode, string sectionCode, int number, string header) {
    this->subjectCode = subjectCode;
    this->sectionCode = sectionCode;
    this->number = number;
    this->header = header;
}

void Question::addAnswer(const Answer& answer) {
    // نمنع تكرار نفس ans_id
    for (int i = 0; i < this->answers.size(); i++) {
        if (this->answers[i].getAnswerId() == answer.getAnswerId()) {
            throw ExamException("Duplicate answer id " + to_string(answer.getAnswerId()) +
                                " in question " + to_string(this->number));
        }
    }
    this->answers.push_back(answer);
}

int Question::getAnswerCount() const {
    return this->answers.size();
}

string Question::getSubjectCode() const {
    return this->subjectCode;
}

string Question::getSectionCode() const {
    return this->sectionCode;
}

int Question::getNumber() const {
    return this->number;
}

string Question::getHeader() const {
    return this->header;
}

const Answer& Question::operator[](int index) const {
    if (index < 0 || index >= this->answers.size()) {
        throw ExamException("Invalid answer index: " + to_string(index));
    }
    return this->answers[index];
}

bool Question::operator==(const Question& other) const {
    return this->subjectCode == other.subjectCode && this->number == other.number;
}

set<int> Question::getCorrectIds() const {
    set<int> ids;
    for (int i = 0; i < this->answers.size(); i++) {
        if (this->answers[i].isCorrect()) {
            ids.insert(this->answers[i].getAnswerId());
        }
    }
    return ids;
}

void Question::checkIdsExist(const set<int>& selected) const {
    for (set<int>::const_iterator it = selected.begin(); it != selected.end(); ++it) {
        bool found = false;
        for (int i = 0; i < this->answers.size(); i++) {
            if (this->answers[i].getAnswerId() == *it) {
                found = true;
            }
        }
        if (!found) {
            throw InvalidSelectionException();
        }
    }
}

void Question::printQuestion(int position) const {
    cout << "Q " << position << ")" << endl;
    cout << "    " << this->header << endl << endl;
    for (int i = 0; i < this->answers.size(); i++) {
        this->answers[i].display('a' + i);
    }
}
