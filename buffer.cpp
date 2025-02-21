#include "buffer.hpp"

bool Buffer::isAllocated() const {
    return (this->buffer != nullptr);
};

void Buffer::allocate(int bufferSize, int messageSize) {
    this->occupation = 0;
    this->startOfBuffer = 0;
    this->endOfBuffer = 0;
    this->bufferSize = bufferSize;
    this->messageSize = messageSize;
    this->buffer = ::operator new(bufferSize * messageSize);

    if (!(this->buffer)) {
        this->buffer = nullptr;
    }
};

int Buffer::enqueue(void* element) {
    if (occupation < bufferSize) {
        void* target = static_cast<char*>(buffer) + (endOfBuffer * messageSize);
        memcpy(target, element, messageSize);
        ++occupation;
        ++endOfBuffer;

        if (endOfBuffer == bufferSize) {
            endOfBuffer = 0;
        }

        return 1;
    }

    return 0;
};

void* Buffer::dequeue() {
    if (occupation > 0) {
        void* element = static_cast<char*>(buffer) + (startOfBuffer * messageSize);
    
        --occupation;
        ++startOfBuffer;

        if (startOfBuffer == bufferSize) {
            startOfBuffer = 0;
        }

        return element;
    }

    return nullptr;
};

int Buffer::getStart() {
    return startOfBuffer;
};

int Buffer::getEnd() {
    return endOfBuffer;
};