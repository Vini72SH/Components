#include "interleavedBTB.hpp"

BranchTargetBuffer::BranchTargetBuffer() {};

bool* BranchTargetBuffer::getValidInstructions() {};

long int BranchTargetBuffer::getNextFetchAddress() {};

void BranchTargetBuffer::updateInfo(long int fetchAddress, bool trueHit) {};

void BranchTargetBuffer::registerNewEntry(long int fetchAddress, long int* fetchTargets) {};

BranchTargetBuffer::~BranchTargetBuffer() {};