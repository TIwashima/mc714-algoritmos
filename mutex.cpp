#include "mutex.hpp"

Mutex_Coord::Mutex_Coord(){
    mutex = FREE;
    receiving = false;
    Q = new queue<int>();
}

Mutex_Coord::~Mutex_Coord(){
    delete(Q);
}

void Mutex_Coord::receive(){
    Lamport *clock = Lamport::getClock();
    MPI_Service *mpi = MPI_Service::getSingleton();

    int message;
    MPI_Status status;
    while(receiving){
        mpi -> receiveMsg(&message, MPI_ANY_SOURCE, &status);
        if (message == MUTEX_REQUEST) {
            if (mutex == FREE) {
                mutex = BUSY;
                mpi -> sendMsg(MUTEX_REQUEST, status.MPI_SOURCE, clock -> timestamp);
            } else {
                Q -> push(status.MPI_SOURCE);
            }
        }
        else if (message == MUTEX_FREE) {
            if (Q -> empty()) {
                mutex = FREE;
            } else {
                mpi-> sendMsg(MUTEX_REQUEST, Q -> front(), clock -> timestamp);
                Q -> pop();
            }
        }
        else if (message == STOP_RECEIVING) {
            if (Q -> empty() && mutex == FREE)
                receiving = false;
        }
    }
}

void Mutex_Coord::startReceiving(){
    receiving = true;
    receive();
}

void Mutex_Coord::stopReceiving(){
    Lamport *clock = Lamport::getClock();
    MPI_Service *mpi = MPI_Service::getSingleton();
    mpi -> sendMsg(STOP_RECEIVING, RANK, clock -> timestamp);
}


            


int Mutex_Slave::request(){
    Lamport *clock = Lamport::getClock();
    MPI_Service *mpi = MPI_Service::getSingleton();
    mpi -> sendMsg(MUTEX_REQUEST, RANK, clock -> timestamp);
    int message;
    mpi -> receiveMsg(&message, RANK);
    return message;
}

int Mutex_Slave::free()  {
    Lamport *clock = Lamport::getClock();
    MPI_Service *mpi = MPI_Service::getSingleton();
    return mpi -> sendMsg(MUTEX_FREE, RANK, clock -> timestamp);
}