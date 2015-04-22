#include <cstdlib>
#include "ChordNode.h"
#include "FingerTable.h"
using namespace std;

FingerTable::FingerTable(int size) {
    fingers = new int*[size];
}

ChordNode::ChordNode(int ID, int size) {
    this->ID = ID;
    this->size = size;
    this->data = new std::map<std::string, std::string>();
}

void ChordNode::AddPeer(int ID) {

}

void ChordNode::RemovePeer(int ID) {

}

void ChordNode::Insert(std::string data) {

}

void ChordNode::Delete(std::string data) {

}

ChordNode *ChordNode::FindPredecessor(int ID) {

}

ChordNode *ChordNode::FindSuccessor(int ID) {

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

}

ChordNode *InitChord(int size) {
    ChordNode *chord = new ChordNode(0, size);
    return chord;
}

void Read(std::string filename) {

}

int main(int argc, char** argv) {
    ChordNode *chord = NULL;

    chord = InitChord(5);
    chord->AddPeer(7);
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

