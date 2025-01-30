#ifndef COMPONENT 
#define COMPONENT

#include <cstdio>
#include <sys/types.h>

class Linkable {};

template <typename MessageType>
struct Node{
    MessageType data;
    Node<MessageType>* next;

    Node(MessageType data) : data(data), next(nullptr) {};
};

template <typename MessageType>
class Queue{
  private:
    int size;
    Node<MessageType>* start;
    Node<MessageType>* end;
  
  public:
    Queue() : size(0), start(nullptr), end(nullptr) {};

    /**
    * @brief Returns a boolean indicating whether the queue is empty
    */
    bool isEmpty() {
      return (this->size == 0);
    };

    /**
     * @brief Returns the size of the queue
     */
    int queueSize() {
      return this->size;
    };

    /**
     * @brief Enqueues a generic type
     * @param data An element of the type parameterized by the class
     */
    void enqueue(MessageType data) {
      Node<MessageType>* aux;
      Node<MessageType>* newNode = new Node(data);

      if (isEmpty()) {
          this->start = newNode;
      } else {
          aux = this->end;
          aux->next = newNode;
      }

      this->end = newNode;
      this->size++;
    };

    /**
     * @brief Dequeues a generic type
     * @param data An element of the type parameterized by the class
     */
    MessageType dequeue() {
      Node<MessageType>* aux;
      MessageType nodeData;

      if(isEmpty()) {
          return MessageType();
      }

      aux = this->start;
      nodeData = aux->data;
      this->start = aux->next;
      if(aux->next == nullptr) {
          this->end = nullptr;
      }

      delete aux;

      this->size--;
      return nodeData;
    };

    /**
     * @brief Flush the queue
     */
    void flushQueue() {
      while(!(isEmpty())) {
        dequeue();
      }
    };

    ~Queue() {
      flushQueue();
    };
};

template <typename MessageType>
class Component : public Linkable {
  private:
    Queue<MessageType> messageQueue;

  public:

    Component() : messageQueue() {};

    /** 
    * @brief Wrapper method 
    */
    bool isQueueEmpty() {
      return messageQueue.isEmpty();
    };

    /** 
    * @brief Wrapper method 
    */
    int queueSize() {
      return messageQueue.queueSize();
    }

    /** 
    * @brief Wrapper method 
    */
    void enqueue(MessageType data) {
      messageQueue.enqueue(data);
    }

    /** 
    * @brief Wrapper method 
    */
    MessageType dequeue() {
      return messageQueue.dequeue();
    }

    /** 
    * @brief Wrapper method 
    */
    void flushQueue() {
      messageQueue.flushQueue();
    }

    /**
     * @brief Component behavior during a clock tick
     * @details This is a virtual method as the components behave differently during a clock cycle.
     * This method will be called on each simulated clock tick to update the state of the simulation.
     * The component should implement this method in the way it is expected to behave during a clock cycle 
     * after receiving messages from other components in the previous cycle.
     * It is recommended that you empty the message queue inside this method while defining its behavior 
     * for each type of message that the component may have received previously.
     */
    virtual void componentClock() = 0;

    virtual ~Component() {};
};

#endif