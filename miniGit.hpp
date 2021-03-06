#ifndef COMMITTREE_HPP
#define COMMITTREE_HPP
#include<vector>
#include<iostream>
using namespace std;

struct singlyNode
{
    string fileName;
    string fileVersion;
    singlyNode * next;
};

struct commitNode
{
    vector<commitNode*> childBranch;
    commitNode * parent;
    string branchName;
    int commitNum;
    singlyNode * head;
    bool branched;
    bool current; 
};


struct hashNode
{
    int key;
    struct hashNode * next;
    struct commitNode * target;
};

class hashTable
{
    private:
        int tableSize;  // No. of buckets (linked lists)

        // Pointer to an array containing buckets
        hashNode* *table;

    public:
        hashTable(int bsize);  // Constructor

        // inserts a key into hash table
        bool insertHash(commitNode *targ);

        // hash function to map values to key
        unsigned int hashFunction(int key);

        

        void printTable();

        commitNode* searchHash(int key);

};

class commitTree
{
    private:
        commitNode * root;
        
        hashTable * hashTab;

    public:
        commitNode * currentBranch;
        commitTree();
        commitTree(singlyNode* head);
        ~commitTree();
        
        void addFile(string fileName, singlyNode* prev);
        void removeFile(string fileName);
        void removeFile(string fileName, commitNode* curr);
        //void commitFile(string fileName);
        void pushCommit(singlyNode * babyHead);
        void pushCommit(singlyNode * babyHead, string branchName);
        void pushCommit(singlyNode * babyHead, string branchName, string newName);
        void checkout(string branchName, int version); //checks out all files found given a branchName and a version
        void createBranch(commitNode *par, string branchN, singlyNode* babyHead);
        
        bool fileCompare(string targ, string curr);
        void gitStatus();
        void pullToVer(string branchName, string target);
        void pullToVer(string branchName);
        static unsigned int stringToInt(string name);
        commitNode* searchComm(string branchName, bool latest);
        commitNode* searchComm(string branchName, int version);
        singlyNode* sLLNodeAt(commitNode* node, int index);

};


#endif