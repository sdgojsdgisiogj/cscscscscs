#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Student {
public:
    int id;
    string name;
    float gpa;

    Student() : id(0), name(""), gpa(0.0) {}

    Student(int id, string name, float gpa) : id(id), name(name), gpa(gpa) {}

    void display() const {
        cout << left << setw(10) << id << setw(20) << name << setw(5) << gpa << endl;
    }
};

class StudentRecord {
private:
    vector<Student> students;
    string filename;

public:
    StudentRecord(string filename) : filename(filename) {
        loadFromFile();
    }

    void addStudent(const Student& student) {
        students.push_back(student);
        saveToFile();
    }

    void deleteStudent(int id) {
        auto it = find_if(students.begin(), students.end(),
                          [id](const Student& s) { return s.id == id; });
        if (it != students.end()) {
            students.erase(it);
            saveToFile();
            cout << "Student with ID " << id << " deleted successfully." << endl;
        } else {
            cout << "Student with ID " << id << " not found." << endl;
        }
    }

    void updateStudent(int id, const string& name, float gpa) {
        for (Student& student : students) {
            if (student.id == id) {
                student.name = name;
                student.gpa = gpa;
                saveToFile();
                cout << "Student with ID " << id << " updated successfully." << endl;
                return;
            }
        }
        cout << "Student with ID " << id << " not found." << endl;
    }

    void searchStudent(int id) const {
        for (const Student& student : students) {
            if (student.id == id) {
                student.display();
                return;
            }
        }
        cout << "Student with ID " << id << " not found." << endl;
    }

    void listAllStudents() const {
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(5) << "GPA" << endl;
        cout << "----------------------------------------" << endl;
        for (const Student& student : students) {
            student.display();
        }
    }

private:
    void loadFromFile() {
        ifstream file(filename);
        if (!file) {
            cout << "File not found. Starting with an empty record." << endl;
            return;
        }
        Student student;
        while (file >> student.id) {
            file.ignore(); // Ignore the whitespace after ID
            getline(file, student.name);
            file >> student.gpa;
            students.push_back(student);
        }
        file.close();
    }

    void saveToFile() const {
        ofstream file(filename);
        if (!file) {
            cout << "Error opening file for writing." << endl;
            return;
        }
        for (const Student& student : students) {
            file << student.id << endl
                 << student.name << endl
                 << student.gpa << endl;
        }
        file.close();
    }
};

void displayMenu() {
    cout << "\n--- Student Record Management System ---" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. Delete Student" << endl;
    cout << "3. Update Student" << endl;
    cout << "4. Search Student" << endl;
    cout << "5. List All Students" << endl;
    cout << "6. Exit" << endl;
    cout << "Select an option: ";
}

int main() {
    StudentRecord record("students.txt");
    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                int id;
                string name;
                float gpa;
                cout << "Enter ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter GPA: ";
                cin >> gpa;
                record.addStudent(Student(id, name, gpa));
                break;
            }
            case 2: {
                int id;
                cout << "Enter Student ID to delete: ";
                cin >> id;
                record.deleteStudent(id);
                break;
            }
            case 3: {
                int id;
                string name;
                float gpa;
                cout << "Enter Student ID to update: ";
                cin >> id;
                cin.ignore();
                cout << "Enter new Name: ";
                getline(cin, name);
                cout << "Enter new GPA: ";
                cin >> gpa;
                record.updateStudent(id, name, gpa);
                break;
            }
            case 4: {
                int id;
                cout << "Enter Student ID to search: ";
                cin >> id;
                record.searchStudent(id);
                break;
            }
            case 5:
                record.listAllStudents();
                break;
            case 6:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 6);

    return 0;
}
