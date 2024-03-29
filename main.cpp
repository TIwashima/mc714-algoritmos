#include <iostream>
#include "mpi.hpp"
#include "lamport.hpp"
#include "mutex.hpp"
#include "leader.hpp"

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
    MPI_Service *mpi = MPI_Service::getSingleton();
    int size = mpi -> getSize();
    int rank = mpi -> getRank();
    Lamport *clock = Lamport::getClock(rank);

    // Lamport
    clock -> tick();
    cout << "[" << rank << "] timestamp " << clock -> timestamp << endl;
    for (int i = 0; i < 2; i++) {
        if (rank == 0) {
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
    cout << "[" << rank << "] timestamp " << clock -> timestamp << endl;
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
            cout << "Dono do mutex: " << rank << endl;
        slave-> free();
        Mutex_Coord::stopReceiving();
        delete(slave);
    }
    join();

    // Leader
    Leader *leader = new Leader();
    while (1) {
        leader -> check();
        if (mpi -> getRank() == 2){
            cout << "End rank 2." << endl;
            break;
        }
    }

    join();

    delete(leader);
    delete(clock);
    MPI_Service::destroy();
    return 0;
} 