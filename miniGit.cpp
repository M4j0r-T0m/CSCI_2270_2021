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
void commitTree::addFile(string fileName, commitNode* curr)
 {
     //int version = commitNode->commitNum;
     singlyNode* head = curr;
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

singlyNode* commitTree::sLLNodeAt(commitNode* node, int index){
    singlyNode* temp = node->head;
    for (int i = 0; i < index; i++){
        temp = temp->next;
    }
    return temp;
}

void commitTree::checkout(string branchName, int version){
    int currNodeSLLSize = 0;
    singlyNode* traversalNode = searchComm(branchName, version)->head;
    while(traversalNode != NULL){
        traversalNode = traversalNode->next;
        currNodeSLLSize += 1;
    }
    for (int i = 0; i < currNodeSLLSize; i++){
        fstream targetFile = fstream();
        ofstream currentFile = ofstream();
        string targetFileName = ".minigit/" + branchName + "/" + branchName + "_" ;
        if (version < 10){
            targetFileName += "0" + to_string(version);
        }
        else{
            targetFileName += to_string(version);
        }
        string fileNameNoType = "";
        string fileType = "";
        bool nextCharFileType = false;
        for (int j = 0; j < sLLNodeAt(searchComm(branchName, version), i)->fileName.length(); j++){
                if (sLLNodeAt(searchComm(branchName, version), i)->fileName[j] == '.'){
                    nextCharFileType = true;
                }
                if (!nextCharFileType){
                    fileNameNoType += sLLNodeAt(searchComm(branchName, version), i)->fileName[j];
                }
                else{
                    fileType += sLLNodeAt(searchComm(branchName, version), i)->fileName[j];
                }
            }
        targetFileName += fileNameNoType += "_";
        if (sLLNodeAt(searchComm(branchName, version), i)->fileVersion < 10){
            targetFileName += "0" + to_string(sLLNodeAt(searchComm(branchName, version), i)->fileVersion);
        }
        else{
            targetFileName += to_string(sLLNodeAt(searchComm(branchName, version), i)->fileVersion);
        }
        targetFileName += fileType;
        targetFile.open(targetFileName);
        currentFile.open(sLLNodeAt(searchComm(branchName, version), i)->fileName);
        string targetFileLine = "";
        while (getline(targetFile, targetFileLine)){
            currentFile << targetFileLine;
        }
    }
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
        commitTree(curr->head); //creates a new commitTree if there isn't one
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
        oldFile.close();
        newFile.close();
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
            oldFile.close();
            newFile.close();
            return false;
        }
    }
    oldFile.close();
    newFile.close();
    return true;
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
    ofstream newFile = ofstream();
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
        nextCharFileType = false;
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
            nextCharFileType = false;
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
            if (sLLNodeAt(currCommitNode, i)->fileVersion < 10){
                fileVersionNum = "0" + to_string(sLLNodeAt(currCommitNode, i)->fileVersion);
            }
            else{
                fileVersionNum = to_string(sLLNodeAt(currCommitNode, i)->fileVersion);
            }
            sameFile = compareFiles(sLLNodeAt(currCommitNode, i)->fileName, fileNameNoType + "_" + fileVersionNum);
            if (!sameFile){
                sLLNodeAt(currCommitNode, i)->fileVersion += 1;
            }
            currentFile.open(fileName);
            
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

void printHelper(commitNode* root){
    for (int i = 0; i < root->childBranch.size(); i++){
        if (root->childBranch.at(i) != NULL){
            printHelper(root->childBranch.at(i));
        }
    }
    singlyNode* sNode = root->head;
    cout << "branch name: " << root->branchName << endl;
    cout << "branch version: " << root->commitNum << endl;
    int sNodeNum = 1;
    while (sNode != NULL){
        cout << "--" << sNodeNum << "th file name: " << sNode->fileName << endl;
        cout << "--" << sNodeNum << "th max file version: " << sNode->fileVersion << endl;
        sNodeNum += 1;
        sNode = sNode->next;
    }
}

void commitTree::print(){
    printHelper(root);
}

#endif
