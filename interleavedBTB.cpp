#include "interleavedBTB.hpp"
#include <cstdint>

/* ==========================================================================
    Two Bit Predictor Methods
   ========================================================================== */

TwoBitPredictor::TwoBitPredictor() : prediction(2) {};

bool TwoBitPredictor::getPrediction() {
    return (prediction >> 1);
};

void TwoBitPredictor::updatePrediction(bool branchTaken) {
    if ((branchTaken) && (prediction < 3)) {
        prediction++;
    }

    if ((!branchTaken) && (prediction > 0)) {
        prediction--;
    }
};

TwoBitPredictor::~TwoBitPredictor() {};


/* ==========================================================================
    BTB Entry Methods
   ========================================================================== */

btb_entry::btb_entry() : validBit(false), simplePredictor(nullptr) {};

void btb_entry::allocate() {
    validBit = false;
    fetchTarget = 0;
    simplePredictor = new TwoBitPredictor();
};

bool btb_entry::getValid() {
    return validBit;
};

uint32_t btb_entry::getTarget() {
    return fetchTarget;
};

bool btb_entry::getPrediction() {
    return simplePredictor->getPrediction();
};

void btb_entry::updateEntry(bool branchTaken) {
    simplePredictor->updatePrediction(branchTaken);
};

void btb_entry::updateEntry(uint32_t fetchTarget, bool branchTaken) {
    this->validBit = true;
    this->fetchTarget = fetchTarget;
    simplePredictor->updatePrediction(branchTaken);
};

btb_entry::~btb_entry() {
    delete simplePredictor;
};

/* ==========================================================================
    Interleaved BTB Methods
   ========================================================================== */

BranchTargetBuffer::BranchTargetBuffer() : instructionValidBits(nullptr), banks(nullptr) {};

uint32_t BranchTargetBuffer::getIndex(uint32_t fetchAddress) {
    uint32_t index = fetchAddress;
    index = index >> numBanks;
    index = index & ((1 << numEntries) - 1);

    return index;
};

void BranchTargetBuffer::allocate(uint numBanks, uint numEntries) {
    this->numBanks = numBanks;
    this->numEntries = numEntries;
    this->totalBranches = 0;
    this->totalHits = 0;
    this->nextFetchBlock = 0;

    int totalBanks = (1 << numBanks);
    int totalEntries = (1 << numEntries);
    this->instructionValidBits = new bool[totalBanks];
    this->banks = new btb_bank[totalBanks];
    for (int bank = 0; bank < totalBanks; ++bank) {
        this->instructionValidBits[bank] = false;
        this->banks[bank] = new btb_entry[totalEntries];

        for (int entries = 0; entries < totalEntries; ++entries) {
            this->banks[bank][entries].allocate();
        }
    }
};

uint32_t BranchTargetBuffer::getNextFetchBlock() {
    return nextFetchBlock;
};

const bool* BranchTargetBuffer::getInstructionValidBits() {
    return instructionValidBits;
};

int BranchTargetBuffer::fetchBTBEntry(uint32_t fetchAddress) {
    uint32_t target;
    uint32_t nextBlock = 0;
    uint32_t index = getIndex(fetchAddress);

    if (banks[0][index].getValid()) {
        for (int i = 0; i < numBanks; ++i) {
            if (banks[i][index].getPrediction()) {
                target = banks[i][index].getTarget();
                if (target != fetchAddress) {
                    nextBlock = target; 
                    instructionValidBits[i] = true;
                }
            } else {
                instructionValidBits[i] = false;
            }
        }

        if (nextBlock == 0) {
            nextBlock = fetchAddress + (1 << numBanks);
        }

        nextFetchBlock = nextBlock;
        return ALLOCATED;
        
    } else {
        nextBlock = fetchAddress + (1 << numBanks);
        nextFetchBlock = nextBlock;

        for (int i = 0; i < numBanks; ++i) {
            instructionValidBits[i] = true;
        }

        return NOTALLOCATED;
    }
};

void BranchTargetBuffer::updateBTBEntries(uint32_t fetchAddress, bool* executedInstructions) {
    bool executedInstruction;
    uint32_t index = getIndex(fetchAddress);

    for (int i = 0; i < numBanks; ++i) {
        executedInstruction = executedInstructions[i];
        banks[i][index].updateEntry(executedInstruction);
    }
};

void BranchTargetBuffer::updateBTBEntries(uint32_t fetchAddress, uint32_t *fetchTargets, bool* executedInstructions) {
    uint32_t fetchTarget;
    bool executedInstruction;
    uint32_t index = getIndex(fetchAddress);

    for (int i = 0; i < numBanks; ++i) {
        fetchTarget = fetchTargets[i];
        executedInstruction = executedInstructions[i];
        banks[i][index].updateEntry(fetchTarget, executedInstruction);
    }
};

BranchTargetBuffer::~BranchTargetBuffer() {
    if (instructionValidBits) {
        delete[] instructionValidBits;
        instructionValidBits = nullptr;
    }

    int totalBanks = (1 << numBanks);
    if (banks) {
        for (int i = 0; i < totalBanks; ++i) {
            delete[] banks[i];
        }
        delete[] banks;
        banks = nullptr;
    }
};