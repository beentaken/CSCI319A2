#ifndef CHORDNODE_H
#define	CHORDNODE_H
#include <map>
#include <iostream>
#include <string>

class ChordNode {
public:
    ChordNode(int ID);
    void AddPeer(int ID);
    void RemovePeer(int ID);
    int FindKey(std::string key);
    std::string Hash(std::string data);
    void Insert(std::string data);
    void Delete(std::string data);
    void Print(std::string key);

private:
    int ID;
    std::map <std::string, std::string> data;

};

#endif	/* CHORDNODE_H */

