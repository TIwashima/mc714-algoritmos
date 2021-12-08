#ifndef MUTEX_H
#define MUTEX_H
#include <queue>
#include "mpi.hpp"
#include "lamport.hpp"

#define FREE 0
#define BUSY 1
#define RANK 1

class Mutex_Coord {

private:
    int mutex;
    bool receiving;
    queue<int> *Q;

public:
    Mutex_Coord();
    ~Mutex_Coord();
    void receive();
    void startReceiving();
    static void stopReceiving();

};

class Mutex_Slave {

public:
    int request();
    int free();
};

#endif