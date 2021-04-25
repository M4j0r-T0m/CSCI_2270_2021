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
    public:
        commitTree();
        ~commitTree();
        void addFile(string fileName);
        void removeFile(string fileName);
        void commitFile(string fileName);
        void createBranch(commitNode parent);
        void fileCompare(string fileName);
        void gitStatus();
        void pullToVer(string branchName, string target);
        void pullToMain(string branchName);
        static unsigned int stringToInt(string name);


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