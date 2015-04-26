#ifndef CHORDNODE_H
#define	CHORDNODE_H
#include <map>
#include <iostream>
#include <string>
#include "FingerTableRow.h"

class ChordNode {
    friend std::ostream& operator<<(std::ostream&, const ChordNode &);
public:
    ChordNode(int, int);
    ~ChordNode();
    void AddPeer(int);
    void RemovePeer(int);
    ChordNode *FindKey(int);
    ChordNode *FindNode(int);
    ChordNode *FindSuccessor(int, int, ChordNode *);
    ChordNode *FixFingerTables(int, ChordNode *);
    int Hash(std::string);
    void Insert(std::string);
    void Delete(std::string);
    bool InsideRange(int, int, int);
    void Print(int);

    ChordNode *GetSuccessor() {
        return this->fingerTable[0].successorNode;
    };

private:
    int ID;
    int ftSize, chordSize;
    FingerTableRow *fingerTable;
    std::map <int, std::string> data;
};

#endif	/* CHORDNODE_H */

