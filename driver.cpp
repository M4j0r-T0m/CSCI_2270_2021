#ifndef COMMITMAIN
#define COMMITMAIN
#include "miniGit.cpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;


int main()
    {
        int choice = 0;
        //menu
        string menuStart;
        string menuContinue;
        //filename
        string fileName;
        //committing
        string commitBranchName;
        //checkout ID
        string searchBranchName;
        int searchVersion;
        //branching
        int agreeToBranch = 0;
        int agreeLatestVersion = 0;
        int versionToBranch = 0;
        string branchName;
        string oldBranchName;
        singlyNode* babyHead;
        //Create the repository, creates the first node of the DLL
        menuStart += "============================================================================\n";
        menuStart += "First you will need to create a new repository\nThe program has been initialized\n";
        menuStart += "============================================================================\n";
        cout << menuStart;
        filesystem::create_directory(".minigit");
        filesystem::create_directory(".minigit/master");
        cout << "The directory has been created." << endl;
        menuContinue += "Now you can do one of the following 5 things...\n";
        menuContinue += "1. Add a file\n";
        menuContinue += "2. Remove file\n";
        menuContinue += "3. Commit\n";
        menuContinue += "4. Check out\n";
        menuContinue += "5. Quit\n";
        //create commit tree
        commitTree fileTree = commitTree();
        cin >> choice;
        do
        {
            cout << menuContinue;
            cin >> choice;
            switch(choice)
            {
                //Process for adding a file
                case 1:
                {
                    cout << "What is the name of the file you wish to enter?" << endl;
                    cin >> fileName;
                    fileTree.addFile(fileName);
                    break;
                }
                //Process for deleting a file
                case 2:
                {
                    cout << "What is the name of the file you wish to delete?" << endl;
                    cin >> fileName;
                    fileTree.removeFile(fileName);
                    break;
                }
                //Process of committing
                case 3:
                {
                    cout << "Do you wish to branch?" << endl;
                    cout << "1) Yes\n2) No\n";
                    cin >> agreeToBranch;
                    if (agreeToBranch == 1){
                        cout << "What would you like the branch name to be?" << endl;
                        cin >> branchName;
                        commitBranchName = branchName;
                        cout << "What branch would you like to branch off of?" << endl;
                        cin >> oldBranchName;
                        cout << "Do you wish to branch at the latest version?" << endl;
                        cout << "1) Yes\n2) No\n";
                        cin >> agreeLatestVersion;
                        if (agreeLatestVersion == 1){

                            fileTree.createBranch(fileTree.searchComm(oldBranchName, true), branchName, babyHead);
                        }
                        else{
                            cout << "Which version would you like to branch off at?" << endl;
                            cin >> versionToBranch;
                            fileTree.createBranch(fileTree.searchComm(oldBranchName, versionToBranch), branchName, babyHead);
                        }
                    }
                    else{
                        commitBranchName = "master";
                    }
                    fileTree.commitFiles(commitBranchName);

                    
                    break;
                }
                //Process of checking out
                case 4:
                {
                    cout << "What file do you wish to check out?" << endl;
                    cin >> searchBranchName;
                    cin >> searchVersion;
                    fileTree.checkout(searchBranchName, searchVersion);
                    break;
                }
                //Quitting
                case 5:
                {
                    cout << "All done." << endl;
                    break;
                }
                //Wrong input
                default:
                {
                    cout << "Invalid input, please select a choice of 1-5." << endl;
                    break;
                }
            }
            
        } while (choice != 5);
        
    }

#endif