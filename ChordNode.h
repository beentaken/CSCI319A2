#ifndef CHORDNODE_H
#define	CHORDNODE_H
#include <map>
#include <iostream>
#include <string>

class ChordNode {
public:
    ChordNode(int ID);
    ChordNode(int ID, int size);
    void AddPeer(int ID);
    void RemovePeer(int ID);
    ChordNode *FindPredecessor(int ID);
    ChordNode *FindSuccessor(int ID);
    int FindKey(std::string key);
    int Hash(std::string data);
    void Insert(std::string data);
    void Delete(std::string data);
    void Print(int ID);

private:
    int ID;
    int size;
    std::map <std::string, std::string> *data;
};

#endif	/* CHORDNODE_H */

