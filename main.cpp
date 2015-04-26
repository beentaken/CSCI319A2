#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include "ChordNode.h"
#include "FingerTableRow.h"
using namespace std;

ChordNode * InitChord(int size) {
    ChordNode *chord = new ChordNode(0, size);
    cout << "Mark Thompson" << endl << "4419431" << endl;
    return chord;
}

void Read(char *filename) {
    ifstream textfile;
    string line, data;
    int lineend;
    ChordNode *chord = NULL;

    textfile.open(filename);
    while (textfile.good()) {
        while (getline(textfile, line, '\n')) {
            size_t found = line.find_first_of("#");
            if (found != string::npos) {
                lineend = line.length() - found;
            } else {
                lineend = line.length() - 1;
            }

            if (line.substr(0, 9) == "initchord") {
                data = line.substr(10, lineend);
                int size = atoi(data.c_str());
                chord = InitChord(size);
            } else if (line.substr(0, 7) == "addpeer") {
                data = line.substr(8, lineend);
                int ID = atoi(data.c_str());
                chord->AddPeer(ID);
            } else if (line.substr(0, 10) == "removepeer") {
                data = line.substr(11, lineend);
                int ID = atoi(data.c_str());
                chord->RemovePeer(ID);
            } else if (line.substr(0, 6) == "insert") {
                data = line.substr(7, lineend);
                chord->Insert(data);
            } else if (line.substr(0, 6) == "delete") {
                data = line.substr(7, lineend);
                chord->Delete(data);
            } else if (line.substr(0, 5) == "print") {
                data = line.substr(7, lineend);
                int ID = atoi(data.c_str());
                chord->Print(ID);
            }
        }
    }
    /*
    cout << endl << endl << *chord << endl;
    ChordNode *succ = chord->GetSuccessor();
    while (succ != chord) {
        cout << *succ << endl;
        succ = succ->GetSuccessor();
    }
     */ 
}

int main(int argc, char** argv) {
    if (argc == 2) {
        Read(argv[1]);
    } else {
        cout << "You must specify a file" << endl;
    }
    return 0;
}

