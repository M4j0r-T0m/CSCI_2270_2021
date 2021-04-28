#ifndef COMMITTREE_CPP
#define COMMITTREE_CPP
#include "miniGit.hpp"
#include<vector>
#include<iostream>
#include<sstream>
#include<fstream>

using namespace std;

unsigned int commitTree::stringToInt(string name);

string nameVersioner(const string& fName)
{
    stringstream powerWord;
    string output;
    string realName;
    int version;
    if(fName.find("_")>0)
    {
        realName = fName.substr(0, fName.find("_"));
        string fVer = fName.substr(fName.find("_")+1, fName.find("."));
        version = stoi(fVer)+1;
    }
    else
    {
        realName = fName.substr(0, fName.find("."));
        version = 00;
    }
    string filetype = fName.substr(fName.find(".")+1, fName.length());
    powerWord << realName << "_" << version << "." << filetype;
    powerWord >> output;
    return output;
}

string makeFilePath(commitNode* curr,const string& fName)
{
    string output;
    stringstream bigName;
    bigName << ".minigit/" ;
    bigName << curr->branchName << "/";
    bigName << curr->branchName << "_" << curr->commitNum << "/" ;
    bigName << fName ;
    bigName >> output;
    return output;
    
}
hashTable::hashTable(int bsize)
{
    this->tableSize = bsize;
    table = new hashNode*[tableSize];
    for(int i=0;i<bsize;i++)
        table[i] = nullptr;
}

//function to calculate hash function
unsigned int HashTable::hashFunction(int key)
{
    return (key % tableSize);
}

// TODO Complete this function
//function to search
commitNode* HashTable::searchHash(int key)
{
    //Compute the index by using the hash function
    int index = hashFunction(key);
    if (table[index] == nullptr)
    {
        return nullptr;
    }

    //TODO: Search the list at that specific index and return the node if found
    hashNode * temp;
    temp = table[index];
    while(temp != nullptr)
        {
            if(key == temp->key)
            {
                return temp->target;
            }
            else
            {
                temp = temp->next;
            }
        }
    return nullptr;
}

//TODO Complete this function
//function to insert


bool HashTable::insertHash(commitNode* targ)
{
    int key = commitTree::stringToInt(targ->branchName);
    
    if(!searchHash(key))
    {
        //cout << "not found ... inserting" << endl;
        // TODO :
        // Use the hash function on the key to get the index/slot,
        // create a new node with the key and insert it in this slot's list
        int ind = hashFunction(key);
        hashNode * neu = new hashnode;
        neu->key = key;
        neu->target = targ;
        hashNode * temp;
        temp = table[ind];
        //cout << "whilin" << endl;
        if (temp == nullptr)
        {
            table[ind]=neu;
            return true;
        }
        else
        {
            while (temp->next != nullptr)
            {
                temp = temp->next;
                //cout << "tempin" << endl;
            }
            //cout << "doodoo" << endl;
            temp->next = neu;
            
            return true;
        }
     }
    else{
        cout<<"duplicate entry: "<<key<<endl;
        return false;
    }

}

//TODO Complete this function
// function to display hash table
void HashTable::printTable()
{
    for (int i = 0; i < tableSize; i++) {
        cout << i <<"|| ";
        hashNode * temp = table[i];
        while (temp != nullptr)
            {
                cout << temp->key << " || ";
                temp = temp->next;
            }
        //TODO
    }

 }

 unsigned int commitTree::stringToInt(string name)
 {
    stringstream ascii;
    for(int i=0; i<name.size(); i++)
    {
        char x = name.at(i);
        ascii << int(x);
    }
    
    int key;
    ascii >> key;
    return key;
 }

// bool branchNameFinder(string Name, commitNode* curr)
// {
//     bool found = false;
//     if(curr == nullptr)
//     {
//         return false;
//     }
//     else if(curr->branchName.compare(Name) == 0)
//     {
//         return true;
//     }
//     else
//     {
//         for(int i = 0; i<curr->childBranch.size(), i++)
//         {
//             found = (found || branchNameFinder(Name, childBranch.at(i)));
//         }
//     }
//     return found;
// }

void commitTree::createBranch(commitNode *par, string branchN, singlyNode* babyHead)
{
    if(searchHash(stringToInt(branchN)) != nullptr)
    {
        cout << "Error: Pick a new branch name" << endl;

        return;
    }
    commitNode * nw = new commitNode;
    nw->branchName= branchN;
    nw->current = true;
    nw->branched = false;
    nw->parent = par;
    nw->head = babyHead;

    if(par != nullptr)
    {
        if(par->branchName.compare(branchN)!=0)
        {
            if(par->childBranch.size() == 0)             //if childbranch has no entry
            {   //garbage entry at index 0
                par->childBranch.push_back(nullptr);     
            }
            par->childBranch.push_back(nw);
            par->branched=true;
            nw->commitNum=0;
        }
        else
        {
            par->childBranch.push_back(nw);
            par->current=false;
            nw->commitNum=par->commitNum++;
        }

        singlyNode * oldList = par->head;
        singlyNode * newList = nw->head;
        string oldLine;
        string newLine;
        while(oldList != nullptr)
        {
            while (newList != nullptr)
            {
                if(oldList->fileName.compare(newList->fileName) == 0)
                {
                    stringstream fcopy;
                    string filecopy;
                    if(fileCompare(newList->fileName, makeFilePath(par, newList->fileVersion)))
                    {
                        fcopy << "cp " << makeFilePath(par, oldList->fileVersion) << " " ;
                        fcopy << ".minigit/" nw->branchName << "/" << nw->branchName << "_" << nw->commitNum << "/" << oldList->fileVersion;
                        fcopy >> filecopy;
                        system(filecopy);
                    }
                    else
                    {
                        fcopy << "cp " << makeFilePath(par, oldList->fileVersion) << " " ;
                        fcopy << ".minigit/" nw->branchName << "/" << nw->branchName << "_" << nw->commitNum << "/" << fileVersioner(oldList->fileVersion);
                        fcopy >> filecopy;
                        system(filecopy);                        
                    }
                }
                newList = newList->next;
            }
            oldList = oldList->next;
        }
        while(oldList != nullptr)
        {
            bool missing = true
            stringstream fcopy;
            string filecopy;
            while (newList != nullptr)
            {
                if(oldList->fileName.compare(newList->fileName) == 0)
                {
                    missing=false;
                }
                newList = newList->next;
            }
            if(missing)
            {
                fcopy << "cp " << makeFilePath(par, oldList->fileVersion) << " " ;
                fcopy << ".minigit/" nw->branchName << "/" << nw->branchName << "_" << nw->commitNum << "/" << oldList->fileVersion;
                fcopy >> filecopy;
                system(filecopy);
            }
            oldList = oldList->next;
        }

    }
    else
    {
        this->root = nw;
        nw->branchName= "Main"


    }

    insertHash(nw);

}

commitTree::commitTree(singlyNode* head)
{
    hashTable(100);
    createBranch(nullptr, "Main", head);
}
commitTree::commitTree()
{
    hashTable(100);
    createBranch(nullptr, "Main", nullptr);
}

commitNode* commitTree::searchComm(string branchName, bool latest) //overloaded function that either gives the first or last node of a commit branch
{
    commitNode* curr = searchHash(stringToInt(branchName)); //accesses a branch via hashtable
    if(latest = false)
    {
        return curr;        //returns a pointer to the origin of a branch
    }
    else
    {
        while (curr->current==false)
        {
            curr = curr->childBranch.at(0);     //returns a pointer to the latest iteration of a branch
        }
    }
    return curr;
}
commitNode* commitTree::searchComm(string branchName, int ver)  //overloaded function to find a specific commit number
{
    commitNode* curr = searchHash(stringToInt(branchName));
    while (curr->commitNum != ver)
    {
        curr = curr->childBranch.at(0);
    }
    return curr;    //returns pointer to desired node
}

copyHelper(string file, string targDir)
{
    ofile()
}

void commitTree::pushCommit(singlyNode * babyHead)
{
    if (root == nullptr)
    {
        commitTree(babyHead); //creates a new commitTree if there isn't one
    }
    if (root->head == nullptr)
    {
        root->head = babyHead;
        singlyNode * temp = babyHead;
        
        
        system("\"mkdir .minigit/main/main_00\"");
        while (temp != nullptr)
        {
            temp->fileVersion=fileVersioner(temp->fileName);
            stringstream fcopy << "cp " << temp->fileName << " " << ".minigit/main/main_00/" << temp->fileVersion;
            string filecopy;
            fcopy >> filecopy;
            system(filecopy);
        }


    }
    else
    {
        createBranch(currentBranch, currentBranch->branchName, babyHead);

    }
}


void commitTree::pushCommit(singlyNode * babyHead, string branchName)
{
    
    {
    if (root == nullptr)
    {
        commitTree(babyHead); //creates a new commitTree if there isn't one
    }
    if (root->head == nullptr)
    {
        root->head = babyHead;
        singlyNode * temp = babyHead;
        
        
        system("\"mkdir .minigit/main/main_00\"");
        while (temp != nullptr)
        {
            temp->fileVersion=fileVersioner(temp->fileName);
            stringstream fcopy << "cp " << temp->fileName << " " << ".minigit/main/main_00/" << temp->fileVersion;
            string filecopy;
            fcopy >> filecopy;
            system(filecopy);
        }


    }
    else
    {
        this->currentBranch=searchComm(branchName, true);
        createBranch(currentBranch, currentBranch->branchName, babyHead);

    }
}

void commitTree::pushCommit(singlyNode * babyHead, string branchName, string newName)
{
    
    {
    if (root == nullptr)
    {
        commitTree(babyHead); //creates a new commitTree if there isn't one
    }
    if (root->head == nullptr)
    {
        root->head = babyHead;
        singlyNode * temp = babyHead;
        
        
        system("\"mkdir .minigit/main/main_00\"");
        while (temp != nullptr)
        {
            temp->fileVersion=fileVersioner(temp->fileName);
            stringstream fcopy << "cp " << temp->fileName << " " << ".minigit/main/main_00/" << temp->fileVersion;
            string filecopy;
            fcopy >> filecopy;
            system(filecopy);
        }


    }
    else
    {
        this->currentBranch=searchComm(branchName, true);
        createBranch(currentBranch, newName, babyHead);

    }
}

bool fileCompare(string targ, string curr)
{
    streampos oldB, oldE;
    streampos newB, newE;
    ifilestream oldFile;
    oldFile.open(targ);
    ifilestream newFile;
    newFile.open(curr);

    oldFile.seekg(0, ios::beg);
    oldB =oldFile.tellg();
    oldFile.seekg(0, ios::end):
    oldE = oldFile.tellg();

    newFile.seekg(0, ios::beg);
    newB = newFile.tellg();
    newFile.seekg(0, ios::end);
    newE = newFile.tellg();

    if((newE-newB)-(oldE-oldB) != 0)
    {
        return false;
    }

    newFile.seekg(0, ios::beg);
    oldFile.seekg(0, ios::beg);

    bool neu = false;
    string newLine,oldLine;
    while(!newFile.eof() || !oldFile.eof())
    {
        getline(newFile, newLine);
        getline(oldFile, oldLine);
        if(newLine.compare(oldLine)!=0)
        {
            return false;
        }
    }
    return true;
}

// void addFile(string fileName)
// {
//     //int version = commitNode->commitNum;
//     singlyNode *node = head;
//     //string v = to_string(version);
//     //string fileVersion = fileName + v;
//     while(node->next != NULL)
//     {
//         if(node->fileName == fileName)
//         {
//             cout << "A file with that name as already been added." << endl;
               //return;
//         }
//         node = node->next;
//     }
//     singlyNode *addNode = new singlyNode;
//     addNode->fileName = fileName;
//     //addNode->fileVersion = fileVersion;
//     head->next = addNode;
//     node->next = NULL;
// }

// void removeFile(string fileName)
// {
//     singlyNode *node = head->next;
//     singlyNode *prev = head;
//     while(node->fileName != fileName)
//     {
//         node = node->next;
//         prev = prev->next;
//     }
//     if (node == NULL)
//     {
//         cout << "No file was found with that name." << endl;
            return;
//     }
//     else if(node->fileName == fileName)
//     {
//         prev->next = node->next;
//         delete node;
//     }
//     else if(node->next == NULL)
//     {
//         prev->next = NULL;
//         delete node;
//     }
// }

#endif
