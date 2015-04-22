#include <cstdlib>
#include "ChordNode.h"
using namespace std;

ChordNode *chord = NULL;

ChordNode::ChordNode(int ID) {
    this->ID = ID;

}

void ChordNode::AddPeer(int ID) {

}

void ChordNode::RemovePeer(int ID) {

}

void ChordNode::Insert(std::string data) {

}

void ChordNode::Delete(std::string data) {

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

void ChordNode::Print(std::string key) {

}

void InitChord(int size) {
    chord = new ChordNode(5);
}

void Read(std::string filename) {

}

int main(int argc, char** argv) {
    InitChord(5);


    return 0;
}

