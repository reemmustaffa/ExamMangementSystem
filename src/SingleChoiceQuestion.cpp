#include "SingleChoiceQuestion.h"
#include "ExamException.h"
using namespace std;

SingleChoiceQuestion::SingleChoiceQuestion(string subjectCode, string sectionCode, int number, string header)
    : Question(subjectCode, sectionCode, number, header) {
}

int SingleChoiceQuestion::getType() const {
    return 1;
}

void SingleChoiceQuestion::display(int position) const {
    this->printQuestion(position);
}

double SingleChoiceQuestion::grade(const set<int>& selected) const {
    // لازم اختيار واحد بس وهو الصح
    if (selected.size() == 1 && selected == this->getCorrectIds()) {
        return 1.0;
    }
    return 0.0;
}

void SingleChoiceQuestion::validateSelection(const set<int>& selected) const {
    if (selected.size() != 1) {
        throw InvalidSelectionException();   // single choice: اختيار واحد بالظبط
    }
    this->checkIdsExist(selected);
}

void SingleChoiceQuestion::validateDefinition() const {
    // القاعدة 5: single choice لازم إجابة صح واحدة بالظبط
    if (this->getCorrectIds().size() != 1) {
        throw ExamException("Single-choice question " + to_string(this->number) +
                            " must have exactly one correct answer");
    }
}
