#include "Student.h"
#include "Exam.h"
#include "ExamException.h"
#include "ConsoleUI.h"
#include "FileHelper.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <random>
#include <map>
using namespace std;

void Student::generateExam(ExamReservation& res, vector<ExamDetails> details) {
    removeReservationRows("data/exam_running.txt", res.getReservationId());
    removeReservationRows("data/exam_running_ans.txt", res.getReservationId());
    ifstream questionsFile("data/questions.txt");
    ifstream answersFile("data/question_answers.txt");
    if (!questionsFile.is_open()) {
        throw FileException("Could not open questions.txt");
    }
    if (!answersFile.is_open()) {
        throw FileException("Could not open question_answers.txt");
    }
    ofstream runFile("data/exam_running.txt", ios::app);
    ofstream runAnsFile("data/exam_running_ans.txt", ios::app);
    if (!runFile.is_open()) {
        throw FileException("Could not open exam_running.txt");
    }
    if (!runAnsFile.is_open()) {
        throw FileException("Could not open exam_running_ans.txt");
    }

    mt19937 gen(time(0));
    int position = 1;
    string line;

    for (int i = 0; i < details.size(); i++) {
        vector<string> pool;
        questionsFile.clear();
        questionsFile.seekg(0);
        while (getline(questionsFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string subCode, secCode, questionNo;
            getline(ss, subCode, '|');
            getline(ss, secCode, '|');
            getline(ss, questionNo, '|');
            if (subCode == details[i].getSubCode() && secCode == details[i].getSecCode()) {
                pool.push_back(questionNo);
            }
        }

        if (pool.size() < details[i].getNoQues()) {
            throw ExamException("Not enough questions in section " + details[i].getSecCode());
        }

        shuffle(pool.begin(), pool.end(), gen);

        for (int j = 0; j < details[i].getNoQues(); j++) {
            runFile << res.getReservationId() << "|" << position << "|"
                    << details[i].getSubCode() << "|" << pool[j] << "|0" << endl;

            answersFile.clear();
            answersFile.seekg(0);
            while (getline(answersFile, line)) {
                if (line.empty()) continue;
                stringstream ss(line);
                string subCode, questionNo, ansId, ansText, isCorrect;
                getline(ss, subCode, '|');
                getline(ss, questionNo, '|');
                getline(ss, ansId, '|');
                getline(ss, ansText, '|');
                getline(ss, isCorrect, '|');
                if (subCode == details[i].getSubCode() && questionNo == pool[j]) {
                    runAnsFile << res.getReservationId() << "|" << position << "|"
                               << ansId << "|" << isCorrect << "|0" << endl;
                }
            }
            position++;
        }
    }
}

void Student::loadRunning(ExamReservation& res, vector<RunQuestion>& qs) {
    string line;

    ifstream runFile("data/exam_running.txt");
    if (!runFile.is_open()) {
        throw FileException("Could not open exam_running.txt");
    }
    while (getline(runFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string resId, posStr, subCode, questionNo;
        getline(ss, resId, '|');
        getline(ss, posStr, '|');
        getline(ss, subCode, '|');
        getline(ss, questionNo, '|');
        if (resId != res.getReservationId()) continue;

        RunQuestion q;
        q.position = toInt(posStr);
        q.subCode = subCode;
        q.questionNo = questionNo;
        q.multiple = false;
        qs.push_back(q);
    }
    runFile.close();

    ifstream questionsFile("data/questions.txt");
    if (!questionsFile.is_open()) {
        throw FileException("Could not open questions.txt");
    }
    while (getline(questionsFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string subCode, secCode, questionNo, text;
        getline(ss, subCode, '|');
        getline(ss, secCode, '|');
        getline(ss, questionNo, '|');
        getline(ss, text, '|');
        for (int i = 0; i < qs.size(); i++) {
            if (qs[i].subCode == subCode && qs[i].questionNo == questionNo) {
                qs[i].text = text;
                qs[i].secCode = secCode;
            }
        }
    }
    questionsFile.close();

    ifstream answersFile("data/question_answers.txt");
    if (!answersFile.is_open()) {
        throw FileException("Could not open question_answers.txt");
    }
    while (getline(answersFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string subCode, questionNo, ansId, ansText, correct;
        getline(ss, subCode, '|');
        getline(ss, questionNo, '|');
        getline(ss, ansId, '|');
        getline(ss, ansText, '|');
        getline(ss, correct, '|');
        for (int i = 0; i < qs.size(); i++) {
            if (qs[i].subCode == subCode && qs[i].questionNo == questionNo) {
                qs[i].ansIds.push_back(ansId);
                qs[i].ansTexts.push_back(ansText);
                qs[i].isCorrect.push_back(correct);
                qs[i].selected.push_back(false);
            }
        }
    }
    answersFile.close();

    for (int i = 0; i < qs.size(); i++) {
        int count = 0;
        for (int j = 0; j < qs[i].isCorrect.size(); j++) {
            if (qs[i].isCorrect[j] == "1") count++;
        }
        qs[i].multiple = (count > 1);
    }
}

void Student::showQuestion(RunQuestion& q, int total, int remTime) {
    cout << endl;
    cout << "╭" << repeatStr("─", W) << "╮" << endl;
    printLine(" ");
    printLine(string(50, ' ') + "Remaining Time: " + to_string(remTime) + " min");
    printLine(" ");
    printLine(" Q " + to_string(q.position) + ")");
    printWrapped("     ", "     ", q.text);
    printLine(" ");

    for (int i = 0; i < q.ansTexts.size(); i++) {
        char letter = 'a' + i;
        string mark = "";
        if (q.selected[i]) mark = " *";
        printWrapped("   " + string(1, letter) + ") ", "      ", q.ansTexts[i] + mark);
    }

    printLine(" ");
    if (q.multiple) printLine("   (choose all that apply, e.g. a,c)");
    printLine("   your choice : (a,b,c,d) or [n->next and p->previous]");
    printLine(" ");
    cout << "╰" << repeatStr("─", W) << "╯" << endl;
}

void Student::runExam(ExamReservation& res) {
    vector<RunQuestion> qs;
    loadRunning(res, qs);
    if (qs.empty()) {
        throw ExamNotAvailableException("No running questions found.");
    }

    int startMinutes = res.getRemTime();
    if (res.getStatus() == 2) {
        map<string,Exam>::iterator it = exames.find(res.getExamCode());
        if (it != exames.end()) {
            startMinutes = it->second.getExamDuration();
        }
    }
    int startSeconds = startMinutes * 60;
    time_t startTime = time(0);

    int cur = 0;
    if (res.getStatus() == 3 && res.getCurQuestion() >= 1 && res.getCurQuestion() <= qs.size()) {
        cur = res.getCurQuestion() - 1;
    }

    bool finished = false;
    while (true) {
        int left = startSeconds - (int)difftime(time(0), startTime);
        if (left <= 0) {
            cout << "\nTime is up!\n";
            finished = true;
            break;
        }
        int remMinutes = (left + 59) / 60;

        showQuestion(qs[cur], qs.size(), remMinutes);
        cout << "Your choice: ";
        string input;
        getline(cin, input);

        if (input == "n") {
            if (cur < qs.size() - 1) cur++;
        }
        else if (input == "p") {
            if (cur > 0) cur--;
        }
        else if (input == "q") {
            break;
        }
        else {
            vector<int> picks;
            bool valid = true;
            for (int k = 0; k < input.size(); k++) {
                char c = input[k];
                if (c == ',' || c == ' ') continue;
                if (c >= 'a' && c < 'a' + qs[cur].ansIds.size()) {
                    picks.push_back(c - 'a');
                } else {
                    valid = false;
                }
            }
            if (picks.empty()) valid = false;
            if (!qs[cur].multiple && picks.size() != 1) valid = false;

            if (valid) {
                for (int i = 0; i < qs[cur].selected.size(); i++) {
                    qs[cur].selected[i] = false;
                }
                for (int k = 0; k < picks.size(); k++) {
                    qs[cur].selected[picks[k]] = true;
                }

                if (cur < qs.size() - 1) {
                    cur++;
                } else {
                    cout << "This was the last question. Finish the exam? (y/n): ";
                    string confirm;
                    getline(cin, confirm);
                    if (confirm == "y" || confirm == "Y") {
                        cout << "\nExam finished.\n";
                        finished = true;
                        break;
                    }
                }
            } else {
                cout << "Invalid choice, try again." << endl;
            }
        }
    }

    if (finished) {
        map<string,Exam>::iterator it = exames.find(res.getExamCode());
        vector<ExamDetails> details = it->second.getDetails();
        showResult(res, qs, details);
    }
}

void Student::startExam(ExamReservation& res, vector<ExamDetails> details) {
    bool needGenerate = false;

    if (res.getStatus() == 2) {
        needGenerate = true;
    } else {
        vector<RunQuestion> test;
        loadRunning(res, test);
        if (test.empty()) {
            needGenerate = true;
        }
    }

    if (needGenerate) {
        generateExam(res, details);
        cout << "\nExam generated successfully.\n";
    }
    runExam(res);
}