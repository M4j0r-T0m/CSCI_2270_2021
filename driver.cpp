#ifndef COMMITMAIN
#define COMMITMAIN
#include "commitTree.cpp"
#include<vector>
#include<iostream>
#include<sstream>

using namespace std;


int main()
    {
        int choice = 0;
        bool repeat;
        string repoName;
        //Create the repository, creates the first node of the DLL
        cout << "============================================================================" << endl;
        cout << "First you will need to create a new repository, what do you wish to name it?" << endl;
        cout << "============================================================================" << endl;
        cin >> repoName;
        cout << "Now you can do one of the following 5 things..." << endl;
        cout << "1. Add a file" << endl;
        cout << "2. Remove file" << endl;
        cout << "3. Commit" << endl;
        cout << "4. Check out" << endl;
        cout << "5. Quit" << endl;
        cout << "hey lol" << endl;
        cin >> choice;
        do
        {
            switch(choice)
            {
                //Proccess for adding a file
                case 1:
                {
                    cout << "What is the name of the file you wish to enter?" << endl;


                    cout << "Now you can do one of the following 5 things..." << endl;
                    cout << "1. Add a file" << endl;
                    cout << "2. Remove file" << endl;
                    cout << "3. Commit" << endl;
                    cout << "4. Check out" << endl;
                    cout << "5. Quit" << endl;
                    cin >> choice;
                    repeat = true;
                    break;
                }
                //Process for deleting a file
                case 2:
                {
                    cout << "What is the name of the file you wish to delete?" << endl;


                    cout << "Now you can do one of the following 5 things..." << endl;
                    cout << "1. Add a file" << endl;
                    cout << "2. Remove file" << endl;
                    cout << "3. Commit" << endl;
                    cout << "4. Check out" << endl;
                    cout << "5. Quit" << endl;
                    cin >> choice;
                    repeat = true;
                    break;
                }
                //Process of commiting
                case 3:
                {
                    cout << "Do you wish to commit?" << endl;


                    cout << "Now you can do one of the following 5 things..." << endl;
                    cout << "1. Add a file" << endl;
                    cout << "2. Remove file" << endl;
                    cout << "3. Commit" << endl;
                    cout << "4. Check out" << endl;
                    cout << "5. Quit" << endl;
                    cin >> choice;
                    repeat = true;
                    break;
                }
                //Process of checking out
                case 4:
                {
                    cout << "Do you wish to check out?" << endl;


                    cout << "Now you can do one of the following 5 things..." << endl;
                    cout << "1. Add a file" << endl;
                    cout << "2. Remove file" << endl;
                    cout << "3. Commit" << endl;
                    cout << "4. Check out" << endl;
                    cout << "5. Quit" << endl;
                    cin >> choice;
                    repeat = true;
                    break;
                }
                //Quitting
                case 5:
                {
                    cout << "All done." << endl;
                    repeat = false;
                    break;
                }
                //Wrong input
                default:
                {
                    cout << "Invalid input, please select a choice of 1-5." << endl;
                    cout << "Now you can do one of the following 5 things..." << endl;
                    cout << "1. Add a file" << endl;
                    cout << "2. Remove file" << endl;
                    cout << "3. Commit" << endl;
                    cout << "4. Check out" << endl;
                    cout << "5. Quit" << endl;
                    cin >> choice;
                    repeat = true;
                }
            }
            
        } while (repeat);
        
    }

#endif