#include <cstdlib>
#include <cmath>
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
    for (int i = 0; i < node.size; i++) {
        out << node.fingerTable[i].successorID << " ";
    }
    return out;
}

ChordNode::ChordNode(const ChordNode &node) {
    this->ID = node.ID;
    this->size = node.size;
    this->fingerTable = new FingerTableRow[size];
    for (int i = 0; i < this->size; i++) {
        this->fingerTable[i].hop = node.fingerTable[i].hop;
        this->fingerTable[i].successorID = node.fingerTable[i].successorID;
        this->fingerTable[i].successorNode = node.fingerTable[i].successorNode;
    }
    this->data = node.data;
}

ChordNode::ChordNode(int ID, int size) {
    this->ID = ID;
    this->size = size;
    this->predecessor = this->successor = this;
    this->data = std::map<int, std::string>();
    this->fingerTable = new FingerTableRow[size];
    for (int i = 0; i < this->size; i++) {
        this->fingerTable[i].hop = ID + pow(2, i);
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
    ChordNode *newNode = new ChordNode(ID, this->size);
    newNode->predecessor = NULL;
    //cout << this->ID << ">";
    /*
    if (this->successor->ID == this->ID) {
        this->successor = newNode;
        newNode->successor = this;
        for (int i = 0; i < size; i++) {
            newNode->fingerTable[i].successorID = 0;
            newNode->fingerTable[i].successorNode = this;
            this->fingerTable[i].successorID = ID;
            this->fingerTable[i].successorNode = newNode;
        }
    } else {
        newNode->successor = this->FindSuccessor(ID, newNode);
        for (int i = 0; i < size; i++) {
            ChordNode *successorNode = FindSuccessor(newNode->fingerTable[i].successorID, newNode->fingerTable[i].successorNode);
            newNode->fingerTable[i].successorID = successorNode->ID;
            newNode->fingerTable[i].successorNode = successorNode;
        }
    }
    */
    ChordNode *succ = FindSuccessor(ID, newNode);
    FixFingerTable(succ);
    newNode->successor = succ;
    cout << endl << "PEER " << newNode->ID << " ADDED" << endl;
    cout << *newNode << endl;
}

void ChordNode::RemovePeer(int ID) {

}

void ChordNode::Insert(std::string data) {

}

void ChordNode::Delete(std::string data) {

}

bool ChordNode::InsideRange(int ID, int start, int finish) {
    int maxID = pow(2, size);
    int minID = 0;

    return (start < finish && start <= ID && ID <= finish) ||
            (start > finish && ((start <= ID && ID <= maxID) || (minID <= ID && ID <= finish))) ||
            ((start == finish) && (ID == start));

}

int ChordNode::FindKey(std::string key) {

}

ChordNode *ChordNode::FindNode(int ID) {

}

ChordNode *ChordNode::ClosestPrecedingNode(int ID, ChordNode *node) {
    if (node == node->successor) {
        return node;
    }
    for (int i = size - 1; i > 0; i--) {
        if (InsideRange(node->fingerTable[i].successorID, ID + 1, ID - 1)) {
            return node->fingerTable[i].successorNode;
        }
    }
    return node->successor;
}

ChordNode *ChordNode::FindSuccessor(int ID, ChordNode *node) {
    bool inrange = InsideRange(ID, this->ID + 1, successor->ID);
    if (inrange) {
        return successor;
    }
    ChordNode *p = ClosestPrecedingNode(ID, node);
    return p->FindSuccessor(ID, node);
}

ChordNode *ChordNode::UpdateNodes(ChordNode *node) {
    int index = 0;
    while (index < size && fingerTable[index].hop <= node->ID) {
        if (fingerTable[index].successorID >= node->ID) {
            fingerTable[index].successorID = node->ID;
            fingerTable[index].successorNode = node;
        }
        index++;
    }
    if (fingerTable[0].successorID != node->ID)
        return fingerTable[0].successorNode->UpdateNodes(node);
    return this;
}

void ChordNode::FixFingerTable(ChordNode *node) {
    for (int i = 0; i < size; i++) {
        node->fingerTable[i].successorNode = FindSuccessor((node->ID + (int) pow(2, i)) % (int) pow(2, size), node);
        node->fingerTable[i].successorID = node->fingerTable[i].successorNode->ID;
    }
}

void ChordNode::Stabilize() {
    ChordNode *node = this->successor->predecessor;
    if (node->ID != this->ID && InsideRange(node->ID, this->ID + 1, this->successor->ID - 1)) {
        this->successor = node;
    }
    this->successor->Notify(this);
}

void ChordNode::Notify(ChordNode *node) {
    if ((this->predecessor->ID == this->ID) ||
            (InsideRange(node->ID, this->predecessor->ID + 1, this->ID - 1))) {
        predecessor = node;
    }
}

int ChordNode::Hash(std::string data) {
    int key = 0;
    for (int i = 0; i < data.length(); i++) {
        key = ((key << 5) + key) ^ (int) data[i];
    }
    return key;
}

void ChordNode::Print(int ID) {
    ChordNode *node = FindNode(ID);
    cout << node << endl;
}

int ChordNode::GetSize() {
    return this->size;
}

int ChordNode::GetID() {
    return this->ID;
}

FingerTableRow ChordNode::GetFingerTableRow(int index) {
    return this->fingerTable[index];
}