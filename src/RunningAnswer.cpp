#include "RunningAnswer.h"
using namespace std;

RunningAnswer::RunningAnswer(string reservationId, int position, int answerId, bool correct, bool selected) {
    this->reservationId = reservationId;
    this->position = position;
    this->answerId = answerId;
    this->correct = correct;
    this->selected = selected;
}

string RunningAnswer::getReservationId() const {
    return this->reservationId;
}

int RunningAnswer::getPosition() const {
    return this->position;
}

int RunningAnswer::getAnswerId() const {
    return this->answerId;
}

bool RunningAnswer::isCorrect() const {
    return this->correct;
}

bool RunningAnswer::isSelected() const {
    return this->selected;
}

void RunningAnswer::setSelected(bool selected) {
    this->selected = selected;
}

string RunningAnswer::toFileLine() const {
    return this->reservationId + "|" + to_string(this->position) + "|" + to_string(this->answerId) + "|" +
           (this->correct ? "1" : "0") + "|" + (this->selected ? "1" : "0");
}
