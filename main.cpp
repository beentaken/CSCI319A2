#include <cstdlib>
#include <cmath>
#include "ChordNode.h"
#include "FingerTableElement.h"
using namespace std;

ChordNode *FindSuccessor(ChordNode *node, int ID);

ChordNode::ChordNode(int ID, int size) {
    this->ID = ID;
    this->size = size;
    this->predecessor = NULL;
    this->successor = this;
    this->data = new std::map<std::string, std::string>();
    this->fingerTable = new FingerTableElement[size];
    for (int i = 0; i < size; i++) {
        if (ID == 0) {
            fingerTable[i].interval = ID + pow(2, i);
            fingerTable[i].successorID = 0;
            fingerTable[i].successorNode = this;
        }
    }
}

void ChordNode::AddPeer(int ID) {
    ChordNode *newnode = new ChordNode(ID, this->size);
    for (int i = 0; i < size; i++) {
        fingerTable[i].interval = ID + pow(2, i);
        fingerTable[i].successorNode = FindSuccessor(this, fingerTable[i].interval);
        fingerTable[i].successorID = fingerTable[i].successorNode->GetID();
    }
    Print(ID);
}

void ChordNode::RemovePeer(int ID) {

}

void ChordNode::Insert(std::string data) {

}

void ChordNode::Delete(std::string data) {

}

ChordNode *FindPredecessor(int ID) {

}

ChordNode *FindSuccessor(ChordNode *node, int ID) {
    for (int i = 0; i < node->GetSize(); i++) {
        if (node->GetFingerTable(i).interval == ID) {
            cout << "Equals: " << i << "\n";
            return node->GetFingerTable(i).successorNode;
        } else if (node->GetFingerTable(i).interval < ID) {
            continue;
        } else {
            cout << "Else: " << i << "\n";
            ChordNode *newnode = NULL;
            for (int j = (node->GetSize() - 1); j >= 0; j--) {
                if (node->GetFingerTable(j).successorID == ID) {
                    cout << "Found it" << endl;
                }
            }
        }
    }
}

int ChordNode::FindKey(std::string key) {

}

int ChordNode::Hash(std::string data) {
    int key = 0;
    for (int i = 0; i < data.length(); i++) {
        key = ((key << 5) + key) ^ (int) data[i];
    }
    return key;
}

void ChordNode::Print(int ID) {
    cout << "Peer - " << ID << endl;
    cout << "Int\tID\n";
    for (int i = 0; i < 5; i++) {
        cout << this->fingerTable[i].interval << "\t" << this->fingerTable[i].successorID << "\n";
    }
}

int ChordNode::GetSize() {
    return this->size;
}

int ChordNode::GetID() {
    return this->ID;
}

FingerTableElement ChordNode::GetFingerTable(int index) {
    return this->fingerTable[index];
}

ChordNode * InitChord(int size) {
    ChordNode *chord = new ChordNode(0, size);
    return chord;
}

void Read(std::string filename) {

}

int main(int argc, char** argv) {
    ChordNode *chord = NULL;

    chord = InitChord(5);
    chord->AddPeer(7);

    //chord->AddPeer(13);
    //chord->AddPeer(15);
    //chord->AddPeer(2);
    //chord->AddPeer(3);

    /*    chord->AddPeer(3);
        chord->RemovePeer(3);
        chord->AddPeer(12);
        chord->AddPeer(3);
        chord->AddPeer(9);
        chord->RemovePeer(3);
        chord->AddPeer(17);
        chord->Insert("THIS IS A TEST");
        chord->Insert("Markus Hagenbuchner");
        chord->Insert("CSCI319");
        chord->Print(12);
        chord->Delete("THIS IS A TEST");
        chord->Print(12);
        chord->RemovePeer(0);
        chord->Print(7);
        chord->Print(9);
        chord->Print(12);
        chord->Print(17);
     */
    return 0;
}

