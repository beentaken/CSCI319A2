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
    void InitChord(int, ChordNode *&);
    void AddPeer(int, ChordNode *&);
    void RemovePeer(int, ChordNode *&);
    ChordNode *FindKey(int, ChordNode *&);
    ChordNode *FindNode(int, ChordNode *&);
    ChordNode *FindSuccessor(int, int, ChordNode *&);
    ChordNode *FixFingerTables(int, ChordNode *&);
    int Hash(std::string);
    void Insert(std::string, ChordNode *&);
    void Delete(std::string, ChordNode *&);
    bool InsideRange(int, int, int);
    void Print(int, ChordNode *&);

    ChordNode *GetSuccessor() {
        return this->fingerTable[0].successorNode;
    };

private:
    int ID;
    int ftSize, chordSize;
    ChordNode *predecessor, *successor;
    FingerTableRow *fingerTable;
    std::map <int, std::string> data;
};

#endif	/* CHORDNODE_H */

