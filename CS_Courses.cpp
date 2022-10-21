#include <iostream>
#include <string>
#include "CourseFunctions.h"

using namespace std;

int main() {
	BinarySearchTree* bst;
	bst = new BinarySearchTree;
	int inputValue = 0;
	string search;
	Courses course;

	while (inputValue != 4) {
		PrintMenu(); // call print menu function
		
		try { // catches invalid inputs
			inputValue = UserInput();
		}
		catch (invalid_argument& excpt) { //catches exceptions for invalid input
			cout << excpt.what() << endl;

			cin.clear();
			cin.ignore();

			continue;
		}

		if (inputValue == 1) { // load file
			cout << "Opening File" << endl;
			ReadCoursesFile("ABCU_Advising_Program_Input.txt", bst);
		}
		else if (inputValue == 2) { // print in alpha numerical order
			bst->InOrder();
		}
		else if (inputValue == 3) { // search for a specific course
			cout << "Enter Course Number" << endl;
			cin >> search;
			course = bst->Search(search);

			if (course.getCourseName() != "null") { // check that course exists
				// print course info
				cout << course.getCourseNumber() << ": " << course.getCourseName() << endl;
				course.printPrerequisites();
				cout << endl;
			}
			else {
				cout << "No such course" << endl;
			}
			
		}
		else if (inputValue == 4) { // exit program
			cout << "Exiting" << endl;
		}
		else {
			cout << "invalid input" << endl;
		}
	}

	cout << "End Session" << endl;

	return 0;
}