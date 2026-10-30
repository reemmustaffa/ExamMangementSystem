#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>
#include <set>
#include "Answer.h"
using namespace std;

// Abstract class: مينفعش نعمل منها object، لازم SingleChoice أو MultipleChoice
class Question {
protected:
    string subjectCode;
    string sectionCode;
    int number;                 // question_no
    string header;              // نص السؤال
    vector<Answer> answers;     // Composition: السؤال هو اللي مالك الإجابات

    set<int> getCorrectIds() const;                      // ids الإجابات الصح
    void checkIdsExist(const set<int>& selected) const;  // كل id لازم يكون موجود في إجابات السؤال
    void printQuestion(int position) const;              // يطبع Q + النص + a) b) c) d)

public:
    Question(string subjectCode, string sectionCode, int number, string header);
    virtual ~Question() = default;

    void addAnswer(const Answer& answer);
    int getAnswerCount() const;

    string getSubjectCode() const;
    string getSectionCode() const;
    int getNumber() const;
    string getHeader() const;

    const Answer& operator[](int index) const;      // الوصول لإجابة بالـ index (يبدأ من 0)
    bool operator==(const Question& other) const;   // نفس المادة ونفس رقم السؤال

    virtual int getType() const = 0;                                      // 1 single, 2 multiple
    virtual void display(int position) const = 0;
    virtual double grade(const set<int>& selected) const = 0;             // 1.0 صح، 0.0 غلط
    virtual void validateSelection(const set<int>& selected) const = 0;   // يرفض الاختيار الغلط
    virtual void validateDefinition() const = 0;                          // يتأكد إن السؤال نفسه سليم
};

#endif
