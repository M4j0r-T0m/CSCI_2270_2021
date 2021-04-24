#ifndef COMMITTREE_CPP
#define COMMITTREE_CPP
#include "commitTree.hpp"
#include<vector>
#include<iostream>
#include<sstream>

using namespace std;

static unsigned int commitTree::stringToInt(string name);

hashNode* HashTable::createNode(int key, hashNode* next)
{
    commitNode* nw = new commitNode;
    nw->key = key;
    nw->next = next;
    return nw;
}

HashTable::HashTable(int bsize)
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
        node * temp = table[i];
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



#endif