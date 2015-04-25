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
    if (this->successor->ID == this->ID) {
        this->predecessor = this->successor = newNode;
        newNode->predecessor = newNode->successor = this;
        for (int i = 0; i < size; i++) {
            newNode->fingerTable[i].successorID = 0;
            newNode->fingerTable[i].successorNode = this;
            this->fingerTable[i].successorID = ID;
            this->fingerTable[i].successorNode = newNode;
        }
    } else {
        newNode->successor = this->FindSuccessor(ID);
        newNode->predecessor = newNode->successor->predecessor;
        newNode->predecessor->successor = newNode;
        newNode->successor->predecessor = newNode;

        for (int i = 0; i < size; i++) {
            ChordNode *successorNode = FindSuccessor(newNode->fingerTable[i].hop);
            newNode->fingerTable[i].successorID = successorNode->ID;
            newNode->fingerTable[i].successorNode = successorNode;
        }
    }
    Stabilize();
    FixFingerTable();
    cout << endl << "PEER " << newNode->ID << " ADDED" << endl;
    //cout << *this << endl;
    //cout << *newNode << endl;
}

void ChordNode::RemovePeer(int ID) {
    ChordNode *node = FindNode(ID);
    node->successor->predecessor = node->predecessor;
    node->predecessor->successor = node->successor;
    delete node;

    cout << endl << "PEER " << ID << " REMOVED" << endl;
}

void ChordNode::FixFingerTable() {
    for (int i = 0; i < size; i++) {
        ChordNode *successorNode = FindSuccessor(this->fingerTable[i].hop);
        this->fingerTable[i].successorID = successorNode->ID;
        this->fingerTable[i].successorNode = successorNode;
    }
}

void ChordNode::Insert(std::string data) {
    int ID = FindKey(data);
    cout << "Found: " << ID << endl;
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
    int hashvalue = Hash(key);
    int nodeint = hashvalue % (int) pow(2, size);
    ChordNode *node = FindNode(nodeint);
    return node->ID;
}

ChordNode *ChordNode::FindNode(int ID) {
    if (InsideRange(ID, this->ID, this->successor->ID)) {
        return this;
    } else {
        this->successor->FindNode(ID);
    }
}

ChordNode *ChordNode::ClosestPrecedingNode(int ID) {
    if (this == successor) {
        return this;
    }
    for (int i = size - 1; i > 0; i--) {
        if (InsideRange(fingerTable[i].successorID, this->ID + 1, ID - 1)) {
            return fingerTable[i].successorNode;
        }
    }
    return successor;
}

ChordNode *ChordNode::FindSuccessor(int ID) {
    if (InsideRange(ID, this->ID + 1, successor->ID)) {
        cout << this->ID << ">";
        return successor;
    }
    ChordNode *p = ClosestPrecedingNode(ID);
    return p->FindSuccessor(ID);
}

ChordNode *ChordNode::UpdateNodes(ChordNode *node) {
    int index = 0;
    while (index < size && fingerTable[index].hop <= node->ID) {
        if (fingerTable[index].successorID >= node->ID)//update successor to point to new node
        {
            fingerTable[index].successorID = node->ID;
            fingerTable[index].successorNode = node;
        }
        index++;
    }
    if (fingerTable[0].successorID != node->ID)//propagate change around chord until back to beginning
        return fingerTable[0].successorNode->UpdateNodes(node);
    return this;
}

void ChordNode::Stabilize() {
    ChordNode *x = this->successor->predecessor;
    if (x->ID != this->ID && InsideRange(x->ID, this->ID + 1, this->successor->ID - 1)) {
        this->successor = x;
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