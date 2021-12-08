#include "lamport.hpp"

Lamport::Lamport(int rank) {
    increment = 1 << rank;
    timestamp = 0;
};

Lamport* Lamport::instance = NULL;

Lamport* Lamport::getClock() {
    if (instance == NULL)
        instance = new Lamport(0);
    return instance;
}

Lamport* Lamport::getClock(int rank) {
    if (instance == NULL)
        instance = new Lamport(rank);
    return instance;
}

void Lamport::tick() {
    timestamp += increment;
};

void Lamport::sync(int time) {
    if (timestamp < time)
        timestamp = time;
    timestamp++;
};