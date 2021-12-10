#include "leader.hpp"
#define LEADER_TIME 10

Leader::Leader() {
    rank = MPI_Service::getSingleton() -> getSize() - 1;
}

int Leader::election() {
    Lamport *clock = Lamport::getClock();
    MPI_Service *mpi = MPI_Service::getSingleton();

    for (int i = 0; i < mpi -> getSize(); i++) {
        if (i != mpi -> getRank()) {
            mpi -> sendMsgNb(ELECTION, i, clock -> timestamp);
        }
    }

    int message, flag;
    MPI_Request request;
    MPI_Status status;

    while (1) {
        int timeout = mpi -> receiveMsgNb(&message, MPI_ANY_SOURCE, &request, &status);
        if (timeout) {
            rank = mpi -> getRank();
            return 0;
        }

        if (message == ELECTION_ACK || message == LEADER_CHECK) {
            if (mpi -> getRank() < status.MPI_SOURCE) {
                return 0;
            }
        }  else if (message == ELECTION) {
            mpi -> sendMsgNb(ELECTION_ACK, status.MPI_SOURCE, clock -> timestamp);
            if (mpi -> getRank() < status.MPI_SOURCE) {
                rank = status.MPI_SOURCE;
                return 0;
            }
            
        }
    }
    return 0;
}

int Leader::check() {
    Lamport *clock = Lamport::getClock();
    MPI_Service *mpi = MPI_Service::getSingleton();

    if (rank == mpi -> getRank()) {
        for (int i = 0; i < LEADER_TIME || mpi -> getRank() != 2; i++) { 
            for (int i = 0; i < mpi -> getSize(); i++) {
                if (i != mpi -> getRank()) {
                    mpi -> sendMsgNb(LEADER_CHECK, i, clock -> timestamp);
                    mpi -> sendMsgNb(LEADER_CHECK, i, clock -> timestamp);
                }
            }
            cout << "Rank " << mpi -> getRank() << " lidera há " << i + 1 << " segundos." << endl;
            sleep(1);
        }
    } else  {
        int message, flag;
        MPI_Request request;
        MPI_Status status;
        int timeout = mpi -> receiveMsgNb(&message, MPI_ANY_SOURCE, &request, &status);
        if (timeout)
            election();

        if (message == ELECTION) {
            mpi -> sendMsgNb(ELECTION_ACK, status.MPI_SOURCE, clock->timestamp);
            if (mpi -> getRank() > status.MPI_SOURCE) {
                election();
            } else {
                rank = status.MPI_SOURCE;
            }
        }
    }
    return 0;
}