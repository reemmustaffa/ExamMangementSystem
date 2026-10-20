#ifndef SECTION_H
#define SECTION_H
#include <string>
using namespace std;

class Section{
    private:
    string sub_code;
    string sec_code;
    string sec_name;

    public:
    Section(string sub_code,string sec_code,string sec_name);

    string getSubjectCode();
    string getSectionCode();
    string getSectionName();

    void display();



};

#endif