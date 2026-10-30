#ifndef ANSWER_H
#define ANSWER_H

#include <string>
using namespace std;

class Answer {
private:
    int answerId;
    string text;
    bool correct;

public:
    Answer(int answerId, string text, bool correct);

    int getAnswerId() const;
    string getText() const;
    bool isCorrect() const;

    void display(char label) const;   // يطبع: a) text
};

#endif
