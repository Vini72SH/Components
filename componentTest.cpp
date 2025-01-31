#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "interleavedBTB.hpp"

using namespace std;

int main () {
    uint32_t v1[] = {10, 20, 30, 40};
    uint32_t v2[] = {15, 25, 35, 45};

    BranchTargetBuffer* btb = new BranchTargetBuffer();

    btb->allocate(4, 4);

    btb->registerNewBlock(64, v1);

    btb->registerNewBlock(128, v2);

    delete btb; 

    return 0;
}