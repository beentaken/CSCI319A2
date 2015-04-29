/******************************************************************************
 * Mark Thompson                                                              *
 * Student #: 4419431                                                         *
 * CSCI319 Distributed Systems - Assignment 2 (Chord Simulator)               *
 *****************************************************************************/

#ifndef CHORDNODE_H
#define	CHORDNODE_H
#include <map>
#include <iostream>
#include <string>
#include "FingerTableRow.h"

class ChordNode {
public:
    ChordNode(int, int);
    ~ChordNode();
    void InitChord(int, ChordNode *&);
    void AddPeer(int, ChordNode *&);
    void RemovePeer(int, ChordNode *&);
    ChordNode *FindKey(int, int, bool, ChordNode *&);
    int Hash(std::string);
    void Insert(std::string, ChordNode *&);
    void Delete(std::string, ChordNode *&);
    void Print(int, ChordNode *&);

private:
    int ID, ftSize, chordSize;
    FingerTableRow *fingerTable;
    std::map <int, std::string> data;

    void FixFingerTablesAfterAdd(int, ChordNode *&);
    void FixFingerTablesAfterRemove(int, ChordNode *&);
    void ZeroFingerTable();
    void UnZeroFingerTable(int);
    int ShiftID(int, int);
};

#endif	/* CHORDNODE_H */

