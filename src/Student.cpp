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
void Student::viewReservations() {
    map<string, Exam> exames;
    map<int,string>statutes;
    reservations.clear();
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

void Student::startExam() {
    cout << "Start exam function\n";
}