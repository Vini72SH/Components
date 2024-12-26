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
    tag = 0;
    fetchTarget = 0;
    simplePredictor = new TwoBitPredictor();
};

bool btb_entry::getValid() {
    return validBit;
};

uint32_t btb_entry::getTag() {
    return tag;
};

uint32_t btb_entry::getTarget() {
    return fetchTarget;
};

bool btb_entry::getPrediction() {
    return simplePredictor->getPrediction();
};

void btb_entry::setEntry(uint32_t tag, uint32_t fetchTarget) {
    this->tag = tag;
    this->fetchTarget = fetchTarget;
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
    return 0;
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