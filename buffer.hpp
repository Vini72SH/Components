#include <new>
#include <cstring>

class Buffer {
	private:
		void *buffer;
		int occupation;
		int bufferSize;
		int messageSize;
		int startOfBuffer;
		int endOfBuffer;
	
	public:
		Buffer() : buffer(nullptr), occupation(0), bufferSize(0), messageSize(0), startOfBuffer(0), endOfBuffer(0) {};

		bool isAllocated() const;

		void allocate(int bufferSize, int messageSize);
		
		int enqueue(void* element);

		void* dequeue();

		int getStart();

		int getEnd();

		~Buffer() {
			if (this->buffer) {
				::operator delete(this->buffer);
			}
		};
};