#ifndef COMMITTREE_CPP
#define COMMITTREE_CPP
#include "miniGit.hpp"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>

using namespace std;

//HASH TABLE FUNCTIONS
//----------
hashTable::hashTable(int bsize)
{
    
    this->tableSize = bsize;
    table = new hashNode*[tableSize];
    for(int i=0;i<bsize;i++)
        table[i] = nullptr;

}

//function to calculate hash function
unsigned int hashTable::hashFunction(int key)
{
    return (key % tableSize);
}

//function to search
commitNode* hashTable::searchHash(int key)
{
    
    //Compute the index by using the hash function
    int index = hashFunction(key);
    if (table[index] == nullptr)
    {
        return nullptr;
    }

    //Search the list at that specific index and return the node if found
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

//function to insert


bool hashTable::insertHash(commitNode* targ)
{
    
    int key = commitTree::stringToInt(targ->branchName);
    
    if(!searchHash(key))
    {
        //cout << "not found ... inserting" << endl;
        // TODO :
        // Use the hash function on the key to get the index/slot,
        // create a new node with the key and insert it in this slot's list
        int ind = hashFunction(key);
        hashNode * neu = new hashNode;
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

return false;
}

// function to display hash table
void hashTable::printTable()
{
    
    for (int i = 0; i < tableSize; i++) {
        cout << i <<"|| ";
        hashNode * temp = table[i];
        while (temp != nullptr)
            {
                cout << temp->key << " || ";
                temp = temp->next;
            }
    }
    
 }



//TREE FUNCTIONS
//----------
void commitTree::addFile(string fileName)
 {
     //int version = commitNode->commitNum;
     singlyNode* head = searchComm("Master", true)->head;
     singlyNode *node = head;
     //string v = to_string(version);
     //string fileVersion = fileName + v;
     while(node->next != NULL)
     {
         if(node->fileName == fileName)
         {
             cout << "A file with that name as already been added." << endl;
               return;
         }
         node = node->next;
     }
     singlyNode *addNode = new singlyNode;
     addNode->fileName = fileName;
     addNode->fileVersion = 0;
     //addNode->fileVersion = fileVersion;
     head->next = addNode;
     node->next = NULL;
 }

 void commitTree::removeFile(string fileName)
 {
     singlyNode* head = searchComm("Master", true)->head;
     singlyNode *node = head->next;
     singlyNode *prev = head;
     while(node->fileName != fileName)
     {
         node = node->next;
         prev = prev->next;
     }
     if (node == NULL)
     {
         cout << "No file was found with that name." << endl;
            return;
     }
     else if(node->fileName == fileName)
     {
         prev->next = node->next;
         delete node;
     }
     else if(node->next == NULL)
     {
         prev->next = NULL;
         delete node;
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

void commitTree::checkout(string branchName, int version){

}

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
    createBranch(nullptr, "Master", head);
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


bool commitTree::compareFiles(string targ, string curr){
    /*
    fstream targetFile = fstream();
    string targetFileContents = "";
    fstream currFile = fstream();
    string currFileContents = "";
    string tempStorage = "";
    targetFile.open(targetFileName);
    if (targetFile.is_open()){
       while (getline(targetFile, tempStorage)){
           targetFileContents += tempStorage;
       }
       targetFile.close();
    }
    currFile.open(currFileName);
    if (currFile.is_open()){
        while (getline(currFile, tempStorage)){
            currFileContents += tempStorage;
        }
        currFile.close();
    }
    if (targetFileContents == currFileContents){
        return true;
    }
    else{
        return false;
    }*/
    streampos oldB, oldE;
    streampos newB, newE;
    fstream oldFile;
    oldFile.open(targ);
    fstream newFile;
    newFile.open(curr);

    oldFile.seekg(0, ios::beg);
    oldB =oldFile.tellg();
    oldFile.seekg(0, ios::end);
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

singlyNode* commitTree::sLLNodeAt(commitNode* node, int index){
    singlyNode* temp = node->head;
    for (int i = 0; i < index; i++){
        temp = temp->next;
    }
    return temp;
}

void commitTree::commitFiles(string commitBranchName){
    //go to commitBranchName
    //check commit version
    commitNode* currCommitNode = searchComm(commitBranchName, true);
    int &currCommitNum = searchComm(commitBranchName, true)->commitNum;
    //versionNum = the string version of the commitNum, adjusted with preceding 0s
    string branchVersionNum;
    if (currCommitNum < 10){
        branchVersionNum = "0" + to_string(currCommitNum);
    }
    else{
        branchVersionNum = to_string(currCommitNum);
    }
    //create the directory
    filesystem::create_directory(".minigit/"+ commitBranchName + "/" + commitBranchName + "_" + branchVersionNum);
    //creating the files
    fstream currentFile = fstream();
    fstream newFile = fstream();
    string fileName;
    string fileNameNoType;
    string fileType;
    string fileContent;
    string fileVersionNum;

    bool nextCharFileType = false;
    bool sameFile = false;

    int currNodeSLLSize = 0;
    singlyNode* traversalNode = currCommitNode->head;
    while(traversalNode != NULL){
        traversalNode = traversalNode->next;
        currNodeSLLSize += 1;
    }
    if (currCommitNum == 0){
        for (int i = 0; i < currNodeSLLSize; i++){
        fileName = sLLNodeAt(currCommitNode, i)->fileName;
        fileNameNoType = "";
        fileType = "";
        for (int j = 0; j < fileName.length(); j++){
            if (fileName[j] == '.'){
                nextCharFileType = true;
            }
            if (!nextCharFileType){
                fileNameNoType += fileName[j];
            }
            else{
                fileType += fileName[j];
            }
        }
        currentFile.open(fileName);
        newFile.open(".minigit/" + commitBranchName + "/" + commitBranchName + "_" + branchVersionNum + "/" + fileNameNoType + "_00" + fileType);
        if (currentFile.is_open() && newFile.is_open()){
            while (getline(currentFile, fileContent)){
                newFile << fileContent;
            }
            newFile.close();
            currentFile.close();
        }
        else{
            cout << "Failed to open file." << endl;
        }
        }
    }
    else{
        for (int i = 0; i < currNodeSLLSize; i++){
            fileName = sLLNodeAt(currCommitNode, i)->fileName;
            fileNameNoType = "";
            fileType = "";
            for (int j = 0; j < fileName.length(); j++){
                if (fileName[j] == '.'){
                    nextCharFileType = true;
                }
                if (!nextCharFileType){
                    fileNameNoType += fileName[j];
                }
                else{
                    fileType += fileName[j];
                }
            }
            sameFile = compareFiles(sLLNodeAt(currCommitNode, i)->fileName, fileNameNoType + "_" + to_string(sLLNodeAt(currCommitNode, i)->fileVersion));
            if (!sameFile){
                sLLNodeAt(currCommitNode, i)->fileVersion += 1;
            }
            currentFile.open(fileName);
            if (sLLNodeAt(currCommitNode, i)->fileVersion < 10){
                fileVersionNum = "0" + to_string(sLLNodeAt(currCommitNode, i)->fileVersion);
            }
            else{
                fileVersionNum = to_string(sLLNodeAt(currCommitNode, i)->fileVersion);
            }
            newFile.open(".minigit/" + commitBranchName + "/" + commitBranchName + "_" + branchVersionNum + "/" + fileNameNoType + "_" + fileVersionNum + fileType);
            if (currentFile.is_open() && newFile.is_open()){
                while (getline(currentFile, fileContent)){
                    newFile << fileContent;
                }
                newFile.close();
                currentFile.close();
            }
            else{
                cout << "Failed to open file." << endl;
            }
        }
    }

    currCommitNum++;
}

#endif
