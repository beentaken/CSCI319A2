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
    ID = node.ID;
    size = node.size;
    fingerTable = new FingerTableRow[size];
    for (int i = 0; i < size; i++) {
        fingerTable[i].hop = node.fingerTable[i].hop;
        fingerTable[i].successorID = node.fingerTable[i].successorID;
        fingerTable[i].successorNode = node.fingerTable[i].successorNode;
    }
    data = node.data;
}

ChordNode::ChordNode(int ID, int size) {
    this->ID = ID;
    this->size = size;
    this->data = std::map<int, std::string>();
    this->fingerTable = new FingerTableRow[size];
    for (int i = 0; i < size; i++) {
        fingerTable[i].hop = ID + pow(2, i);
        fingerTable[i].successorID = ID;
        fingerTable[i].successorNode = NULL;
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
    ChordNode *newnode = new ChordNode(ID, this->size);
    newnode->successor = FindSuccessor(ID);

    cout << *newnode << endl;
}

void ChordNode::RemovePeer(int ID) {

}

void ChordNode::Insert(std::string data) {

}

void ChordNode::Delete(std::string data) {

}

int ChordNode::FindKey(std::string key) {

}

ChordNode *ChordNode::FindNode(int ID) {

}

ChordNode *ChordNode::FindSuccessor(int ID) {

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

void ChordNode::BuildFingerTable(ChordNode* node) {

}