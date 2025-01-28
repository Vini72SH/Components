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

    bool isEmpty() {
      return (this->size == 0);
    };

    int queueSize() {
      return this->size;
    };

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

    void print () {
      Node<MessageType>* aux;

      aux = this->start;
      while (aux) {
        printf("%d\n", aux->data);
        aux = aux->next;
      }
    }

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

    bool isQueueEmpty() {
      return messageQueue.isEmpty();
    };

    int queueSize() {
      return messageQueue.queueSize();
    }

    void enqueue(MessageType data) {
      messageQueue.enqueue(data);
    }

    MessageType dequeue() {
      return messageQueue.dequeue();
    }

    void print() {
      messageQueue.print();
    }

    void flushQueue() {
      messageQueue.flushQueue();
    }

    ~Component() {};
};

#endif