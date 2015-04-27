/******************************************************************************
 * Mark Thompson                                                              *
 * Student #: 4419431                                                         *
 * CSCI319 Distributed Systems - Assignment 2 (Chord Simulator)               *
 *****************************************************************************/

#include <cstdlib>
#include <cmath>
#include <vector>
#include "ChordNode.h"
using namespace std;

ChordNode::ChordNode(int ID, int ftSize) {
    this->ID = ID;
    this->ftSize = ftSize;
    this->chordSize = pow(2, ftSize);
    this->data = std::map<int, std::string>();
    this->fingerTable = new FingerTableRow[ftSize];
    for (int i = 0; i < this->ftSize; i++) {
        this->fingerTable[i].hop = (ID + (int) pow(2, i)) % this->chordSize;
        this->fingerTable[i].successorID = ID;
        this->fingerTable[i].successorNode = NULL;
    }
}

ChordNode::~ChordNode() {
    if (this->fingerTable != NULL) {
        delete [] this->fingerTable;
    }
    this->fingerTable = NULL;
    if (!this->data.empty()) {
        this->data.clear();
    }
}

void ChordNode::InitChord(int size, ChordNode *&chordPtr) {
    if (chordPtr != NULL) {
        ChordNode *next;
        int startID = chordPtr->ID;
        while (chordPtr->fingerTable[0].successorID != startID) {
            next = chordPtr->fingerTable[0].successorNode;
            delete chordPtr;
            chordPtr = next;
            if (chordPtr == NULL) {
                break;
            }
        }
    }
    ChordNode *node = new ChordNode(0, size);
    for (int i = 0; i < size; i++) {
        node->fingerTable[i].successorID = 0;
        node->fingerTable[i].successorNode = node;
    }
    chordPtr = node;
    cout << "Mark Thompson" << endl << "4419431" << endl;
}

void ChordNode::AddPeer(int ID, ChordNode *chordPtr) {
    ChordNode *newNode = new ChordNode(ID, this->ftSize);
    ChordNode *succ = this->FindKey(newNode->fingerTable[0].hop, this->ID, true, chordPtr);
    newNode->fingerTable[0].successorID = succ->ID;
    newNode->fingerTable[0].successorNode = succ;
    map<int, std::string>::iterator it = succ->data.begin();
    map<int, std::string>::iterator newit = succ->data.upper_bound(ID);
    if (it != succ->data.end()) {
        while (it != newit) {
            newNode->data.insert(pair<int, std::string>(it->first, it->second));
            it++;
        }
        succ->data.erase(succ->data.begin(), newit);
    }
    FixFingerTablesAfterAdd(this->ID, newNode);
    for (int i = 1; i < this->ftSize; i++) {
        ChordNode *succ = this->FindKey(newNode->fingerTable[i].hop, this->ID, false, chordPtr);
        newNode->fingerTable[i].successorID = succ->ID;
        newNode->fingerTable[i].successorNode = succ;
    }
    cout << "PEER " << newNode->ID << " ADDED" << endl;
}

void ChordNode::RemovePeer(int ID, ChordNode *&chordPtr) {
    ChordNode *node = this->FindKey(ID, this->ID, true, chordPtr);
    ChordNode *succ = this->FindKey(ID + 1, this->ID, false, chordPtr);
    if (node->ID == succ->ID) {
        delete chordPtr;
        chordPtr = NULL;
        return;
    }
    map<int, std::string>::iterator it = node->data.begin();
    while (it != node->data.end()) {
        succ->data.insert(pair<int, std::string>(it->first, it->second));
        it++;
    }
    int deleteID = node->ID;
    delete node;
    node = succ;
    if (chordPtr->ID == ID) {
        chordPtr = succ;
        chordPtr->FixFingerTablesAfterRemove(deleteID, node);
    } else {
        this->FixFingerTablesAfterRemove(deleteID, node);
    }
    cout << "PEER " << ID << " REMOVED" << endl;
}

ChordNode *ChordNode::FindKey(int ID, int startID, bool print, ChordNode *chordPtr) {
    int origID = this->ID;
    this->ZeroFingerTable();
    int shiftedID = this->ShiftID(ID, origID);
    int i = this->ftSize - 1;
    while (i >= 0 && shiftedID <= this->fingerTable[i].successorID) {
        i--;
    }

    if (i == this->ftSize - 1) {
        if (this->fingerTable[i].successorID == this->chordSize) {
            this->UnZeroFingerTable(origID);
            if (this->ID != startID && print) {
                cout << this->ID << endl;
            }
            return this;
        }
        this->UnZeroFingerTable(origID);
        if (print) {
            cout << this->ID << ">";
        }
        return this->fingerTable[i].successorNode->FindKey(ID, startID, print, this->fingerTable[i].successorNode);
    }
    if (i == -1) {
        this->UnZeroFingerTable(origID);
        if (print) {
            cout << this->ID << ">" << this->fingerTable[0].successorNode->ID << endl;
        }
        return this->fingerTable[0].successorNode;
    }
    this->UnZeroFingerTable(origID);
    if (print) {
        cout << this->ID << ">";
    }
    return this->fingerTable[i].successorNode->FindKey(ID, startID, print, this->fingerTable[i].successorNode);
}

int ChordNode::Hash(std::string data) {
    int key = 0;
    for (int i = 0; i < data.length(); i++) {
        key = ((key << 5) + key) ^ data[i];
    }
    return key & (chordSize - 1);
}

void ChordNode::Insert(std::string data, ChordNode *chordPtr) {
    int hash = this->Hash(data);
    ChordNode *node = this->FindKey(hash, this->ID, true, chordPtr);
    node->data.insert(pair<int, string>(hash, data));
    cout << "INSERTED " << data << " (key=" << hash << ") AT " << node->ID << endl;
}

void ChordNode::Delete(std::string data, ChordNode *chordPtr) {
    int hash = this->Hash(data);
    ChordNode *node = this->FindKey(hash, this->ID, true, chordPtr);
    map<int, std::string>::iterator startit, endit;
    pair<map<int, string>::iterator, map<int, std::string>::iterator> key(startit, endit);
    key = node->data.equal_range(hash);
    if (key.first != node->data.end()) {
        while (key.first != key.second && key.first->second != data) {
            key.first++;
        }
        if ((key.first != key.second) || (key.first == key.second &&
                key.second != node->data.end())) {
            cout << "REMOVED " << data << " (key=" << hash << ") FROM " << node->ID << endl;
            node->data.erase(key.first);
        }
    }
}

void ChordNode::Print(int ID, ChordNode *chordPtr) {
    ChordNode *node = this->FindKey(ID, this->ID, false, chordPtr);
    cout << "DATA AT INDEX NODE " << node->ID << ':';
    map<int, std::string>::const_iterator it = node->data.begin();
    while (it != node->data.end()) {
        cout << endl << it->second;
        it++;
    }
    cout << endl << "FINGER TABLE OF NODE " << node->ID << ":" << endl;
    for (int i = 0; i < node->ftSize; i++) {
        cout << node->fingerTable[i].successorID << " ";
    }
    cout << endl;
}

void ChordNode::FixFingerTablesAfterAdd(int startID, ChordNode *node) {
    int origID = this->ID;
    this->ZeroFingerTable();
    int shiftedID = this->ShiftID(node->ID, origID);
    int i = 0;
    while (i < this->ftSize && this->fingerTable[i].hop <= shiftedID) {
        if (this->fingerTable[i].successorID >= shiftedID) {
            this->fingerTable[i].successorID = shiftedID;
            this->fingerTable[i].successorNode = node;
        }
        i++;
    }
    this->UnZeroFingerTable(origID);
    if (this->fingerTable[0].successorID != startID) {
        this->fingerTable[0].successorNode->FixFingerTablesAfterAdd(startID, node);
    }
}

void ChordNode::FixFingerTablesAfterRemove(int deleteID, ChordNode *node) {
    int origID = this->ID;
    this->ZeroFingerTable();
    int shiftedID = this->ShiftID(deleteID, origID);
    int succShiftedID = this->ShiftID(node->ID, origID);
    int i = 0;
    while (i < this->ftSize) {
        if (this->fingerTable[i].successorID == shiftedID) {
            this->fingerTable[i].successorID = succShiftedID;
            this->fingerTable[i].successorNode = node;
        }
        i++;
    }
    this->UnZeroFingerTable(origID);
    if (this->fingerTable[0].successorID != node->ID) {
        this->fingerTable[0].successorNode->FixFingerTablesAfterRemove(deleteID, node);
    }
}

void ChordNode::ZeroFingerTable() {
    int origID = this->ID;
    this->ID = 0;
    for (int i = 0; i < this->ftSize; i++) {
        this->fingerTable[i].hop = pow(2, i);
        if (this->fingerTable[i].successorID <= origID) {
            this->fingerTable[i].successorID += this->chordSize;
        }
        this->fingerTable[i].successorID -= origID;
    }
}

void ChordNode::UnZeroFingerTable(int origId) {
    this->ID = origId;
    for (int i = 0; i < this->ftSize; i++) {
        this->fingerTable[i].hop = (this->fingerTable[i].hop + origId) % this->chordSize;
        this->fingerTable[i].successorID = (this->fingerTable[i].successorID + origId) % this->chordSize;
    }
}

int ChordNode::ShiftID(int ID, int shift) {
    if (ID <= shift) {
        ID += this->chordSize;
    }
    return ID - shift;
}