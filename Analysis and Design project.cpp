//============================================================================
// Name        : CS300Project2.cpp
// Author      : Christopher Diaz
// Version     :
// Copyright   : Your copyright notice
// Description : BST for courses
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

// course information
struct Course {
	string number;
	string name;
	vector<string> prerequisites;
};

// For a node in the BST
struct Node {
	Course course; // data for course stored in this node
	Node* left; // pointers to children
	Node* right;

	// constructor for initializing node with a course
	Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// BST class to store and handle courses
class BST {

private:
	Node* root;

	// insert course into BST
	void insert(Node*& node, Course course) {
		if (!node) {
			node = new Node(course); // if spot empty, insert new node
		}
		else if (course.number < node->course.number) {
			insert(node->left, course); // left if course num is smaller
		}
		else {
			insert(node->right, course);
		}
	}

	// print courses in order(alphabetic)
	void inOrder(Node* node) {
		if (!node) return;
		inOrder(node->left);
		cout << node->course.number << ", " << node->course.name << endl;
		inOrder(node->right);
	}

	// search for course by num
	Course* search(Node* node, string number) {
		if (!node) return nullptr;
		if (node->course.number == number) return &node->course;
		if (number < node->course.number)
			return search(node->left, number);
		else
			return search(node->right, number);
	}

public:
	BST() : root(nullptr) {}

	// public method to insert course
	void insert(Course course) {
		insert(root, course);
	}

	// print all courses in sorted order
	void printInOrder() {
		inOrder(root);
	}

	// public method to find a course by num
	Course* find(string number) {
		return search(root, number);
	}
};

// load course data from file and return pointer to BST
BST* loadData(const string& filename) {
	BST* courseTree = new BST();
	ifstream file(filename); // open file

	if (!file.is_open()) {
		cout << "Error: cannot open " << filename << endl;
		return nullptr;
	}

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string token;
		vector<string> tokens;


		// line split by commas
		while (getline(ss, token, ',')) {
			tokens.push_back(token); // store token
		}

		// skip if line has less than 2
		if (tokens.size() < 2) continue;

		Course course;
		course.number = tokens[0];
		course.name = tokens[1];
		course.prerequisites.clear();

		// add additional tokens for prerequisites
		for (size_t i = 2; i < tokens.size(); ++i) {
			if (!tokens[i].empty())
				course.prerequisites.push_back(tokens[i]);
		}

		courseTree->insert(course);
	}

	file.close();
	return courseTree;
}

int main() {
	BST* courseTree = nullptr;
	bool loaded = false; // track if data is loaded or not
	int choice;

	while (true) { // menu options
		cout << "\nMenu:" << endl;
		cout << "1. Load data" << endl;
		cout << "2. Sort and print all courses" << endl;
		cout << "3. Get course details" << endl;
		cout << "9. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;
		cout << endl;

		// for invalid input
		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "\nInvalid input. Enter a number from menu" << endl;
			cout << "Enter choice: ";
			continue;
		}

		switch (choice) {
		case 1:
			if (loaded) { // load data from file
				cout << "Data loaded already" << endl;
			}
			else {
				string filename;
				cout << "Enter filename: " << endl;
				cin.ignore();
				getline(cin, filename);
				courseTree = loadData(filename);
				if (courseTree == nullptr) {
					cout << "Failed to load data" << endl;
				}
				else {
					loaded = true;
					cout << "Data loaded" << endl;
				}
			}
			break;
		case 2:
			if (!loaded) {
				cout << "Load data first" << endl;
			}
			else { // print sorted course list
				courseTree->printInOrder();
			}
			break;
		case 3:
			if (!loaded) {
				cout << "Load data first" << endl;
			}
			else { // look up course information
				string courseNum;
				cout << "Enter the course number: ";
				cin.ignore();
				getline(cin, courseNum);
				cout << endl;
				Course* c = courseTree->find(courseNum);
				if (c) {
					cout << c->number << ": " << c->name << endl;
					if (c->prerequisites.empty()) {
						cout << "No prerequisites found" << endl;
					}
					else {
						cout << "Prerequisites: ";
						for (const auto& p : c->prerequisites) {
							cout << p << " ";
						}
						cout << endl;
					}
				}
				else {
					cout << "Course was not found" << endl;
				}
			}
			break;
		case 9:
			cout << "Exiting the program" << endl;
			delete courseTree;
			return 0;
		default:
			cout << "Invalid choice" << endl;
		}
	}
}
