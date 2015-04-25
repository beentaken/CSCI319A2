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
    ChordNode *ClosestPrecedingNode(int);
    ChordNode *FindSuccessor(int);
    ChordNode *UpdateNodes(ChordNode *);
    void FixFingerTable(ChordNode *);
    void Stabilize();
    void Notify(ChordNode *);
    int Hash(std::string);
    void Insert(std::string);
    void Delete(std::string);
    bool InsideRange(int, int, int);
    void Print(int);
    int GetSize();
    int GetID();

    ChordNode *GetPredecessor() {
        return predecessor;
    }

    ChordNode *GetSuccessor() {
        return successor;
    };
    FingerTableRow GetFingerTableRow(int);

private:
    int ID;
    int size;
    ChordNode *predecessor, *successor;
    FingerTableRow *fingerTable;
    std::map <int, std::string> data;
};

#endif	/* CHORDNODE_H */

