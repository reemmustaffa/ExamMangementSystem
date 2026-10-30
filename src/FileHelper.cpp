#include "FileHelper.h"
#include "ExamException.h"
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int toInt(string s){
    if(s.empty()) return 0;
    try{
        return stoi(s);
    }
    catch(exception& e){
        throw FileException("Invalid number in data file: " + s);
    }
}

void removeReservationRows(string fileName, string reservationId){
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