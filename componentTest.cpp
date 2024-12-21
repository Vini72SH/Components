#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "interleavedBTB.hpp"

using namespace std;

int main () {
    BranchTargetBuffer* interleavedBTB = new BranchTargetBuffer();

    if (interleavedBTB == nullptr) {
        cout << "Falha na alocação!\n";
        exit(1);
    }

    cout << "Alocação bem sucedida!\n";

    int numBanks = 2;
    int numEntries = 8;

    interleavedBTB->allocate(numBanks, numEntries);

    delete interleavedBTB;
    cout << "Desalocação de memória!\n";
}