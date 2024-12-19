#include "interleavedBTB.hpp"
#include <cstdint>

/* ==========================================================================
    Interleaved BTB Methods
   ========================================================================== */

BranchTargetBuffer::BranchTargetBuffer() {};

void BranchTargetBuffer::allocate(uint latency, uint numBanks, uint numEntries) {
    this->numBanks = numBanks;
    this->numEntries = numEntries;
    this->latency = latency;
    this->totalBranches = 0;
    this->totalHits = 0;
    this->nextFetchBlock = 0;
    this->instructionValidBits = new bool[numBanks];
    this->banks = new btb_bank[numBanks];
    for (int bank = 0; bank < numBanks; ++bank) {
        this->instructionValidBits[bank] = false;
        this->banks[bank] = new btb_entry[numEntries];
        for (int entry = 0; entry < numEntries; ++entry) {
            this->banks[bank][entry].allocate();
        }
    }
};

bool* BranchTargetBuffer::getValidInstructions() {};

long int BranchTargetBuffer::getNextFetchAddress() {};

void BranchTargetBuffer::updateInfo(long int fetchAddress, bool trueHit) {};

void BranchTargetBuffer::registerNewEntry(uint32_t fetchAddress, long int* fetchTargets) {};

BranchTargetBuffer::~BranchTargetBuffer() {
    delete[] instructionValidBits;

    for (int i = 0; i < numBanks; ++i) {
        delete[] banks[i];
    }
    delete[] banks;

    instructionValidBits = nullptr;
    banks = nullptr;
};