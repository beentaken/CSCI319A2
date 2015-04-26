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
    this->predecessor = this->successor = this;
    this->data = std::map<int, std::string>();
    this->fingerTable = new FingerTableRow[ftSize];
    for (int i = 0; i < this->ftSize; i++) {
        this->fingerTable[i].hop = (ID + (int) pow(2, i)) % this->chordSize;
        this->fingerTable[i].successorID = ID;
        this->fingerTable[i].successorNode = this;
    }
}

ChordNode::~ChordNode() {
    if (fingerTable != NULL) {
        delete [] fingerTable;
    }
    fingerTable = NULL;
    if (!data.empty()) {
        data.clear();
    }
}

void ChordNode::InitChord(int size, ChordNode *&chordPtr) {
    if (chordPtr != NULL) {
        ChordNode *next;
        int startID = chordPtr->ID;
        while (chordPtr->successor->ID != startID) {
            next = chordPtr->successor;
            delete chordPtr;
            chordPtr = next;
            if (chordPtr == NULL) {
                break;
            }
        }
    }
    ChordNode *node = new ChordNode(0, size);
    chordPtr = node;
    cout << "Mark Thompson" << endl << "4419431" << endl;
}

void ChordNode::AddPeer(int ID, ChordNode *&chordPtr) {
    /*
    
    ChordNode *newNode = new ChordNode(ID, this->ftSize);
    ChordNode *succ = this->FindSuccessor(newNode->fingerTable[0].hop, ID, this);
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
    this->FixFingerTables(ID, newNode);
    for (int i = 1; i < this->ftSize; i++) {
        succ = this->FindSuccessor(newNode->fingerTable[i].hop, ID, this);
        newNode->fingerTable[i].successorID = succ->ID;
        newNode->fingerTable[i].successorNode = succ;
    }
     */
    ChordNode *newNode = new ChordNode(ID, this->ftSize);
    if (this->fingerTable[0].successorID == this->ID) { // If this is the second node to be added to network
        newNode->predecessor = this;
        newNode->successor = this;
        this->predecessor = newNode;
        this->successor = newNode;
        for (int i = 0; i < this->ftSize; i++) {
            newNode->fingerTable[i].successorID = 0;
            newNode->fingerTable[i].successorNode = this;
            this->fingerTable[i].successorID = ID;
            this->fingerTable[i].successorNode = newNode;
        }
    } else {
        ChordNode *succ = FindSuccessor(newNode->fingerTable[0].hop, this->ID, chordPtr);
        newNode->fingerTable[0].successorID = succ->ID;
        newNode->fingerTable[0].successorNode = succ;
        newNode->predecessor = succ->predecessor;
        newNode->predecessor->successor = newNode;
        succ->predecessor = newNode;
        newNode->successor = succ;
        for (int i = 1; i < this->ftSize; i++) {
            succ = FindSuccessor(newNode->fingerTable[i].hop, this->ID, chordPtr);
            newNode->fingerTable[i].successorID = succ->ID;
            newNode->fingerTable[i].successorNode = succ;
        }
        FixFingerTables(this->ID, newNode);
    }
    cout << endl << "PEER " << newNode->ID << " ADDED" << endl;
}

void ChordNode::RemovePeer(int ID, ChordNode *&chordPtr) {
    ChordNode *node = FindNode(ID, chordPtr);
    delete node;

    cout << endl << "PEER " << ID << " REMOVED" << endl;
}

ChordNode *ChordNode::FixFingerTables(int startID, ChordNode *&chordPtr) {
    int originalId, convertNewId, newNodeId;
    newNodeId = chordPtr->ID;
    int index = 0;
    originalId = this->ID;
    while (index < this->ftSize && this->fingerTable[index].hop <= convertNewId) {
        if (this->fingerTable[index].successorID >= convertNewId) {
            this->fingerTable[index].successorID = convertNewId;
            this->fingerTable[index].successorNode = chordPtr;
        }
        index++;
    }
    if (this->fingerTable[0].successorID != startID)
        return this->fingerTable[0].successorNode->FixFingerTables(startID, chordPtr);
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

bool ChordNode::InsideRange(int ID, int start, int finish) {
    int maxID = this->chordSize;
    int minID = 0;

    return (start < finish && start <= ID && ID <= finish) ||
            (start > finish && ((start <= ID && ID <= maxID) || (minID <= ID && ID <= finish))) ||
            ((start == finish) && (ID == start));
}

ChordNode *ChordNode::FindKey(int key, ChordNode *&chordPtr) {
    return FindSuccessor(key, this->ID, chordPtr);
}

ChordNode *ChordNode::FindNode(int ID, ChordNode *&chordPtr) {
    return NULL;
}

ChordNode *ChordNode::FindSuccessor(int ID, int startID, ChordNode *&chordPtr) {
    if (InsideRange(ID, chordPtr->ID, chordPtr->successor->ID)) {
        if (ID != startID) {
            cout << chordPtr->ID << ">";
        }
        return chordPtr->successor;
    } else {
        for (int i = chordPtr->ftSize - 1; i >= 0; i++) {
            if (ID <= this->fingerTable[i].successorID) {
                return this->fingerTable[i].successorNode->FindSuccessor(ID, startID, chordPtr);
            }
        }
        if (ID != startID) {
            cout << chordPtr->ID << ">";
        }
        return this->successor->FindSuccessor(ID, startID, chordPtr);
    }

    /*
    if (i == ftSize - 1) {
        if (fingerTable[i].successorID == this->chordSize) {
            if (this->ID != startID)
                cout << this->ID << '\n';
            return this;
        }
        cout << this->ID << '>';
        node = this->fingerTable[i].successorNode;
        return node->fingerTable[i].successorNode->FindSuccessor(ID, startID, node);
    }
    if (i == -1) {
        if (this->ID != startID)
            cout << this->ID << '\n';
        return this->fingerTable[0].successorNode;
    }
    cout << this->ID << '>';
    node = this->fingerTable[i].successorNode;
    return this->fingerTable[i].successorNode->FindSuccessor(ID, startID, node);
     */
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