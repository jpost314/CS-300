//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Jordan Poston
// Version     : 1.3
// Copyright   : Copyright © 2025 SNHU COCE
// Description : Project Two
//============================================================================

# include <iostream>
# include <iomanip>
# include <string>
# include <vector>
# include <sstream>
# include <fstream>

using namespace std;

//Struct
struct Course {
    string courseId;
    string courseName;
    vector<string> PreReqs;

    //Constructor
    Course() {};
    //W/ values
    Course(string aCourseId, string aCourseName) {
        courseId = aCourseId;
        courseName = aCourseName;
    }
    //Add prereqs to contructor
    Course(string aCourseId, string aCourseName, vector<string> prereqs) {
        courseId = aCourseId;
        courseName = aCourseName;
        vector<string> PreReqs = prereqs;
    }
};

//Struct for node
struct Node {
    Course course;
    Node* left;
    Node* right;

    //Node constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course acourse) : Node() {
        this->course = acourse;
    }
};

//Binary Tree 
class BinaryTree {
private:
    Node* root;
    void addNode(Course acourse, Node* node);
    void inOrder(Node* node);
    void printCourse(Node* node, string courseId);

public:
    BinaryTree();
    void InOrder();
    void PrintCourse(string courseId);
    bool loadData();
    bool validatePreReqs(Course course);
    void insert(Course course);
};

//Binary Tree constructor
BinaryTree::BinaryTree() {
    root = nullptr;
}

//InOrder to pass to root
void BinaryTree::InOrder() {
    this->inOrder(root);
}

//Send root to print method
void BinaryTree::PrintCourse(string courseId) {
    this->printCourse(root, courseId);
}

//Insertion method for courses
void BinaryTree::insert(Course course) {
    //if root is empty
    if (root == nullptr) {
        root = new Node(course);
    }
    //Addition method passes root
    else {
        this->addNode(course, root);
    }
}

//Insertion method for nodes to tree
void BinaryTree::addNode(Course aCourse, Node* node) {

    //When selected node > passed courseId
    if (node->course.courseId.compare(aCourse.courseId) > 0) {
        //If left node is empty
        if (node->left == nullptr) {
            node->left = new Node(aCourse);
        }
        //Traverse left side
        else {
            this->addNode(aCourse, node->left);
        }
    }
    //When selected node < passed courseId
    else {
        //If right node is empty
        if (node->right == nullptr) {
            node->right = new Node(aCourse);
        }
        //Traverse right side
        else {
            this->addNode(aCourse, node->right);
        }
    }
}

//Ordered print method
void BinaryTree::inOrder(Node* node) {

    if (node != nullptr) {
        //Traverse left side
        inOrder(node->left);
        //Course info print method for selected node
        cout << "Course ID: " << node->course.courseId;
        cout << "Course Name: " << node->course.courseName;
        cout << "Prerequisites: ";

        //Loop through prereq for printing
        for (int i = 0; i < node->course.PreReqs.size(); ++i) {
            cout << node->course.PreReqs[i] << " ";
        }
        if (node->course.PreReqs.size() == 0) {
            cout << "N/A";
        }
        cout << "\n";

        //Traverse right side
        inOrder(node->right);
    }
    return;
}

//Method for single course printing
void BinaryTree::printCourse(Node* node, string courseId) {

    while (node != nullptr) {
        //If direct match exists
        if (node->course.courseId.compare(courseId) == 0) {
            cout << "Course ID: " << node->course.courseId;
            cout << "  Course Name: " << node->course.courseName;
            cout << "  Prerequisites: ";
            //Loop through <prereq> for printing
            for (int i = 0; i < node->course.PreReqs.size(); ++i) {
                cout << node->course.PreReqs[i] << " ";
            }
            //If there are no prereqs
            if (node->course.PreReqs.size() == 0) {
                cout << "None";
            }
            cout << "\n";
            return;
        }
        //If selected courseId < than passed courseId
        else if (node->course.courseId.compare(courseId) > 0 && node->left != nullptr) {
            node = node->left;
        }
        //If selected courseId > than passed courseId
        else if (node->course.courseId.compare(courseId) < 0 && node->right != nullptr) {
            node = node->right;
        }
        else {
            cout << "Course ID not found. Please use a valid Course ID.  \n";
            return;
        }
    }
    //Data load error message
    if (root == nullptr) {
        cout << "An unexpected data error has occurred. \n";
        return;
    }
}


//Pull data into binary tree
bool BinaryTree::loadData() {

    //Open file w/ fstream
    fstream file;
    file.open("CS 300 ABCU_Advising_Program_Input.csv");

    if (file.is_open()) {
        //Loop until eof
        while (!file.eof()) {
            //Vector for line elements
            vector<string> courseLine;
            //Parse lines
            string line;

            //Assign from getLine
            getline(file, line);
            //Check for line content
            while (line.length() > 0) {
                unsigned int delim = line.find(',');
                //Character limit
                if (delim < 100) {
                    //Element addition, then delete
                    courseLine.push_back(line.substr(0, delim));
                    line.erase(0, delim + 1);
                }
                //Pushback to add items and clear
                else {
                    courseLine.push_back(line.substr(0, line.length()));
                    line = "";
                }
            }

            //Local object for insertion
            Course course;

            //Add courseId and courseName
            course.courseId = courseLine[0];
            course.courseName = courseLine[1];
            //Pushback to add to <prereq>
            for (unsigned int i = 2; i < courseLine.size(); i++) {
                course.PreReqs.push_back(courseLine[i]);
            }
            //Binary Tree insertion
            insert(course);
        }
        return true;
    }
    if (!file) {
        cout << "Error: File not found \n";
        return false;
    }
    file.close();
}

bool BinaryTree::validatePreReqs(Course course) {
    //Set local node to root
    Node* current = root;
    if (current->course.PreReqs.size() > 0) {
        //Loop through <prereq>
        for (int i = 0; i < course.PreReqs.size(); i++) {
            while (current != nullptr) {
                //Match discovered
                if (course.PreReqs[i] == current->course.courseId) {
                    return true;
                }
                //If courseId < prereq
                else if (course.PreReqs[i].compare(current->course.courseId) > 0 && current->left != nullptr) {
                    current = current->left;
                }
                //If courseId > prereq
                else if (course.PreReqs[i].compare(current->course.courseId) < 0 && current->right != nullptr) {
                    current = current->right;
                }
                //CourseId not located
                else {
                    cout << "Prerequisite not found. \n";
                    return false;
                }
            }
        }
    }
    else {
        //No prereqs
        return true;
    }
}

int main()
{
    //Create Binary Tree 
    BinaryTree* binaryTree = new BinaryTree();
    //CourseID for inputs
    string courseId;
    //Choice set to zero
    int choice = 0;
    //Loop until input = 9
    while (choice != 9) {
        cout << "*****************************\n";
        cout << "|      Course Planner       |\n";
        cout << "|---------------------------|\n";
        cout << "|(1) Load Course Data       |\n";
        cout << "|(2) Print List of Courses  |\n";
        cout << "|(3) Print Course           |\n";
        cout << "|(9) Exit                   |\n";
        cout << "*****************************\n";
        cout << "Please make a selection: ";
        cin >> choice;

        //Input choices 
        switch (choice) {
            //Data load
        case 1:
            binaryTree->loadData();
            break;
            //Ordered print choice
        case 2:
            cout << "\n         Course List: \n";
            cout << "***************************** \n";
            binaryTree->InOrder();
            cout << "\n";
            break;
            //Course search choice
        case 3:
            cout << "Please enter the selected course ID: ";
            cin >> courseId;
            binaryTree->PrintCourse(courseId);
            cout << "\n";
            break;
            //Exit choice
        case 9:
            cout << "\nThank you. Goodbye. \n";
            break;
            //Input error 
        default:
            cout << "Unexpected error. Please make a valid selection from the menu.\n";
        }
    }
    return 0;
}