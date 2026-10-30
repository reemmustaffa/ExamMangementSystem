#ifndef RUNQUESTION_H
#define RUNQUESTION_H
#include <string>
#include <vector>
using namespace std;

struct RunQuestion {
    int position;            
    string subCode;
    string secCode;
    string questionNo;
    string text;             
    vector<string> ansIds;   
    vector<string> ansTexts; 
    vector<string> isCorrect;  
    vector<bool> selected;    
    bool multiple;
};

#endif