#ifndef FINGERTABLEELEMENT_H
#define	FINGERTABLEELEMENT_H
class ChordNode;

class FingerTableElement {
public:
    int successorID;
    int interval;
    ChordNode *successorNode;
};

#endif	/* FINGERTABLEELEMENT_H */