#ifndef COMMITTREE_HPP
#define COMMITTREE_HPP
#include<vector>
#include<iostream>
using namespace std;



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

struct singlyNode
{
    string fileName;
    string fileVersion;
    singlyNode * next;
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
        commitNode * currentBranch;
        
    public:
        commitTree();
        ~commitTree();
        void addFile(string fileName);
        void removeFile(string fileName);
        //void commitFile(string fileName);
        void pushCommit(commitNode * curr, string branchName)
        void createBranch(commitNode *par, string branchN, singlyNode* babyHead);
        bool fileCompare(string targ, string curr);
        void gitStatus();
        void pullToVer(string branchName, string target);
        void pullToVer(string branchName);
        static unsigned int stringToInt(string name);
        commitNode* searchComm(string branchName, bool latest);
        commitNode* searchComm(string branchName, int version);


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
        bool insertHash(string name);

        // hash function to map values to key
        unsigned int hashFunction(int key);

        

        void printTable();

        commitNode* searchHash(int key);

};

#endif