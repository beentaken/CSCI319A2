#ifndef CHORDNODE_H
#define	CHORDNODE_H
#include <map>
#include <iostream>
#include <string>
#include "FingerTableRow.h"

class ChordNode {
    friend std::ostream& operator<<(std::ostream&, const ChordNode &);
public:
    ChordNode(const ChordNode &);
    ChordNode(int, int);
    ~ChordNode();
    void AddPeer(int);
    void RemovePeer(int);
    int FindKey(std::string);
    ChordNode *FindNode(int);
    ChordNode *FindSuccessor(int);
    ChordNode *UpdateNodes(ChordNode *);
    int Hash(std::string);
    void Insert(std::string);
    void Delete(std::string);
    void Print(int);
    int GetSize();
    int GetID();
    ChordNode *GetSuccessor();
    ChordNode *GetPredeccessor();
    FingerTableRow GetFingerTableRow(int);
    void BuildFingerTable(ChordNode *);

private:
    int ID;
    int size;
    ChordNode *successor;
    FingerTableRow *fingerTable;
    std::map <int, std::string> data;
};

#endif	/* CHORDNODE_H */

