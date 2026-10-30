#include "MultipleChoiceQuestion.h"
#include "ExamException.h"
using namespace std;

MultipleChoiceQuestion::MultipleChoiceQuestion(string subjectCode, string sectionCode, int number, string header)
    : Question(subjectCode, sectionCode, number, header) {
}

int MultipleChoiceQuestion::getType() const {
    return 2;
}

void MultipleChoiceQuestion::display(int position) const {
    this->printQuestion(position);
}

double MultipleChoiceQuestion::grade(const set<int>& selected) const {
    // exact match: لازم نفس مجموعة الإجابات الصح بالظبط (من غير درجات جزئية)
    if (selected == this->getCorrectIds()) {
        return 1.0;
    }
    return 0.0;
}

void MultipleChoiceQuestion::validateSelection(const set<int>& selected) const {
    if (selected.empty()) {
        throw InvalidSelectionException();
    }
    this->checkIdsExist(selected);
}

void MultipleChoiceQuestion::validateDefinition() const {
    // القاعدة 6: multiple choice لازم إجابتين صح على الأقل
    if (this->getCorrectIds().size() < 2) {
        throw ExamException("Multiple-choice question " + to_string(this->number) +
                            " must have at least two correct answers");
    }
}
