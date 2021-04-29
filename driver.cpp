#include "miniGit.cpp"
#include <filesystem>
#include <ctype.h>
#include <locale>
//#pragma once

using namespace std;

void addListReInit(singlyNode* addHead)
{
    
    addHead->fileName="init";

    while(addHead->next != nullptr)
    {
        singlyNode* temp;
        temp = addHead->next;
        if(addHead->next->next != nullptr)
        {
            addHead->next = temp->next;
        }
        delete temp;
    }
    addHead->next = nullptr;
}


int main()
{
    
    //menu
    string menuStart;
    string menuContinue;
    //filename
    string fileName;
    //committing
    // string commitBranchName;
    //checkout ID
    // string searchBranchName;
    int searchVersion;
    //branching
    int agreeToBranch = 0;
    int agreeLatestVersion = 0;
    int versionToBranch = 0;
    // string branchName;
    // string oldBranchName;
    //singlyNode* babyHead;
    //Create the repository, creates the first node of the DLL
    menuStart += "============================================================================\n";
    menuStart += "First you will need to create a new repository\nThe program has been initialized\n";
    menuStart += "============================================================================\n";
    cout << menuStart;
    // filesystem::create_directory(".minigit");
    // filesystem::create_directory(".minigit/master");
    cout << "The directory has been created." << endl;
    menuContinue += "Now you can do one of the following 5 things...\n";
    menuContinue += "1. Add a file\n";
    menuContinue += "2. Remove file\n";
    menuContinue += "3. Commit\n";
    menuContinue += "4. Check out\n";
    menuContinue += "5. Quit\n";
    //create commit tree
    singlyNode* newHead = new singlyNode;
    newHead->next = nullptr;
    newHead->fileName="init";
    newHead->fileVersion="init_00.";
    commitTree fileTree = commitTree(newHead);
    bool menuLoop = true;
    
    while (menuLoop)
    {
        int choice;
        cout << menuContinue;
        cin >> choice;
        cin.ignore(10000, '\n');
        switch(choice)
        {
        
            //Process for adding a file
            case 1:
            {
                cout << "What is the name of the file you wish to enter?" << endl;
                
                getline(cin, fileName);
                cin.ignore(10000, '\n');
                fileTree.addFile(fileName, newHead);
                //addListReInit(newHead);

                
            }break;
            //Process for deleting a file
            case 2:
            {
                cout << "What is the name of the file you wish to delete?" << endl;
                getline(cin, fileName);
                cin.ignore(10000, '\n');
                fileTree.removeFile(fileName);
                
            }break;
            //Process of committing
            case 3:
            {
                bool looped=true;
                while(looped)
                {
                    cout << "Enter a valid existing branch name, a new branch name, or hit enter to commit at last branch." << endl;
                    
                    string branchAppel;
                    getline(cin, branchAppel);
                    cin.ignore(10000, '\n');
                    if(branchAppel.empty())
                    {
                        fileTree.pushCommit(newHead);
                        addListReInit(newHead);
                        looped = false;
                        break;
                    }
                    else if (fileTree.currentBranch->branchName.compare(branchAppel) == 0)
                    {
                        fileTree.pushCommit(newHead);
                        addListReInit(newHead);
                        looped = false;
                        break;
                    }
                    else
                    {

                        if(fileTree.searchComm(branchAppel, false) == nullptr)
                        {
                            
                            bool looped1 = true;
                            bool curr = false;
                            while(looped1)
                            {
                                cout << "Branch not found. Would you like to create a new branch with this name?: Y/N" << endl;
                                string ysno;
                                getline(cin, ysno);
                                cin.ignore(10000, '\n');
                                locale loc;
                                for(int i = 0; i<ysno.size(); i++)
                                {
                                    tolower(ysno[i]);
                                }

                            
                                
                                if (ysno.compare("yes")==0 || ysno.compare("y"))
                                {
                                    looped1=false;
                                    bool looped2=true;
                                    while(looped2)
                                    {
                                        int a = 0;
                                        cout << "Please enter a valid existing branch name or hit enter to commit at last branch." << endl;
                                        string branchLocation;
                                        getline(cin, branchLocation);
                                        cin.ignore(10000, '\n');
                                    
                                    
                                        if(branchLocation.size()==0)
                                        {
                                            fileTree.createBranch(fileTree.currentBranch, branchAppel, newHead);
                                            looped2=false;
                                            looped1=false;
                                            looped=false;
                                            addListReInit(newHead);
                                            break;
                                        }
                                        else if(fileTree.searchComm(branchLocation, false) != nullptr)
                                        {
                                            looped2=false;
                                            looped=false;
                                            looped1=false;
                                            cout << "Enter a branch version number or leave blank to commit to current branch node." << endl;
                                            string pushVer;
                                            getline(cin, pushVer);
                                            cin.ignore(10000, '\n');

                                            try
                                            {
                                                a = stoi(pushVer);
                                            }        // Standard exceptions for stoi
                                            catch (const invalid_argument & e) 
                                            {
                                                curr=true;
                                            }
                                            catch (const out_of_range & e) 
                                            {
                                                curr=true;
                                            }
                                            if(curr)
                                            {
                                                fileTree.pushCommit(newHead, branchLocation, branchAppel);
                                                addListReInit(newHead);
                                            }
                                            else
                                            {
                                                fileTree.createBranch(fileTree.searchComm(branchLocation, a), branchAppel, newHead);
                                                addListReInit(newHead);
                                            }
                                            break;
                                        }
                                        else
                                        {
                                            cout << "Insertion point not found." << endl;
                                            looped2=true;
                                            break;
                                        }
                                    
                                    }
                                }
                                else if (ysno.compare("no")==0 || ysno.compare("n"))
                                {
                                    looped=true;
                                    looped1=false;
                                    break;
                                }
                                else
                                {
                                    cout << "Invalid input, please enter \"Yes\" or \"No\"" << endl;
                                    looped1=true;
                                    break;
                                }
                            
                            }
                        }
                    }
                }
                                
                
            }break;
            //Process of checking out
            case 4:
            {
            
                bool checkoutLoop= true;
                while(checkoutLoop)
                {
                    cout << "What file do you wish to check out? Enter nothing to pull most recent branch, or hit E to exit" << endl;
                    string searchBranchName;
                    getline(cin, searchBranchName);
                    cin.ignore(10000, '\n');
                    int verS = 0;
                    bool curr = false;
                    if (searchBranchName.compare("e") == 0 ||searchBranchName.compare("E")==0)
                    {
                        checkoutLoop=false;
                        
                    }
                    else if(searchBranchName.size()==0)
                    {
                        cout << "Caution! This will overwrite any conflicting local files! Do you wish to continue? Y/N:" << endl;
                        string ysno;
                        getline(cin, ysno);
                        cin.ignore(10000, '\n');
                        locale loc;
                        for(int i = 0; i<ysno.size(); i++)
                        {
                            tolower(ysno[i]);
                        }

                        if(ysno.compare("yes")==0 || ysno.compare("y"))
                        {
                            checkoutLoop=false;

                            fileTree.checkout(fileTree.currentBranch->branchName, fileTree.currentBranch->commitNum);
                            addListReInit(newHead);
                            break;
                            
                        }
                        else if(ysno.compare("no")==0 || ysno.compare("n"))
                        {
                            checkoutLoop=false;
                            break;
                        }
                        else
                        {
                            cout << "Invalid input, please enter \"Yes\" or \"No\"" << endl;
                            checkoutLoop=true;
                            break;
                        }
                    
                    }

                    else if(searchBranchName.compare("nothing")==0 || searchBranchName.compare("NOTHING")==0 || searchBranchName.compare("Nothing")==0)
                    {
                        cout<< "Haha wiseguy, try again." << endl;
                        checkoutLoop=true;
                        break;
                    }
                    
                    else if(fileTree.searchComm(searchBranchName, false)!=nullptr)
                    {
                        checkoutLoop = false;
                        cout << "Enter version or hit enter for latest";
                        string searchVersion;
                        getline(cin, searchVersion);
                        cin.ignore(10000, '\n');
                        cout << "Caution! This will overwrite any conflicting local files! Do you wish to continue? Y/N:" << endl;
                        string ysno;
                        getline(cin, ysno);
                        cin.ignore(10000, '\n');
                        locale loc;
                        for(int i = 0; i<ysno.size(); i++)
                        {
                            tolower(ysno[i]);
                        }

                        if (ysno.compare("yes")==0 || ysno.compare("y"))
                        {
                            checkoutLoop=false;
                            try
                            {
                                verS = stoi(searchVersion);
                            }        // Standard exceptions for stoi
                            catch (const invalid_argument & e) 
                            {
                                curr=true;
                            }
                            catch (const out_of_range & e) 
                            {
                                curr=true;
                            }
                            if(curr)
                            {
                                fileTree.checkout(searchBranchName, fileTree.searchComm(searchBranchName, true)->commitNum);
                                checkoutLoop=true;
                                addListReInit(newHead);
                                break;
                            }
                            else
                            {
                                fileTree.checkout(searchBranchName, fileTree.searchComm(searchBranchName, verS)->commitNum);
                                checkoutLoop=true;
                                addListReInit(newHead);
                                break;
                            }
                            break;
                            
                        }
                        else if (ysno.compare("no")==0 || ysno.compare("n"))
                        {
                            checkoutLoop=false;
                            break;
                        }
                        else
                        {
                            cout << "Invalid input, please enter \"Yes\" or \"No\"" << endl;
                            checkoutLoop=true;
                            break;
                        }
                    
                    }

                
                }
            }break;
            //Quitting
            case 5:
            {
                cout << "All done." << endl;
                menuLoop=false;
                return 1;
            }break;
            //Wrong input
            default:
            {
                cout << "Invalid input, please select a choice of 1-5." << endl;
            }break;
            
        }
        
    } 
}
