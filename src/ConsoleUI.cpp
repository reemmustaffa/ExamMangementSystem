#include "ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

string repeatStr(string s, int n){
    string res = "";
    for(int i = 0; i < n; i++){
        res += s;
    }
    return res;
}

void printLine(string text){
    if(text.size() > W) text = text.substr(0, W);
    cout << "|" << left << setw(W) << text << "|" << endl;
}

void printInfo(string label, string value){
    stringstream ss;
    ss << " " << left << setw(15) << label << value;
    printLine(ss.str());
}

void printWrapped(string firstPrefix, string nextPrefix, string text){
    string prefix = firstPrefix;
    int width = W - prefix.size();
    while ((int)text.size() > width) {
        size_t cut = text.rfind(' ', width);
        if (cut == string::npos || cut == 0) {
            cut = width;
        }
        printLine(prefix + text.substr(0, cut));
        text = text.substr(cut);
        while (!text.empty() && text[0] == ' ') {
            text.erase(0, 1);
        }
        prefix = nextPrefix;
        width = W - prefix.size();
    }
    printLine(prefix + text);
}