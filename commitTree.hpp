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
    commitNode * target;
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



};

class hashTable
{

    int tableSize;  // No. of buckets (linked lists)

    // Pointer to an array containing buckets
    commitNode* *table;

public:
    HashTable(int bsize);  // Constructor

    // inserts a key into hash table
    bool insertItem(int key);

    // hash function to map values to key
    unsigned int hashFunction(int key);

    unsigned int stringToInt(string name);

    void printTable();

    commitNode* searchItem(int key);

};

#endif