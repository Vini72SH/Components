#ifndef COMPONENT 
#define COMPONENT

#include <cstdio>
#include <sys/types.h>
#include "queue.hpp"

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
class Component {
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
     * Using this method, the expected behavior of the component must be defined at each clock cycle, 
     * its interactions with other components and message handling.
     */
    virtual void componentClock() = 0;

    virtual ~Component() {};
};

#endif