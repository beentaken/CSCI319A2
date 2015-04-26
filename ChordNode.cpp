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

ChordNode::ChordNode(const ChordNode &node) {
    this->ID = node.ID;
    this->ftSize = node.ftSize;
    this->chordSize = node.chordSize;
    this->predecessor = node.predecessor;
    this->successor = node.successor;
    this->fingerTable = new FingerTableRow[ftSize];
    for (int i = 0; i < this->ftSize; i++) {
        this->fingerTable[i].hop = node.fingerTable[i].hop;
        this->fingerTable[i].successorID = node.fingerTable[i].successorID;
        this->fingerTable[i].successorNode = node.fingerTable[i].successorNode;
    }
    this->data = node.data;
}

ChordNode::ChordNode(int ID, int ftSize) {
    this->ID = ID;
    this->ftSize = ftSize;
    this->chordSize = pow(2, ftSize);
    this->predecessor = this;
    this->successor = this;
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

void ChordNode::AddPeer(int ID) {
    ChordNode *newNode = new ChordNode(ID, this->ftSize);
    if (this->successor->ID == this->ID) {
        this->predecessor = newNode;
        this->successor = newNode;
        newNode->predecessor = this;
        newNode->successor = this;
        for (int i = 0; i < this->ftSize; i++) {
            newNode->fingerTable[i].successorID = 0;
            newNode->fingerTable[i].successorNode = this;
            this->fingerTable[i].successorID = ID;
            this->fingerTable[i].successorNode = newNode;
        }
    } else {
        for (int i = 0; i < this->ftSize; i++) {
            ChordNode *successorNode = FindSuccessor(newNode->fingerTable[i].hop);
            newNode->fingerTable[i].successorID = successorNode->ID;
            newNode->fingerTable[i].successorNode = successorNode;
        }
        newNode->successor = newNode->fingerTable[0].successorNode;
        newNode->predecessor = newNode->successor->predecessor;
        newNode->predecessor->successor = newNode;
        newNode->successor->predecessor = newNode;
        FixFingerTables(this);
        ChordNode *succ = this->successor;
        while (succ->ID != 0) {
            FixFingerTables(succ);
            succ = succ->successor;
        }
    }

    cout << endl << "PEER " << newNode->ID << " ADDED" << endl;
}

void ChordNode::RemovePeer(int ID) {
    ChordNode *node = FindNode(ID);
    delete node;

    cout << endl << "PEER " << ID << " REMOVED" << endl;
}

void ChordNode::FixFingerTables(ChordNode *node) {
    for (int i = 0; i < node->ftSize; i++) {
        ChordNode *successorNode = FindSuccessor(node->fingerTable[i].hop);
        node->fingerTable[i].successorID = successorNode->ID;
        node->fingerTable[i].successorNode = successorNode;
    }
}

void ChordNode::Insert(std::string data) {
    int hashkey = Hash(data);
    ChordNode *node = FindKey(hashkey);
    node->data.insert(pair<int, string>(hashkey, data));
    cout << "INSERTED " << data << " (key=" << hashkey << ") AT " << node->ID << endl;
}

void ChordNode::Delete(std::string data) {
    int hashkey = Hash(data);
    ChordNode *node = FindKey(hashkey);
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

ChordNode *ChordNode::FindKey(int key) {
    return FindSuccessor(key);
}

ChordNode *ChordNode::FindNode(int ID) {
    return NULL;
}

ChordNode *ChordNode::ClosestPrecedingNode(int ID) {
    if (this == this->successor) {
        return this;
    }
    for (int i = this->ftSize - 1; i >= 0; i--) {
        if (InsideRange(fingerTable[i].successorID, this->ID, ID)) {
            return fingerTable[i].successorNode;
        }
    }
    return this->successor;
}

ChordNode *ChordNode::FindSuccessor(int ID) {
    if (InsideRange(ID, this->ID + 1, this->successor->ID)) {
        cout << this->ID << ">";
        return this->successor;
    }
    ChordNode *p = ClosestPrecedingNode(ID);
    return p->FindSuccessor(ID);
}

int ChordNode::Hash(std::string data) {
    int key = 0;
    for (int i = 0; i < data.length(); i++) {
        key = ((key << 5) + key) ^ data[i];
    }
    return key & (chordSize - 1);
}

void ChordNode::Print(int ID) {
    cout << *FindKey(ID) << endl;
}