#ifndef COURSEFUNCTIONS_H
#define COURSEFUNCTIONS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

class Courses { // store information about each course
public:
    string getCourseNumber() {
        return courseNumber;
    }

    void setCourseNumber(string number) {
        courseNumber = number;
    }

    string getCourseName() {
        return courseName;
    }

    void setCourseName(string name) {
        courseName = name;
    }

    int getTotalPrerequisites() {
        return totalPrerequisites;
    }

    void setTotalPrerequisites(int prereq) {
        totalPrerequisites = prereq;
    }

    string getFirstPrereq() {
        return firstPrereq;
    }

    void setFirstPrereq(string prereq) {
        firstPrereq = prereq;
    }
    string getSecondPrereq() {
        return secondPrereq;
    }
    void setSecondPrereq(string prereq) {
        secondPrereq = prereq;
    }

    void printPrerequisites() {
        if (totalPrerequisites == 0) { // in course has no prerequisites
            cout << "None";
        }
        else { // print prerequisite course number
            for (int i = 0; i < totalPrerequisites; ++i) {
                if (i == 0) { // first prerequisite
                    cout << firstPrereq;
                }
                if (i == 1) { // second prerequisite
                    cout << " , " << secondPrereq;
                }
            }
            cout << endl;
        }
    }


private:
    string courseNumber = "null";
    string courseName = "null";
    int totalPrerequisites = 0;
    string firstPrereq = "null";
    string secondPrereq = "null";
};

struct Node { // defines node for binary search tree
    Courses course;
    Node* left;
    Node* right;

    Node() { // new nodes start with null left and right pointers
        left = nullptr;
        right = nullptr;
    }

    Node(Courses newCourse) : // construct new node
        Node() {
        course = newCourse;
    }
};

class BinarySearchTree {
    // class to store list of courses in bst
private:
    Node* root = nullptr; // initially a null root

    void addNode(Node* node, Courses newCourseInfo) { // add a new node
        if (node != nullptr && node->course.getCourseNumber().compare(newCourseInfo.getCourseNumber()) > 0) { // compare to left
            if (node->left == nullptr) { // if left node pointer is empty
                node->left = new Node(newCourseInfo); // new node becomes left
                return;
            }
            else { 
                this->addNode(node->left, newCourseInfo); // if left is not empty recursively search left node
            }
        }
        else if (node != nullptr && node->course.getCourseNumber().compare(newCourseInfo.getCourseNumber()) < 0) { // compare to right
            if (node->right == nullptr) { // if right node is empty
                node->right = new Node(newCourseInfo); // new node becomes right
                return;
            }
            else { // recursivley search next node
                this->addNode(node->right, newCourseInfo);
            }
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left); // check for left most leaf recursivley
            cout << node->course.getCourseNumber() << " : " << node->course.getCourseName() << endl;
            cout << "    Prerequisites: "; // print course information
            node->course.printPrerequisites();
            cout << endl;
            inOrder(node->right); // check right right
        }

    }

public:
    void InOrder() { // calls in order by starting with the root node
        inOrder(root);
        cout << endl;
    }

    void Insert(Courses newCourseInfo) { // called to insert new course
        if (root == nullptr) { // checks if bst is empty
            root = new Node(newCourseInfo); // becomes root if empty
        }
        else { // if bst is not empty calls add node function
            this->addNode(root, newCourseInfo); // starts with the root node
        }

    }

    Courses Search(string searchCourseNumber) { // called to search for specific course by course number
        Node* current = root; // start at root 
        while (current != nullptr) { // until find a leaf
            if (current->course.getCourseNumber().compare(searchCourseNumber) == 0) {
                return current->course; // course found
            }
            else if (searchCourseNumber.compare(current->course.getCourseNumber()) < 0) {
                current = current->left; // if less then current node search left
            }
            else { // if more then current node search right
                current = current->right;
            }
        }
        Courses course; // if item not found return empty course
        return course;
    }
};

void PrintMenu() { // prints menu options
    cout << "1 - Load Data" << endl;
    cout << "2 - Print Course List" << endl;
    cout << "3 - Print Course Information" << endl;
    cout << "4 - Exit" << endl;
}

int UserInput() { // gets input from user
    int input;

    cin >> input;

    if (cin.fail()) { // validate input
        throw invalid_argument("Menu input must be a number");
    }
    return input;
}

Courses ParseLine(string line) { // parses each line in file
    int commaLocation = 0;
    int prereqCounter = 0;
    string courseInfo; // string to store parsed section of line
    Courses newCourse; // new course to be added to tree

    // parses line at ','
    // first section should be course number
    commaLocation = line.find(','); 
    courseInfo = line.substr(0, commaLocation);
    newCourse.setCourseNumber(courseInfo);
    line.erase(0, commaLocation + 1);

    // second section should be course name
    commaLocation = line.find(',');
    courseInfo = line.substr(0, commaLocation);
    newCourse.setCourseName(courseInfo);
    line.erase(0, commaLocation + 1);

    // check for prerequisites
    commaLocation = line.find(',');
    if (commaLocation == -1) { // if no more commas are found
        if (line.size() < 8) { // if next section is less then coursenumber length
            newCourse.setFirstPrereq(line); // only one prereq
            ++prereqCounter;
        }
    }
    if (commaLocation != 0 && commaLocation != -1) { // if one more comma exists in line
        ++prereqCounter;
        courseInfo = line.substr(0, commaLocation);
        newCourse.setFirstPrereq(courseInfo); // before comma is first prereq
        line.erase(0, commaLocation + 1);
        if (line.size() > 0) { // next section is second prereq
            newCourse.setSecondPrereq(line);
            ++prereqCounter;
        }
    }

    newCourse.setTotalPrerequisites(prereqCounter);

    return newCourse;
}

void ReadCoursesFile(string file, BinarySearchTree* bst) {
    ifstream courseFS;
    string courseLine;
    Courses addCourse;

    courseFS.open(file); // open file
    courseFS.ignore();

    if (!courseFS.is_open()) { // check that file opened
        cout << "Could not open file" << endl;
        return;
    }
    courseFS.ignore(); // clear stream
    courseFS.ignore();
    courseFS.clear();
    
    getline(courseFS, courseLine); // get first line in file

    while (!courseFS.fail()) { // until end of file
        addCourse = ParseLine(courseLine); // pass line to parser
        bst->Insert(addCourse); // insert info from line in bst
    
        courseFS.clear(); // clear stream

        getline(courseFS, courseLine); // get next line
    }

}
#endif
