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
    ChordNode *FindKey(int);
    ChordNode *FindNode(int);
    ChordNode *ClosestPrecedingNode(int);
    ChordNode *FindSuccessor(int);
    //ChordNode *FindSuccessor(int, int, ChordNode *);
    void FixFingerTables(ChordNode *);
    int Hash(std::string);
    void Insert(std::string);
    void Delete(std::string);
    void DeleteAll(ChordNode *&);
    bool InsideRange(int, int, int);
    void Print(int);
    FingerTableRow GetFingerTableRow(int);

private:
    int ID;
    int ftSize, chordSize;
    ChordNode *predecessor, *successor;
    FingerTableRow *fingerTable;
    std::map <int, std::string> data;
};

#endif	/* CHORDNODE_H */