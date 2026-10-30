#include "Student.h"
#include "Exam.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <random>
#include "ExamException.h"
#include <iomanip>
#include <map>
using namespace std;

const int W = 80;

Student::Student(string username, string firstName, string lastName, string passwordHash) : User(username, firstName, lastName, passwordHash) {
}

static int toInt(string s){
    if(s.empty()) return 0;
    try{
        return stoi(s);
    }
    catch(exception& e){
    throw FileException("Invalid number in data file: " + s);
}
}

static string repeatStr(string s,int n){
    string res="";
    for(int i=0;i<n;i++){
        res+=s;
    }
    return res;
}

static void printLine(string text){
    if(text.size()>W) text=text.substr(0,W);
    cout<<"|"<<left<<setw(W)<<text<<"|"<<endl;
}

static void printInfo(string label,string value){
    stringstream ss;
    ss<<" "<<left<<setw(15)<<label<<value;
    printLine(ss.str());
}

static void printWrapped(string firstPrefix, string nextPrefix, string text){
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

static bool isAnswerCorrect(RunQuestion& q){
    for(int i = 0; i < q.selected.size(); i++){
        bool shouldBeSelected = (q.isCorrect[i] == "1");
        if(q.selected[i] != shouldBeSelected) return false;
    }
    return true;
}

static void removeReservationRows(string fileName, string reservationId){
    vector<string> keep;
    string line;

    ifstream in(fileName);
    if(in.is_open()){
        while(getline(in, line)){
            if(line.empty()) continue;
            stringstream ss(line);
            string resId;
            getline(ss, resId, '|');
            if(resId != reservationId){
                keep.push_back(line);
            }
        }
        in.close();
    }

    ofstream out(fileName);
    for(int i = 0; i < keep.size(); i++){
        out << keep[i] << endl;
    }
}

void Student::viewReservations() {
    map<int,string>statutes;
    reservations.clear();
    exames.clear();
    string line;

    ifstream examesfile("data/exams.txt");
    if(!examesfile.is_open()){
        throw FileException("Could not open exams.txt");
    }
    while(getline(examesfile,line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string examCode, examName, examDurationStr, maxScoreStr, passScoreStr;
        getline(ss, examCode, '|');
        getline(ss, examName, '|'); 
        getline(ss, examDurationStr, '|');
        getline(ss, maxScoreStr, '|');
        getline(ss, passScoreStr, '|');
        if(passScoreStr.empty() || maxScoreStr.empty() || examDurationStr.empty() || examName.empty() || examCode.empty()){
            throw FileException("Malformed record in exams.txt: " + line);
        }
        exames.insert(make_pair(examCode, Exam(examCode, examName, toInt(examDurationStr), toInt(maxScoreStr), toInt(passScoreStr))));
    }
    examesfile.close();

    ifstream statutesfile("data/reservation_status.txt");
    if(!statutesfile.is_open()){
        throw FileException("Could not open reservation_status.txt");
    }
    while(getline(statutesfile,line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string statusCodeStr, statusName;
        getline(ss, statusCodeStr, '|');
        getline(ss, statusName, '|');
        if(statusCodeStr.empty() || statusName.empty()){
            throw FileException("Malformed record in reservation_status.txt: " + line);
        }
        statutes[toInt(statusCodeStr)] = statusName;
    }
    statutesfile.close();

    ifstream reservationsfile("data/exam_reservations.txt");
    if(!reservationsfile.is_open()){
        throw FileException("Could not open exam_reservations.txt");
    }
    while(getline(reservationsfile,line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string reservationId, username, examDate, examCode, cur_questionStr, scoreStr, statusStr, rem_timeStr, pass_scoreStr;
        getline(ss, reservationId, '|');
        getline(ss, username, '|');
        getline(ss, examDate, '|');
        getline(ss, examCode, '|');
        getline(ss, cur_questionStr, '|');
        getline(ss, scoreStr, '|');
        getline(ss, statusStr, '|');
        getline(ss, rem_timeStr, '|');
        getline(ss, pass_scoreStr, '|');

        if(username != getUsername()) continue;

        if(examCode.empty()){
            throw FileException("Malformed record in reservations.txt: " + line);
        }

        reservations.push_back(ExamReservation(reservationId, username, examDate, examCode,
            toInt(cur_questionStr), toInt(scoreStr), toInt(statusStr), toInt(rem_timeStr), toInt(pass_scoreStr)));
    }
    reservationsfile.close();

    // Display reservations
    cout<<endl;
    cout << "╭" << repeatStr("─", W) << "╮" << endl;
    printLine(" ");
    printLine(" user : "+ getFirstName() +" " + getLastName());
    printLine(" ");
    printLine("  Reserved Exams :");

    if(reservations.empty()){
        printLine("  No reservations found.");
    }else{
        stringstream header;
       header << "  " << left << setw(5) << "" << setw(45) << "exam Name" << setw(12) << "Date" << setw(10) << "Status" << "Score";
        printLine(header.str());

        for(int i=0;i<reservations.size();i++){
            map<string,Exam>::iterator it=exames.find(reservations[i].getExamCode());
           if (it == exames.end()) {
                throw ExamException("Unknown exam code: " + reservations[i].getExamCode());
            }

            string score;
            if(reservations[i].getStatus()==4){
                score=to_string(reservations[i].getScore());;
            }
             stringstream row;
            row << "  " << left << setw(5) << (i + 1)
                << setw(45) << it->second.getExamName()
                << setw(12) << reservations[i].getExamDate()
                << setw(10) << statutes[reservations[i].getStatus()]
                << score;
            printLine(row.str());
        }
    }
     printLine("");
    cout << "╰" << repeatStr("─", W) << "╯" << endl;
}

bool Student::showExamDetails(string examCode){
    map<string,Exam>::iterator it=exames.find(examCode);
    if (it == exames.end()) {
       throw ExamException("Unknown exam code: " + examCode);
    }
    Exam& exam=it->second;

    map<string,string> subjectNames;
    map<string,string>sectionNames;

    string line;

    ifstream subjectsfile("data/subjects.txt");
    if(!subjectsfile.is_open()){
        throw FileException("Could not open subjects.txt");
    }
    while(getline(subjectsfile,line)){
         if (line.empty()) continue;
        stringstream ss(line);
        string sub_code, sub_name, sub_hoursStr;
        getline(ss, sub_code, '|');
        getline(ss, sub_name, '|');
        if(sub_code.empty() || sub_name.empty()){
            throw FileException("Malformed record in subjects.txt: " + line);
        }
        subjectNames[sub_code] = sub_name;
    }
    subjectsfile.close();

    ifstream sectionsfile("data/sections.txt");
    if(!sectionsfile.is_open()){
        throw FileException("Could not open sections.txt");
    }
    while(getline(sectionsfile,line)){
        if (line.empty()) continue;
        stringstream ss(line);
        string sub_code, sec_code, sec_name;
        getline(ss, sub_code, '|');
        getline(ss, sec_code, '|');
        getline(ss, sec_name, '|');
        if(sub_code.empty() || sec_code.empty() || sec_name.empty()){
            throw FileException("Malformed record in sections.txt: " + line);
        }
        sectionNames[sub_code + "|" + sec_code] = sec_name;
    }
    sectionsfile.close();

    ifstream examDetailsfile("data/exam_details.txt");
    if(!examDetailsfile.is_open()){
        throw FileException("Could not open exam_details.txt");
    }
    while(getline(examDetailsfile,line)){
        if (line.empty()) continue;
        stringstream ss(line);
        string exam_code, sub_code, sec_codeStr, no_quesStr;
        getline(ss, exam_code, '|');
        getline(ss, sub_code, '|');
        getline(ss, sec_codeStr, '|');
        getline(ss, no_quesStr, '|');

        if(exam_code.empty() || sub_code.empty() || sec_codeStr.empty() || no_quesStr.empty()){
            throw FileException("Malformed record in exam_details.txt: " + line);
        }
        if (exam_code != examCode) continue; 
        exam.addDetail(ExamDetails(exam_code, sub_code, sec_codeStr, toInt(no_quesStr)));
    }
    examDetailsfile.close();

    vector<ExamDetails> details = exam.getDetails();
    if(details.empty()){
        throw ExamNotAvailableException("No details found for this exam.");
    }


    // Display exam details
     cout << endl;
    cout << "╭" << repeatStr("─", W) << "╮" << endl;
    printLine(" ");
    printInfo("Exam Code:", exam.getExamCode());
    printInfo("Exam Name:", exam.getExamName());
    printInfo("Duration:", to_string(exam.getExamDuration()) + " minutes");
    printInfo("Max Score:", to_string(exam.getMaxScore()));
    printInfo("Pass Score:", to_string(exam.getPassScore()));

    for (int i = 0; i < details.size(); i++) {
        map<string, string>::iterator s = subjectNames.find(details[i].getSubCode());
        if (s == subjectNames.end()) {
            throw ExamException("Unknown subject code: " + details[i].getSubCode());
        }
        map<string, string>::iterator sec = sectionNames.find(details[i].getSubCode() + "|" + details[i].getSecCode());
        if (sec == sectionNames.end()) {
            throw ExamException("Unknown section code: " + details[i].getSecCode());
        }
        stringstream row;
        row << "  " << left << setw(20) << s->second << setw(25) << sec->second << details[i].getNoQues();
        printLine(row.str());
    }
    printLine("");
    stringstream total;
    total << "  " << left << setw(45) << "Total Number of Questions:" << exam.getTotalQuestions();
    printLine(total.str());
    printLine("");
    printLine("  if you ready to start press 1 or q to quit :");
    printLine("");
    cout << "╰" << repeatStr("─", W) << "╯" << endl;


    cout << "Your choice: ";
    string input;
    getline(cin, input);
    if (input == "1") return true;
    if (input == "q" || input == "Q") return false;
    throw InvalidSelectionException();

}

void Student::selectExam(){
    if(reservations.empty()){
        throw ExamNotAvailableException("No exams available to select.");
    }
    string input;
    cout << "Select an exam to start (enter the number): ";
    getline(cin, input);
     int choice;
    try{
       choice=stoi(input);
    }
     catch (exception& e) {
        throw InvalidSelectionException();     
    }
     if(choice<1 || choice > reservations.size()){
            throw InvalidSelectionException();
    }
    ExamReservation res=reservations[choice-1];
    int status=res.getStatus();

      if (status == 1) {
        throw ExamNotAvailableException("This exam is reserved but not available yet.");
    }
    if (status == 4) {
        throw ExamNotAvailableException("This exam is finished and cannot be reopened.");
    }
    if(status == 3) {
        cout << "\nResuming your exam from question " << res.getCurQuestion() << "...\n";
    }
    if(status == 2) {
        cout << "\nStarting your exam...\n";
    }
       bool ready = showExamDetails(res.getExamCode());
    if (ready) {
    map<string,Exam>::iterator it = exames.find(res.getExamCode());
    vector<ExamDetails> details = it->second.getDetails();  
    startExam(res, details);
    } else {
        cout << "\nBye!\n";
    }

}

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