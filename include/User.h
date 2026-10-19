#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

class User{
    private:
    string username;
    string firstName;
    string lastName;
    string passwordHash;

    public:
    User(string username, string firstName, string lastName, string passwordHash);

    void login();
    void showMenu();

    string getUsername();
    string getFirstName();
    string getLastName();

    static string encrypt(string text);
    static string decrypt(string text);
};
#endif