#ifndef COMMITTREE_CPP
#define COMMITTREE_CPP
#include "commitTree.hpp"
#include<vector>
#include<iostream>
#include<sstream>

using namespace std;

unsigned int commitTree::stringToInt(string name);


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
    nw->commitNum=0;

    if(par != nullptr)
    {
        if(parent->childBranch.size() == 0)
        {
            parent->childBranch.push_back(nullptr);
        }
        parent->childBranch.push_back(nw);
        parent->branched=true;

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

void pushCommit(commitNode * curr, string branchName)
{
    if (root == nullptr)
    {
        commitTree(curr->head); //creates a new commitTree if there isn't one
    }
    else
    {
        commitNode * par = searchComm(branchName, true);    //finds the desired insertion point
        while(par->current == false)
        {
            par = par->childBranch.at(0);
        }
        if(curr->branchName.compare(branchName) != 0)
        {
            createBranch(par, curr->branchName, curr->head);    //if the insertion point has a different branch name, create a new branch
        }
        else
        {
            par->childBranch.at(0) = curr;
            curr->current = true;
            par->current = false;
            curr->commitNum=par->commitNum++;
            singlyNode * parT = par->head;  //temp pointers to the file SLL
            singlyNode * curT = curr->head;
            while(curT != nullptr)
            {
                bool ext = false        //does the file exist?
                while(parT != nullptr) //does the file exist, and has it changed? 
                {                       //if file exists compare the files for changes, and iterate the version number if need be

                    if(curT->fileName.compare(parT->fileName) == 0 && fileCompare(parT->fileName, )) // does the
                }
            }
        }

    }
    

}

// void addFile(string fileName)
// {
//     int version = commitNode->commitNum;
//     singlyNode *node = head;
//     string v = to_string(version);
//     string fileVersion = fileName + v;
//     while(node->next != NULL)
//     {
//         if(node->fileName == fileName)
//         {
//             cout << "A file with that name as already been added." << endl;
//         }
//         node = node->next;
//     }
//     singlyNode *addNode = new singlyNode;
//     addNode->fileName = fileName;
//     addNode->fileVersion = fileVersion;
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