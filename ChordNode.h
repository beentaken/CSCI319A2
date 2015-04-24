#ifndef CHORDNODE_H
#define	CHORDNODE_H
#include <map>
#include <iostream>
#include <string>
#include "FingerTableElement.h"

class ChordNode {
public:
    ChordNode(int ID);
    ChordNode(int ID, int size);
    void AddPeer(int ID);
    void RemovePeer(int ID);
    int FindKey(std::string key);
    int Hash(std::string data);
    void Insert(std::string data);
    void Delete(std::string data);
    void Print(int ID);
    int GetSize();
    int GetID();
    ChordNode *GetSuccessor();
    ChordNode *GetPredeccessor();
    FingerTableElement GetFingerTable(int index);

private:
    int ID;
    int size;
    ChordNode *predecessor, *successor;
    FingerTableElement *fingerTable;
    std::map <std::string, std::string> *data;
};

#endif	/* CHORDNODE_H */

