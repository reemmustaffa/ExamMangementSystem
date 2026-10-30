#ifndef CONSOLEUI_H
#define CONSOLEUI_H
#include <string>
using namespace std;

const int W = 80;

string repeatStr(string s, int n);
void printLine(string text);
void printInfo(string label, string value);
void printWrapped(string firstPrefix, string nextPrefix, string text);

#endif