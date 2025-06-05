#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

class Student {
private:
    int id;
    string name;
    string department;
    float gpa;

public:
    Student() : id(0), name(""), department(""), gpa(0.0f) {}

    // Input student data with basic validation
    void input() {
        cout << "Enter ID: ";
        while (!(cin >> id)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid ID. Enter again: ";
        }

        cin.ignore(); // To consume leftover newline
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Department: ";
        getline(cin, department);
        cout << "Enter GPA (0.0 - 4.0): ";
        while (!(cin >> gpa) || gpa < 0.0 || gpa > 4.0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid GPA. Enter again: ";
        }
    }

    // Display student data
    void display() const {
        cout << left << setw(10) << id
             << setw(20) << name
             << setw(15) << department
             << fixed << setprecision(2) << setw(5) << gpa << endl;
    }

    // Getters
    int getID() const { return id; }
    string getName() const { return name; }
    string getDepartment() const { return department; }
    float getGPA() const { return gpa; }
};

class StudentDatabase {
private:
    vector<Student> students;

public:
    void addStudent() {
        Student s;
        s.input();
        students.push_back(s);
        cout << "Student added successfully.\n";
    }

    void displayAll() const {
        if (students.empty()) {
            cout << "No students in the database.\n";
            return;
        }
        cout << left << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(15) << "Department"
             << setw(5) << "GPA" << endl;
        cout << "--------------------------------------------------------\n";
        for (const auto& s : students) {
            s.display();
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            cerr << "Error opening file for writing.\n";
            return;
        }
        for (const auto& s : students) {
            file << s.getID() << ','
                 << s.getName() << ','
                 << s.getDepartment() << ','
                 << s.getGPA() << '\n';
        }
        file.close();
        cout << "Data saved to " << filename << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening file for reading.\n";
            return;
        }

        students.clear();
        string line;
        while (getline(file, line)) {
            Student s;
            size_t pos;

            try {
                pos = line.find(',');
                int id = stoi(line.substr(0, pos));
                line.erase(0, pos + 1);

                pos = line.find(',');
                string name = line.substr(0, pos);
                line.erase(0, pos + 1);

                pos = line.find(',');
                string dept = line.substr(0, pos);
                line.erase(0, pos + 1);

                float gpa = stof(line);

                // Manually set private members through constructor
                Student temp;
                temp = Student(); // default constructor
                students.push_back({id, name, dept, gpa}); // use constructor or factory if available

            } catch (...) {
                cerr << "Error parsing line. Skipping.\n";
            }
        }

        file.close();
        cout << "Data loaded from " << filename << endl;
    }

    void searchByID(int searchID) const {
        for (const auto& s : students) {
            if (s.getID() == searchID) {
                cout << "Student found:\n";
                s.display();
                return;
            }
        }
        cout << "Student with ID " << searchID << " not found.\n";
    }

    void deleteByID(int deleteID) {
        auto it = remove_if(students.begin(), students.end(), [deleteID](const Student& s) {
            return s.getID() == deleteID;
        });

        if (it != students.end()) {
            students.erase(it, students.end());
            cout << "Student deleted.\n";
        } else {
            cout << "Student with ID " << deleteID << " not found.\n";
        }
    }

    void sortByGPA() {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.getGPA() > b.getGPA();
        });
        cout << "Students sorted by GPA in descending order.\n";
    }
};

void displayMenu() {
    cout << "\n------ Student Management System ------\n";
    cout << "1. Add Student\n";
    cout << "2. Display All Students\n";
    cout << "3. Save to File\n";
    cout << "4. Load from File\n";
    cout << "5. Search by ID\n";
    cout << "6. Delete by ID\n";
    cout << "7. Sort by GPA\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    StudentDatabase db;
    int choice;
    string filename = "students.txt";

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                db.addStudent();
                break;
            case 2:
                db.displayAll();
                break;
            case 3:
                db.saveToFile(filename);
                break;
            case 4:
                db.loadFromFile(filename);
                break;
            case 5: {
                int id;
                cout << "Enter ID to search: ";
                cin >> id;
                db.searchByID(id);
                break;
            }
            case 6: {
                int id;
                cout << "Enter ID to delete: ";
                cin >> id;
                db.deleteByID(id);
                break;
            }
            case 7:
                db.sortByGPA();
                break;
            case 8:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 8);

    return 0;
}
