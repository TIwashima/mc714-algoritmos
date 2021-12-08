#include <iostream>
#include "mpi.hpp"
#include "lamport.hpp"
#include "mutex.hpp"

using namespace std;

void join() {
    Lamport *clock = Lamport::getClock();
    MPI_Service *mpi = MPI_Service::getSingleton();
    if (mpi -> getRank() == 0) {
        char aux;
        cin >> aux;
        mpi -> sendMsg(JOIN, 1, clock -> timestamp);
        mpi -> sendMsg(JOIN, 2, clock -> timestamp);
    } else {
        int message;
        do {
            mpi -> receiveMsg(&message, 0);
        } while (message != JOIN);
    }
}

int main (int argc, char** argv) {
    Lamport *clock = Lamport::getClock();
    MPI_Service *mpi = MPI_Service::getSingleton();
    int size = mpi -> getSize();
    int rank = mpi -> getRank();

    // Lamport
    clock -> tick();
    cout << "Rank " << rank << " timestamp " << clock -> timestamp << endl;
    for (int i = 0; i < 2; i++) {
        if (rank = 0) {
            int message;
            mpi -> sendMsg(SYNC, 1, clock -> timestamp);
            mpi -> receiveMsg(&message, 2);
        } else if (rank == 1) {
            int message;
            mpi -> receiveMsg(&message, 0);
            mpi -> sendMsg(SYNC, 2, clock -> timestamp);
        } else if (rank == 2) {
            int message;
            mpi -> receiveMsg(&message, 1);
            mpi -> sendMsg(SYNC, 0, clock -> timestamp);
        }
    }
    cout << "Rank " << rank << " timestamp " << clock -> timestamp << endl;
    join();

    // Mutex
    clock -> tick();
    
    if (rank == RANK) {
        Mutex_Coord *coord = new Mutex_Coord();
        coord -> startReceiving();
        delete(coord);
    } else {
        Mutex_Slave *slave = new Mutex_Slave();
        slave -> request();
        for (int i = 0; i < 10; i++)
            cout << "Rank " << rank << endl;
        slave-> free();
        Mutex_Coord::stopReceiving();
        delete(slave);
    }
    join();

    delete(clock);
    MPI_Service::destroy();
    return 0;
} 