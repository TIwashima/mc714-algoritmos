#include <unistd.h>
#include "mpi.hpp"
#define TIMEOUT 5

MPI_Service::MPI_Service() {
    MPI_Init(NULL, NULL);
}

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
    if (dest >= getSize())
        return -1;
          
    int msg[2] = { message, timestamp };
    return MPI_Send(msg, 2, MPI_INT, dest, 1, MPI_COMM_WORLD);
};

int MPI_Service::sendMsgNb(int message, int dest, int timestamp) {
    if (dest >= getSize())
        return -1;

    MPI_Request request;
    int msg[2] = { message, timestamp };
    return MPI_Isend(msg, 2, MPI_INT, dest, 1, MPI_COMM_WORLD, &request);
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

int MPI_Service::receiveMsgNb(int *ret, int src, MPI_Request *request, MPI_Status *status) {
    int message[2];
    int fail = MPI_Irecv(message, 2, MPI_INT, src, 1, MPI_COMM_WORLD, request);
    int flag;
    bool timeout = false;

    int i = 0;
    do {
        MPI_Test(request, &flag, status);
        sleep(1);
        if (++i == TIMEOUT) {
            timeout = true;
            break;
        }
    } while (!flag);

    if (timeout)
        return -1;

    if (!fail) {
        *ret = message[0];
        int timestamp = message[1];
        Lamport::getClock() -> sync(timestamp);
    }
    return fail;
};