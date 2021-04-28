#ifndef COMMITTREE_HPP
#define COMMITTREE_HPP
#include <vector>
#include <iostream>

using namespace std;

struct singlyNode
{
    string fileName;
    int fileVersion;
    singlyNode * next;
};

struct commitNode
{
    vector<commitNode*> childBranch;
    commitNode * parent;
    string branchName;
    int commitNum;
    singlyNode* head; 
    bool branched;
    bool current; 
};

struct hashNode
{
    int key;
    struct hashNode * next;
    struct commitNode * target;
};

class commitTree
{
    private:
        commitNode * root;
    public:
        //commitTree();
        commitTree(singlyNode* head);
        //~commitTree();
        void addFile(string fileName); 
        void removeFile(string fileName);
        bool compareFiles(string targ, string curr);//compares two files named targ and curr
        void commitFiles(string commitBranchName);//makes copies of all files named in SLL into new folder inside commitBranchName folder
        void pushCommit(commitNode * curr, string branchName);
        void checkout(string branchName, int version);//checks out all the files in the SLL from folder branchName_version
        void createBranch(commitNode* parent, string branchName, singlyNode* head);//creates a branch from parent named branchName with new head;
        void gitStatus();
        void pullToVer(string branchName, string target); //merges branches
        void pullToMain(string branchName);
        static unsigned int stringToInt(string name);
        commitNode* searchComm(string branchName, bool latest); //finds commitNode* of the latest version in folder branchName
        commitNode* searchComm(string branchName, int version); //finds commitNode* of branchName_version
        singlyNode* sLLNodeAt(commitNode* node, int index);

        void print();


};

class hashTable
{
    private:
        int tableSize;  // No. of buckets (linked lists)

        // Pointer to an array containing buckets
        commitNode* *table;

    public:
        hashTable(int bsize);  // Constructor

        // inserts a key into hash table
        bool insertHash(commitNode* target);

        // hash function to map values to key
        unsigned int hashFunction(int key);

        

        void printTable();

        commitNode* searchHash(int key);

};

#endif