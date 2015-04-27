#include <cstdlib>
#include <cmath>
#include <vector>
#include "ChordNode.h"
using namespace std;

ostream& operator<<(ostream& out, const ChordNode &node) {
    out << "DATA AT INDEX NODE " << node.ID << ':';
    map<int, std::string>::const_iterator it = node.data.begin();
    while (it != node.data.end()) {
        cout << '\n' << it->second;
        it++;
    }
    out << "\nFINGER TABLE OF NODE " << node.ID << ":\n";
    for (int i = 0; i < node.ftSize; i++) {
        out << node.fingerTable[i].successorID << " ";
    }
    return out;
}

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
    //delete successor;
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

void ChordNode::AddPeer(int ID, ChordNode *&chordPtr) {
    ChordNode *newNode = new ChordNode(ID, this->ftSize);
    ChordNode *succ = this->FindSuccessor(newNode->fingerTable[0].hop, this->ID, chordPtr);
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
    chordPtr->FixFingerTables(chordPtr->ID, newNode);
    for (int i = 1; i < this->ftSize; i++) {
        ChordNode *succ = this->FindSuccessor(newNode->fingerTable[i].hop, this->ID, chordPtr);
        newNode->fingerTable[i].successorID = succ->ID;
        newNode->fingerTable[i].successorNode = succ;
    }
    cout << endl << "PEER " << newNode->ID << " ADDED" << endl;
}

void ChordNode::RemovePeer(int ID, ChordNode *&chordPtr) {
    ChordNode *node = FindNode(ID, this->ID, chordPtr);
    delete node;

    cout << endl << "PEER " << ID << " REMOVED" << endl;
}

ChordNode *ChordNode::FixFingerTables(int startID, ChordNode *&newNode) {
    int origID = this->ID;
    int shiftedID = this->ShiftID(newNode->ID, origID);
    this->ZeroFingerTable();
    int i = 0;
    while (i < this->ftSize && this->fingerTable[i].hop <= shiftedID) {
        if (this->fingerTable[i].successorID >= shiftedID) {
            this->fingerTable[i].successorID = shiftedID;
            this->fingerTable[i].successorNode = newNode;
        }
        i++;
    }
    this->UnZeroFingerTable(origID);
    if (this->fingerTable[0].successorID != startID) {
        return fingerTable[0].successorNode->FixFingerTables(startID, newNode);
    }
    return this;
}

void ChordNode::Insert(std::string data, ChordNode *&chordPtr) {
    int hashkey = Hash(data);
    ChordNode *node = FindKey(hashkey, chordPtr);
    node->data.insert(pair<int, string>(hashkey, data));
    cout << "INSERTED " << data << " (key=" << hashkey << ") AT " << node->ID << endl;
}

void ChordNode::Delete(std::string data, ChordNode *&chordPtr) {
    int hashkey = Hash(data);
    ChordNode *node = FindKey(hashkey, chordPtr);
    node->data.erase(hashkey);
    cout << "REMOVED " << data << " (key=" << hashkey << ") FROM " << node->ID << endl;
}

ChordNode *ChordNode::FindKey(int key, ChordNode *&chordPtr) {
    return FindSuccessor(key, this->ID, chordPtr);
}

ChordNode *ChordNode::FindNode(int ID, int startID, ChordNode *&chordPtr) {
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
            if (this->ID != startID)
                cout << this->ID << endl;
            return this;
        }
        this->UnZeroFingerTable(origID);
        cout << this->ID << ">";
        return this->fingerTable[i].successorNode->FindNode(ID, startID, this->fingerTable[i].successorNode);
    }
    if (i == -1) {
        this->UnZeroFingerTable(origID);
        if (this->ID != startID)
            cout << this->ID << endl;
        return this->fingerTable[0].successorNode;
    }
    this->UnZeroFingerTable(origID);
    cout << this->ID << ">";
    this->fingerTable[i].successorNode;
    return this->fingerTable[i].successorNode->FindNode(ID, startID, chordPtr);
}

ChordNode *ChordNode::FindSuccessor(int ID, int startID, ChordNode *&chordPtr) {
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
            if (this->ID != startID)
                cout << this->ID << endl;
            return this;
        }
        this->UnZeroFingerTable(origID);
        cout << this->ID << ">";
        return this->fingerTable[i].successorNode->FindSuccessor(ID, startID, this->fingerTable[i].successorNode);
    }
    if (i == -1) {
        this->UnZeroFingerTable(origID);
        if (this->ID != startID)
            cout << this->ID << endl;
        return this->fingerTable[0].successorNode;
    }
    this->UnZeroFingerTable(origID);
    cout << this->ID << ">";
    return this->fingerTable[i].successorNode->FindSuccessor(ID, startID, this->fingerTable[i].successorNode);
}

void ChordNode::ZeroFingerTable() {
    int origID = this->ID;
    this->ID = 0;
    for (int i = 0; i < this->ftSize; i++) {
        this->fingerTable[i].hop = 1 << i;
        if (this->fingerTable[i].successorID <= origID) {
            this->fingerTable[i].successorID |= this->chordSize;
        }
        this->fingerTable[i].successorID -= origID;
    }
}

void ChordNode::UnZeroFingerTable(int origId) {
    this->ID = origId;
    for (int i = 0; i < this->ftSize; i++) {
        this->fingerTable[i].hop = (this->fingerTable[i].hop + origId) & ~chordSize;
        this->fingerTable[i].successorID = (this->fingerTable[i].successorID + origId) & ~chordSize;
    }
}

int ChordNode::ShiftID(int ID, int shift) {
    if (ID <= shift) {
        ID |= this->chordSize;
    }
    return ID - shift;
}

int ChordNode::Hash(std::string data) {
    int key = 0;
    for (int i = 0; i < data.length(); i++) {
        key = ((key << 5) + key) ^ data[i];
    }
    return key & (chordSize - 1);
}

void ChordNode::Print(int ID, ChordNode *&chordPtr) {
    cout << *FindKey(ID, chordPtr) << endl;
}