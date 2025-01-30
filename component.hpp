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

/**
 * @details All components shall inherit from this class. The MessageType type
 * parameter defines the message type the component receives from other
 * components. If the component does not receive any message, int can be used as
 * a placeholder. Note that Component<T> is just a wrapper for the underlying
 * mother class Linkable. This is done to centralize the message-passing
 * implementation in a non-template class for optimization reasons. This wrapper
 * allows a nice and type-safe API on top of a single, fast and generic
 * implementation.
 *
 * Avoiding big types in MessageType is a good idea, because they're passed by
 * value.
 */
template <typename MessageType>
class Component : public Linkable {
  private:
    Queue<MessageType> messageQueue;

  public:
    Component() : messageQueue() {};

    /**
     * @brief This method should be called by other components to send a message
     * to the component.
     * @param message The message to send.
     * @param channelID The ID of the channel on which to send the message.
     * @returns Non-zero if the message cannot be sent (i.e., there's no space
     * left for buffering, case where the sender may want to buffer itself the
     * message for trying again in the next cycles or just give up trying to
     * send). 0 otherwise.
     */
    inline int SendMessage(const MessageType message, int channelID) {
        return this->SendMessageLinkable((const char*)&message, channelID);
    }
    /**
     * @brief This method should be called by other components to read the
     * response buffered.
     * @param message The buffer on which to copy the response message.
     * @param channelID The ID of the channel from which to retrieve a response.
     * @returns Non-zero if there's no response buffered yet. In this case,
     * message is not touched. 0 otherwise, and message is populated with the
     * response.
     */
    inline int RetrieveResponse(MessageType* message, int channelID) {
        return this->RetrieveResponseLinkable((const char*)&message, channelID);
    }
    
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