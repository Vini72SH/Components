#include "interleavedBTB.hpp"

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

    btb_entry::btb_entry() : simplePredictor(nullptr) {};

    void btb_entry::allocate() {
        simplePredictor = new TwoBitPredictor();
        fetchTarget = 0;
        validBit = false;
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

        for (int entry = 0; entry < totalEntries; ++entry) {
            this->banks[bank][entry].allocate();
        }
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