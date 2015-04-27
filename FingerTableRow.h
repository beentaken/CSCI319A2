/******************************************************************************
 * Mark Thompson                                                              *
 * Student #: 4419431                                                         *
 * CSCI319 Distributed Systems - Assignment 2 (Chord Simulator)               *
 *****************************************************************************/

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