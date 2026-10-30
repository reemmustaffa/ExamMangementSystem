#include "Student.h"
#include "Exam.h"
#include "ExamException.h"
#include "ConsoleUI.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
using namespace std;

static bool isAnswerCorrect(RunQuestion& q){
    for(int i = 0; i < q.selected.size(); i++){
        bool shouldBeSelected = (q.isCorrect[i] == "1");
        if(q.selected[i] != shouldBeSelected) return false;
    }
    return true;
}

void Student::showResult(ExamReservation& res, vector<RunQuestion>& qs, vector<ExamDetails>& details) {
    map<string,Exam>::iterator it = exames.find(res.getExamCode());
    if (it == exames.end()) {
        throw ExamException("Unknown exam code: " + res.getExamCode());
    }
    int maxScore = it->second.getMaxScore();
    int passScore = it->second.getPassScore();

    map<string,string> subjectNames;
    map<string,string> sectionNames;
    string line;

    ifstream subjectsfile("data/subjects.txt");
    if (!subjectsfile.is_open()) {
        throw FileException("Could not open subjects.txt");
    }
    while (getline(subjectsfile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string subCode, subName;
        getline(ss, subCode, '|');
        getline(ss, subName, '|');
        subjectNames[subCode] = subName;
    }
    subjectsfile.close();

    ifstream sectionsfile("data/sections.txt");
    if (!sectionsfile.is_open()) {
        throw FileException("Could not open sections.txt");
    }
    while (getline(sectionsfile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string subCode, secCode, secName;
        getline(ss, subCode, '|');
        getline(ss, secCode, '|');
        getline(ss, secName, '|');
        sectionNames[subCode + "|" + secCode] = secName;
    }
    sectionsfile.close();

    vector<string> rows;
    int totalQ = 0;
    int totalCorrect = 0;

    for (int i = 0; i < details.size(); i++) {
        int count = 0;
        int correct = 0;
        for (int j = 0; j < qs.size(); j++) {
            if (qs[j].subCode == details[i].getSubCode() && qs[j].secCode == details[i].getSecCode()) {
                count++;
                if (isAnswerCorrect(qs[j])) correct++;
            }
        }
        int percent = 0;
        if (count > 0) percent = correct * 100 / count;

        totalQ += count;
        totalCorrect += correct;

        stringstream row;
        row << "  " << left << setw(20) << subjectNames[details[i].getSubCode()]
            << setw(22) << sectionNames[details[i].getSubCode() + "|" + details[i].getSecCode()]
            << setw(14) << count
            << setw(10) << correct
            << percent << "%";
        rows.push_back(row.str());
    }

    int overallPercent = 0;
    if (totalQ > 0) overallPercent = totalCorrect * 100 / totalQ;
    int score = overallPercent * maxScore / 100;
    bool passed = (score >= passScore);

    cout << endl;
    cout << "╭" << repeatStr("─", W) << "╮" << endl;
    printLine(" ");
    if (passed) {
        printLine("  Congratulation you do a good job");
    } else {
        printLine("  Sorry, you did not pass. Better luck next time");
    }
    printLine(" ");

    stringstream header;
    header << "  " << left << setw(20) << "Subject" << setw(22) << "section"
           << setw(14) << "no_questions" << setw(10) << "correct" << "%";
    printLine(header.str());

    for (int i = 0; i < rows.size(); i++) {
        printLine(rows[i]);
    }

    stringstream total;
    total << "  " << setw(66) << "" << setw(7) << (to_string(overallPercent) + "%") << (passed ? "Pass" : "Fail");
    printLine(total.str());
    printLine(" ");
    printLine("  Pass Score:  " + repeatStr("*", passScore / 50) + "  " + to_string(passScore));
    printLine("  Your Score:  " + repeatStr("*", score / 50) + "  " + to_string(score));
    printLine(" ");
    cout << "╰" << repeatStr("─", W) << "╯" << endl;
}