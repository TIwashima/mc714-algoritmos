#include <unistd.h>
#include "mpi.hpp"

MPI_Service::MPI_Service() {
    MPI_Init(NULL, NULL);
}

MPI_Service::~MPI_Service() {
    MPI_Finalize();
};

MPI_Service* MPI_Service::instance = NULL;

MPI_Service* MPI_Service::getSingleton() {
    if (instance == NULL)
        instance = new MPI_Service();
    return instance;
};

void MPI_Service::destroy() {
    delete(instance);
}

int MPI_Service::getRank() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return rank;
};

int MPI_Service::getSize() {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return size;
};

int MPI_Service::sendMsg(int message, int dest, int timestamp) {   
    if (dest < getSize()) {
        int msg[2] = { message, timestamp };
        return MPI_Send(msg, 2, MPI_INT, dest, 1, MPI_COMM_WORLD);
    }
    return -1;
};

int MPI_Service::receiveMsg(int *ret, int src) {
    int message[2];
    int fail = MPI_Recv(message, 2, MPI_INT, src, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (!fail) {
        *ret = message[0];
        int timestamp = message[1];
        Lamport::getClock() -> sync(timestamp);
    }
    return fail;
};

int MPI_Service::receiveMsg(int *ret, int src, MPI_Status *status) {
    int message[2];
    int fail = MPI_Recv(message, 2, MPI_INT, src, 1, MPI_COMM_WORLD, status);
    if (!fail) {
        *ret = message[0];
        int timestamp = message[1];
        Lamport::getClock() -> sync(timestamp);
    }
    return fail;
};