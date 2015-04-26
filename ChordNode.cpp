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
    /*
    if (this->fingerTable[0].successorID == this->ID) {
        this->fingerTable[0].successorNode = newNode;
        newNode->fingerTable[0].successorNode = this;
        for (int i = 0; i < this->ftSize; i++) {
            newNode->fingerTable[i].successorID = 0;
            newNode->fingerTable[i].successorNode = this;
            this->fingerTable[i].successorID = ID;
            this->fingerTable[i].successorNode = newNode;
        }
    } else {
        for (int i = 0; i < this->ftSize; i++) {
            ChordNode *successorNode = FindSuccessor(newNode->fingerTable[i].hop, this->ID, this);
            newNode->fingerTable[i].successorID = successorNode->ID;
            newNode->fingerTable[i].successorNode = successorNode;
        }
        newNode->fingerTable[0].successorNode = newNode->fingerTable[0].successorNode;
        FixFingerTables(this->ID, newNode);
    }
     */
    cout << endl << "PEER " << newNode->ID << " ADDED" << endl;
}

void ChordNode::RemovePeer(int ID) {
    ChordNode *node = FindNode(ID);
    delete node;

    cout << endl << "PEER " << ID << " REMOVED" << endl;
}

ChordNode *ChordNode::FixFingerTables(int startID, ChordNode *node) {
    int originalId, convertNewId, newNodeId;
    newNodeId = node->ID;
    int index = 0;
    originalId = this->ID;
    while (index < this->ftSize && this->fingerTable[index].hop <= convertNewId) {
        if (this->fingerTable[index].successorID >= convertNewId) {
            this->fingerTable[index].successorID = convertNewId;
            this->fingerTable[index].successorNode = node;
        }
        index++;
    }
    if (this->fingerTable[0].successorID != startID)
        return this->fingerTable[0].successorNode->FixFingerTables(startID, node);
    return this;
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
    return FindSuccessor(key, this->ID, this);
}

ChordNode *ChordNode::FindNode(int ID) {
    return NULL;
}

ChordNode *ChordNode::FindSuccessor(int ID, int startID, ChordNode *node) {
    int i = this->ftSize - 1;
    while (i >= 0 && ID <= this->fingerTable[i].successorID) {
        i--;
    }
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