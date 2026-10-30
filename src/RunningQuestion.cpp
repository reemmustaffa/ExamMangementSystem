#include "RunningQuestion.h"
#include "ExamException.h"
using namespace std;

RunningQuestion::RunningQuestion(string reservationId, int position, const Question* question, int score) {
    if (question == nullptr) {
        throw ExamException("Running question has no question reference");
    }
    this->reservationId = reservationId;
    this->position = position;
    this->question = question;
    this->score = score;

    // نعمل RunningAnswer لكل إجابة في السؤال (كلهم unselected في الأول)
    for (int i = 0; i < question->getAnswerCount(); i++) {
        const Answer& a = (*question)[i];
        this->answers.push_back(RunningAnswer(reservationId, position, a.getAnswerId(), a.isCorrect(), false));
    }
}

string RunningQuestion::getReservationId() const {
    return this->reservationId;
}

int RunningQuestion::getPosition() const {
    return this->position;
}

const Question* RunningQuestion::getQuestion() const {
    return this->question;
}

int RunningQuestion::getScore() const {
    return this->score;
}

const vector<RunningAnswer>& RunningQuestion::getAnswers() const {
    return this->answers;
}

set<int> RunningQuestion::getSelectedIds() const {
    set<int> ids;
    for (int i = 0; i < this->answers.size(); i++) {
        if (this->answers[i].isSelected()) {
            ids.insert(this->answers[i].getAnswerId());
        }
    }
    return ids;
}

bool RunningQuestion::isAnswered() const {
    return !this->getSelectedIds().empty();
}

void RunningQuestion::setSelectedAnswers(const set<int>& ids) {
    if (!ids.empty()) {
        this->question->validateSelection(ids);   // runtime polymorphism: single أو multiple
    }
    for (int i = 0; i < this->answers.size(); i++) {
        bool isChosen = ids.count(this->answers[i].getAnswerId()) > 0;
        this->answers[i].setSelected(isChosen);
    }
}

void RunningQuestion::restoreAnswer(int answerId, bool selected) {
    for (int i = 0; i < this->answers.size(); i++) {
        if (this->answers[i].getAnswerId() == answerId) {
            this->answers[i].setSelected(selected);
            return;
        }
    }
    throw FileException("Unknown answer id in exam_running_ans.txt: " + to_string(answerId));
}

int RunningQuestion::calculateScore() {
    this->score = (int)this->question->grade(this->getSelectedIds());
    return this->score;
}

string RunningQuestion::toFileLine() const {
    return this->reservationId + "|" + to_string(this->position) + "|" +
           this->question->getSubjectCode() + "|" + to_string(this->question->getNumber()) + "|" +
           to_string(this->score);
}
