
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class Student {
public:
    int id;
    string name;
    double grade;

    Student(int studentId, string studentName, double studentGrade)
        : id(studentId), name(studentName), grade(studentGrade) {}

    void display() const {
        cout << "\nID: " << id
             << "\nName: " << name
             << "\nGrade: " << fixed << setprecision(2) << grade
             << "\n--------------------\n";
    }
};

const string FILE_NAME = "students.txt";

void saveStudents(const vector<Student>& students) {
    ofstream file(FILE_NAME);

    if (!file) {
        cout << "Error: Could not save student records.\n";
        return;
    }

    for (const Student& student : students) {
        file << student.id << '|'
             << student.name << '|'
             << student.grade << '\n';
    }
}

void loadStudents(vector<Student>& students) {
    ifstream file(FILE_NAME);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string idText, name, gradeText;

        if (!getline(ss, idText, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, gradeText)) {
            continue;
        }

        try {
            size_t idPos, gradePos;
            int id = stoi(idText, &idPos);
            double grade = stod(gradeText, &gradePos);

            if (idPos != idText.size() ||
                gradePos != gradeText.size() ||
                id <= 0 || grade < 0 || grade > 100) {
                continue;
            }

            bool duplicate = false;
            for (const Student& student : students) {
                if (student.id == id) {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate) {
                students.emplace_back(id, name, grade);
            }
        } catch (...) {
            continue;
        }
    }
}

int main() {
    vector<Student> students;
    loadStudents(students);

    int choice;

    cout << "Loaded " << students.size() << " student record(s).\n";

    while (true) {
        cout << "\n===== Student Record Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student by ID\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            int id;
            string name;
            double grade;

            cout << "Enter student ID: ";
            if (!(cin >> id) || id <= 0) {
                cout << "Invalid ID. Enter a positive integer.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            bool exists = false;
            for (const Student& student : students) {
                if (student.id == id) {
                    exists = true;
                    break;
                }
            }

            if (exists) {
                cout << "A student with this ID already exists.\n";
                continue;
            }

            cout << "Enter student name (do not use |): ";
            getline(cin >> ws, name);

            if (name.find('|') != string::npos) {
                cout << "The name cannot contain the | character.\n";
                continue;
            }

            cout << "Enter grade (0-100): ";
            if (!(cin >> grade) || grade < 0 || grade > 100) {
                cout << "Invalid grade. Enter a number from 0 to 100.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            students.emplace_back(id, name, grade);
            saveStudents(students);
            cout << "Student added and saved successfully!\n";
        }
        else if (choice == 2) {
            if (students.empty()) {
                cout << "No student records found.\n";
            } else {
                for (const Student& student : students) {
                    student.display();
                }
            }
        }
        else if (choice == 3) {
            int id;
            cout << "Enter student ID to search: ";

            if (!(cin >> id)) {
                cout << "Invalid ID.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            bool found = false;
            for (const Student& student : students) {
                if (student.id == id) {
                    student.display();
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Student not found.\n";
            }
        }
        else if (choice == 4) {
            int id;
            cout << "Enter student ID to update: ";

            if (!(cin >> id)) {
                cout << "Invalid ID.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            bool found = false;
            for (Student& student : students) {
                if (student.id == id) {
                    string name;
                    double grade;

                    cout << "Enter new name (do not use |): ";
                    getline(cin >> ws, name);

                    if (name.find('|') != string::npos) {
                        cout << "The name cannot contain the | character.\n";
                        found = true;
                        break;
                    }

                    cout << "Enter new grade (0-100): ";
                    if (!(cin >> grade) || grade < 0 || grade > 100) {
                        cout << "Invalid grade. Update cancelled.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        found = true;
                        break;
                    }

                    student.name = name;
                    student.grade = grade;
                    saveStudents(students);
                    cout << "Student updated and saved successfully!\n";
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Student not found.\n";
            }
        }
        else if (choice == 5) {
            int id;
            cout << "Enter student ID to delete: ";

            if (!(cin >> id)) {
                cout << "Invalid ID.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            bool found = false;
            for (auto it = students.begin(); it != students.end(); ++it) {
                if (it->id == id) {
                    students.erase(it);
                    saveStudents(students);
                    cout << "Student deleted and records saved!\n";
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Student not found.\n";
            }
        }
        else if (choice == 6) {
            saveStudents(students);
            cout << "Records saved. Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Please select 1-6.\n";
        }
    }

    return 0;
}
