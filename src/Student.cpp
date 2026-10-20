#include "Student.h"
#include "Exam.h"
#include <iostream>
#include <fstream>
#include <sstream>
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
    Exam exam=it->second;

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
        startExam();
    } else {
        cout << "\nBye!\n";
    }

}


void Student::startExam() {
    cout << "Start exam function\n";
}