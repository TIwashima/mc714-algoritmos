#ifndef LAMPORT_H
#define LAMPORT_H
#include <cstddef>

class Lamport{
private:
    Lamport(int rank);
    static Lamport *instance;

public:
    static Lamport *getClock();
    static Lamport *getClock(int rank);
    int timestamp;
    int increment;
    void tick();
    void sync(int time);
};
#endif