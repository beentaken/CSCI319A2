#ifndef FINGERTABLEROW_H
#define	FINGERTABLEROW_H
class ChordNode;

class FingerTableRow {
public:
    int hop;
    int successorID;
    ChordNode *successorNode;
};

#endif	/* FINGERTABLEROW_H */