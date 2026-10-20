#include <iostream>
using namespace std;

#include "Student.h"
#include "ExamException.h"

int main() {
    Student student("", "", "", "");

    try {
        student.login();
        cout << "\nLogin successful. Welcome " << student.getFirstName() << "!\n";
         student.viewReservations();
         student.selectExam();
    }
    catch (InvalidCredentialsException& e) {
        cout << "\n" << e.what() << "\n";
    }
    catch (FileException& e) {
        cout << "\nFile error: " << e.what() << "\n";
    }
    catch (ExamNotAvailableException& e) {
        cout << "\n" << e.what() << "\n";
    }
    catch (InvalidSelectionException& e) {
        cout << "\n" << e.what() << "\n";
    }
    catch (exception& e) {
        cout << "\nError: " << e.what() << "\n";
    }
    return 0;
}