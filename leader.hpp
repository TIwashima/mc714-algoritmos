
#include <unistd.h>
#include "mpi.hpp"

class Leader{

public:
    int rank;
    Leader();
    int election();
    int check();
};