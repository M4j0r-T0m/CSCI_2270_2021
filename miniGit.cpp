#ifndef COMMITTREE_CPP
#define COMMITTREE_CPP
#include "miniGit.hpp"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <stdio.h>

using namespace std;

inline bool fileExists (const string& name);

string nameVersioner(const string& fName)
{
    stringstream powerWord;
    string output;
    string realName;
    int version;
    size_t found = fName.find("_");

    if (found != string::npos)
    {
        realName = fName.substr(0, fName.find("_"));
        string fVer = fName.substr(fName.find("_")+1, fName.find("."));
        try
        {
            version = stoi(fVer)+1;
        }
        catch (const invalid_argument & e) 
        {
            version = 0;
        }
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
commitTree::~commitTree()
{}


void commitTree::addFile(string fileName, singlyNode * head)
 {
    if(!fileExists(fileName))
    {
        cout << "Error: File not found." << endl;
        return;
    }
     //int version = commitNode->commitNum;
    if(head->fileName.compare("init") == 0)
        {
            head->fileName=fileName;
            head->fileVersion = nameVersioner(fileName);
            head->next=nullptr;
            return;
        }
     singlyNode * finder = head;
     //string v = to_string(version);
     //string fileVersion = fileName + v;
     while(finder->next != NULL)
     {
         if(finder->fileName.compare(fileName) == 0)
         {
            cout << "A file with that name as already been added." << endl;
            return;
         }
         finder = finder->next;
     }
     singlyNode * addNode = new singlyNode;
     addNode->fileName=fileName;
     addNode->fileVersion=nameVersioner(fileName);
     addNode->next=nullptr;
     finder->next=addNode;
     
 }

 void commitTree::removeFile(string fileName)
 {
     removeFile(fileName, this->currentBranch);
 }

 void commitTree::removeFile(string fileName, commitNode* curr)
 {
     singlyNode* head = curr->head;
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

inline bool fileExists (const string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

void commitTree::checkout(string branchName, int version){
    string filecopy;
    stringstream fcopy;
    commitNode * temp = (searchComm(branchName, version));
    singlyNode * skree = temp->head;
    while(skree != nullptr)
    {

            string skreeName = makeFilePath(temp, skree->fileVersion);

            fcopy << "cp " << skreeName << " " ;
            fcopy << skree->fileName;
            fcopy >> filecopy;
            system(filecopy.c_str());
        skree = skree->next;
    }

    

}

//     int currNodeSLLSize = 0;
//     singlyNode* traversalNode = searchComm(branchName, version)->head;
//     while(traversalNode != NULL){
//         traversalNode = traversalNode->next;
//         currNodeSLLSize += 1;
//     }
//     for (int i = 0; i < currNodeSLLSize; i++){
//         fstream targetFile = fstream();
//         ofstream currentFile = ofstream();
//         string targetFileName = ".minigit/" + branchName + "/" + branchName + "_" ;
//         if (version < 10){
//             targetFileName += "0" + to_string(version);
//         }
//         else{
//             targetFileName += to_string(version);
//         }
//         string fileNameNoType = "";
//         string fileType = "";
//         bool nextCharFileType = false;
//         for (int j = 0; j < sLLNodeAt(searchComm(branchName, version), i)->fileName.length(); j++){
//                 if (sLLNodeAt(searchComm(branchName, version), i)->fileName[j] == '.'){
//                     nextCharFileType = true;
//                 }
//                 if (!nextCharFileType){
//                     fileNameNoType += sLLNodeAt(searchComm(branchName, version), i)->fileName[j];
//                 }
//                 else{
//                     fileType += sLLNodeAt(searchComm(branchName, version), i)->fileName[j];
//                 }
//             }
//         string fiVer = sLLNodeAt(searchComm(branchName, version), i)->fileVersion;
//         targetFileName += fileNameNoType += "_";
//         string realName = fiVer.substr(0, fiVer.find("_"));
//         string fVer = fiVer.substr(fiVer.find("_")+1, fiVer.find("."));
//         int version = stoi(fVer)+1;
//         if (version < 10){
//             targetFileName += "0" + to_string(sLLNodeAt(searchComm(branchName, version), i)->fileVersion);
//         }
//         else{
//             targetFileName += to_string(sLLNodeAt(searchComm(branchName, version), i)->fileVersion);
//         }
//         targetFileName += fileType;
//         targetFile.open(targetFileName);
//         currentFile.open(sLLNodeAt(searchComm(branchName, version), i)->fileName);
//         string targetFileLine = "";
//         while (getline(targetFile, targetFileLine)){
//             currentFile << targetFileLine;
//         }
//     }
// }

void commitTree::createBranch(commitNode *par, string branchN, singlyNode* babyHead)
{
    
    
    commitNode * nw = new commitNode;
    nw->branchName= branchN;
    nw->current = true;
    nw->branched = false;
    nw->parent = par;
    nw->head = babyHead;
    // if(hashTab->searchHash(stringToInt(branchN)) != nullptr)
    // {
    //     cout << "Error: Pick a new branch name" << endl;

    //     return;
    // }
    

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
            par->childBranch[0]=nw;
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
                        fcopy << ".minigit/" << nw->branchName << "/" << nw->branchName << "_" << nw->commitNum << "/" << oldList->fileVersion;
                        fcopy >> filecopy;
                        system(filecopy.c_str());
                    }
                    else
                    {
                        fcopy << "cp " << makeFilePath(par, oldList->fileVersion) << " " ;
                        fcopy << ".minigit/" << nw->branchName << "/" << nw->branchName << "_" << nw->commitNum << "/" << nameVersioner(oldList->fileVersion);
                        fcopy >> filecopy;
                        system(filecopy.c_str());                        
                    }
                }
                newList = newList->next;
            }
            oldList = oldList->next;
        }
        while(oldList != nullptr)
        {
            bool missing = true;
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
                fcopy << ".minigit/" << nw->branchName << "/" << nw->branchName << "_" << nw->commitNum << "/" << oldList->fileVersion;
                fcopy >> filecopy;
                system(filecopy.c_str());
            }
            oldList = oldList->next;
        }

    }
    else
    {
        this->root = nw;
        nw->branchName= "main";


    }

    hashTab->insertHash(nw);
    this->currentBranch = nw;

}

commitTree::commitTree()
{
    hashTable *commHash = new hashTable(100);
    this->hashTab = commHash;
    this->root = nullptr;
    this->currentBranch = nullptr;
    createBranch(nullptr, "Main", nullptr);
}

commitTree::commitTree(singlyNode* head)
{
    hashTable *commHash = new hashTable(100);
    this->hashTab = commHash;
    this->root = nullptr;
    this->currentBranch = nullptr;
    createBranch(nullptr, "Main", head);

}

commitNode* commitTree::searchComm(string branchName, bool latest) //overloaded function that either gives the first or last node of a commit branch
{
    commitNode* curr = hashTab->searchHash(stringToInt(branchName)); //accesses a branch via hashtable
    if(latest == false)
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
    
    commitNode* curr = hashTab->searchHash(stringToInt(branchName));
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
        commitTree *gitTree = new commitTree(babyHead); //creates a new commitTree if there isn't one

    }
    if (root->head == nullptr)
    {
        root->head = babyHead;
        singlyNode * temp = babyHead;
        
        
        system("mkdir .minigit/main/main_00");
        while (temp != nullptr)
        {
            temp->fileVersion=nameVersioner(temp->fileName);
            stringstream fcopy;
            fcopy << "cp " << temp->fileName << " " << ".minigit/main/main_00/" << temp->fileVersion;
            string filecopy;
            fcopy >> filecopy;
            system(filecopy.c_str());
        }
        this->currentBranch=root;
        hashTab->insertHash(root);

    }
    
    else
    {
        
        createBranch(currentBranch, currentBranch->branchName, babyHead);

    }
}


void commitTree::pushCommit(singlyNode * babyHead, string branchName)
{
    
    
    if (root == nullptr)
    {
        commitTree(); //creates a new commitTree if there isn't one
    }
    if (root->head == nullptr)
    {
        root->head = babyHead;
        singlyNode * temp = babyHead;
        
        
        system("mkdir .minigit/main/main_00");
        while (temp != nullptr)
        {
            temp->fileVersion=nameVersioner(temp->fileName);
            stringstream fcopy;
            fcopy << "cp " << temp->fileName << " " << ".minigit/main/main_00/" << temp->fileVersion;
            string filecopy;
            fcopy >> filecopy;
            system(filecopy.c_str());
        }
        this->currentBranch=root;
        hashTab->insertHash(root);

    }
    else
    {
        this->currentBranch=searchComm(branchName, true);
        createBranch(currentBranch, currentBranch->branchName, babyHead);

    }
}

void commitTree::pushCommit(singlyNode * babyHead, string branchName, string newName)
{
    
    
    if (root == nullptr)
    {
        commitNode * main = new commitNode;
        root = main; //creates a new commitTree if there isn't one
    }
    if (root->head == nullptr)
    {
        root->head = babyHead;
        singlyNode * temp = babyHead;
        
        
        system("mkdir .minigit/main/main_00");
        while (temp != nullptr)
        {
            temp->fileVersion=nameVersioner(temp->fileName);
            stringstream fcopy;
            fcopy << "cp " << temp->fileName << " " << ".minigit/main/main_00/" << temp->fileVersion;
            string filecopy;
            fcopy >> filecopy;
            system(filecopy.c_str());
        }
        this->currentBranch=root;
        hashTab->insertHash(root);

    }
    else
    {
        this->currentBranch=searchComm(branchName, true);
        createBranch(currentBranch, newName, babyHead);

    }
}

bool commitTree::fileCompare(string targ, string curr) //requires filepath name for all non-local files
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


// void commitTree::commitFiles(string commitBranchName){
//     //go to commitBranchName
//     //check commit version
//     commitNode* currCommitNode = searchComm(commitBranchName, true);
//     int &currCommitNum = searchComm(commitBranchName, true)->commitNum;
//     //versionNum = the string version of the commitNum, adjusted with preceding 0s
//     string branchVersionNum;
//     if (currCommitNum < 10){
//         branchVersionNum = "0" + to_string(currCommitNum);
//     }
//     else{
//         branchVersionNum = to_string(currCommitNum);
//     }
//     //create the directory
//     filesystem::create_directory(".minigit/"+ commitBranchName + "/" + commitBranchName + "_" + branchVersionNum);
//     //creating the files
//     fstream currentFile = fstream();
//     ofstream newFile = ofstream();
//     string fileName;
//     string fileNameNoType;
//     string fileType;
//     string fileContent;
//     string fileVersionNum;

//     bool nextCharFileType = false;
//     bool sameFile = false;

//     int currNodeSLLSize = 0;
//     singlyNode* traversalNode = currCommitNode->head;
//     while(traversalNode != NULL){
//         traversalNode = traversalNode->next;
//         currNodeSLLSize += 1;
//     }
//     if (currCommitNum == 0){
//         for (int i = 0; i < currNodeSLLSize; i++){
//         fileName = sLLNodeAt(currCommitNode, i)->fileName;
//         fileNameNoType = "";
//         fileType = "";
//         nextCharFileType = false;
//         for (int j = 0; j < fileName.length(); j++){
//             if (fileName[j] == '.'){
//                 nextCharFileType = true;
//             }
//             if (!nextCharFileType){
//                 fileNameNoType += fileName[j];
//             }
//             else{
//                 fileType += fileName[j];
//             }
//         }
//         currentFile.open(fileName);
//         newFile.open(".minigit/" + commitBranchName + "/" + commitBranchName + "_" + branchVersionNum + "/" + fileNameNoType + "_00" + fileType);
//         if (currentFile.is_open() && newFile.is_open()){
//             while (getline(currentFile, fileContent)){
//                 newFile << fileContent;
//             }
//             newFile.close();
//             currentFile.close();
//         }
//         else{
//             cout << "Failed to open file." << endl;
//         }
//         }
//     }
//     else{
//         for (int i = 0; i < currNodeSLLSize; i++){
//             fileName = sLLNodeAt(currCommitNode, i)->fileName;
//             fileNameNoType = "";
//             fileType = "";
//             nextCharFileType = false;
//             for (int j = 0; j < fileName.length(); j++){
//                 if (fileName[j] == '.'){
//                     nextCharFileType = true;
//                 }
//                 if (!nextCharFileType){
//                     fileNameNoType += fileName[j];
//                 }
//                 else{
//                     fileType += fileName[j];
//                 }
//             }
//             if (sLLNodeAt(currCommitNode, i)->fileVersion < 10){
//                 fileVersionNum = "0" + to_string(sLLNodeAt(currCommitNode, i)->fileVersion);
//             }
//             else{
//                 fileVersionNum = to_string(sLLNodeAt(currCommitNode, i)->fileVersion);
//             }
//             sameFile = compareFiles(sLLNodeAt(currCommitNode, i)->fileName, fileNameNoType + "_" + fileVersionNum);
//             if (!sameFile){
//                 sLLNodeAt(currCommitNode, i)->fileVersion += 1;
//             }
//             currentFile.open(fileName);
            
//             newFile.open(".minigit/" + commitBranchName + "/" + commitBranchName + "_" + branchVersionNum + "/" + fileNameNoType + "_" + fileVersionNum + fileType);
//             if (currentFile.is_open() && newFile.is_open()){
//                 while (getline(currentFile, fileContent)){
//                     newFile << fileContent;
//                 }
//                 newFile.close();
//                 currentFile.close();
//             }
//             else{
//                 cout << "Failed to open file." << endl;
//             }
//         }
//     }

//     currCommitNum++;
// }

// void printHelper(commitNode* root){
//     for (int i = 0; i < root->childBranch.size(); i++){
//         if (root->childBranch.at(i) != NULL){
//             printHelper(root->childBranch.at(i));
//         }
//     }
//     singlyNode* sNode = root->head;
//     cout << "branch name: " << root->branchName << endl;
//     cout << "branch version: " << root->commitNum << endl;
//     int sNodeNum = 1;
//     while (sNode != NULL){
//         cout << "--" << sNodeNum << "th file name: " << sNode->fileName << endl;
//         cout << "--" << sNodeNum << "th max file version: " << sNode->fileVersion << endl;
//         sNodeNum += 1;
//         sNode = sNode->next;
//     }
// }

// void commitTree::print(){
//     printHelper(root);
// }

#endif
