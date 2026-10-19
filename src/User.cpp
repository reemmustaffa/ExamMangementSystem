#include <windows.h>
#include <conio.h>
#include "User.h"
#include "ExamException.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

const int SHIFT = 3;

User::User(string username, string firstName, string lastName, string passwordHash) {
    this->username = username;
    this->firstName = firstName;
    this->lastName = lastName;
    this->passwordHash = passwordHash;
}

string User::getUsername() {
    return username;
}

string User::getFirstName() {
    return firstName;
}

string User::getLastName() {
    return lastName;
}

// Caesar cipher
string User::encrypt(string text) {
    for (int i = 0; i < text.size(); i++) {
        char c = text[i];
        if (c >= 'a' && c <= 'z') {
            text[i] = (c - 'a' + SHIFT) % 26 + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            text[i] = (c - 'A' + SHIFT) % 26 + 'A';
        } else if (c >= '0' && c <= '9') {
            text[i] = (c - '0' + SHIFT) % 10 + '0';
        }
    }
    return text;
}

string User::decrypt(string text) {
    for (int i = 0; i < text.size(); i++) {
        char c = text[i];
        if (c >= 'a' && c <= 'z') {
            text[i] = (c - 'a' - SHIFT + 26) % 26 + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            text[i] = (c - 'A' - SHIFT + 26) % 26 + 'A';
        } else if (c >= '0' && c <= '9') {
            text[i] = (c - '0' - SHIFT + 10) % 10 + '0';
        }
    }
    return text;
}

static string readPassword() {
    string pass = "";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            }
        } else {
            pass += ch;
            cout << '*';
        }
    }
    cout << endl;
    return pass;
}

static void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

static int getCursorY() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.dwCursorPosition.Y;
}

static string repeatStr(string s, int n) {
    string result = "";
    for (int i = 0; i < n; i++) {
        result += s;
    }
    return result;
}

static void drawLoginBox(int x, int y) {
    const int W = 40;   // inner width

    gotoxy(x, y);
    cout << "╭" << repeatStr("─", W) << "╮";

    for (int i = 1; i <= 4; i++) {
        gotoxy(x, y + i);
        cout << "│" << string(W, ' ') << "│";
    }

    gotoxy(x, y + 5);
    cout << "╰" << repeatStr("─", W) << "╯";

    gotoxy(x + 5, y + 2);
    cout << "user name:";
    gotoxy(x + 5, y + 3);
    cout << "password:";
}

void User::login() {
    string inputUsername, inputPassword;

    SetConsoleOutputCP(CP_UTF8);       
    cout << string(7, '\n');            
    int y = getCursorY() - 7;

    drawLoginBox(0, y);

    gotoxy(17, y + 2);
    getline(cin, inputUsername);
    gotoxy(17, y + 3);
    inputPassword = readPassword();

    gotoxy(0, y + 7);                   


    ifstream file("data/exam_users.txt");
    if (!file.is_open()) {
        throw FileException("Cannot open file: data/exam_users.txt");
    }

    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string u, f, l, p;
        getline(ss, u, '|');
        getline(ss, f, '|');
        getline(ss, l, '|');
        getline(ss, p, '|');

        if (p.empty()) {
            throw FileException("Malformed record in exam_users.txt: " + line);
        }

        if (u == inputUsername && decrypt(p) == inputPassword) {
            this->username = u;
            this->firstName = f;
            this->lastName = l;
            this->passwordHash = p;
            file.close();
            return;
        }
    }

    file.close();
    throw InvalidCredentialsException();
}

void User::showMenu() {
    cout << "Show menu function\n";
}