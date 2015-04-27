#ifndef CHORDNODE_H
#define	CHORDNODE_H
#include <map>
#include <iostream>
#include <string>
#include "FingerTableRow.h"

class ChordNode {
    friend std::ostream& operator<<(std::ostream&, const ChordNode&);
public:
    ChordNode(int, int);
    ~ChordNode();
    void InitChord(int, ChordNode *&);
    void AddPeer(int, ChordNode *&);
    void RemovePeer(int, ChordNode *&);
    ChordNode *FindKey(int, int, ChordNode *&);
    int Hash(std::string);
    void Insert(std::string, ChordNode *&);
    void Delete(std::string, ChordNode *&);
    void Print(int, ChordNode *&);

    ChordNode *GetSuccessor() {
        return this->fingerTable[0].successorNode;
    };

private:
    int ID, ftSize, chordSize;
    FingerTableRow *fingerTable;
    std::map <int, std::string> data;

    void FixFingerTables(int, ChordNode *);
    void ZeroFingerTable();
    void UnZeroFingerTable(int);
    int ShiftID(int, int);
};

#endif	/* CHORDNODE_H */

