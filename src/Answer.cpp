#include "Answer.h"
#include <iostream>
using namespace std;

Answer::Answer(int answerId, string text, bool correct) {
    this->answerId = answerId;
    this->text = text;
    this->correct = correct;
}

int Answer::getAnswerId() const {
    return this->answerId;
}

string Answer::getText() const {
    return this->text;
}

bool Answer::isCorrect() const {
    return this->correct;
}

void Answer::display(char label) const {
    cout << label << ") " << this->text << endl;
}
