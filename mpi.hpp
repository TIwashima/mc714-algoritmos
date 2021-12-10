#ifndef MPI_SERVICE_H
#define MPI_SERVICE_H

#include <mpi.h>
#include <iostream>
#include "lamport.hpp"

#define SYNC 0
#define MUTEX_REQUEST 1
#define MUTEX_FREE 2
#define STOP_RECEIVING 3
#define LEADER_CHECK 4
#define ELECTION 5
#define ELECTION_ACK 6
#define JOIN 7

using namespace std;

class MPI_Service{
    private:
        MPI_Service();
        static MPI_Service *instance;

    public:
        static MPI_Service *getSingleton();
        static void destroy();
        int getRank();
        int getSize();
        int sendMsg(int message, int dest, int timestamp);
        int sendMsgNb(int message, int dest, int timestamp);
        int receiveMsg(int *ret, int src);
        int receiveMsg(int *ret, int src, MPI_Status *status);
        int receiveMsgNb(int *ret, int src, MPI_Request *request, MPI_Status *status);
};
#endif