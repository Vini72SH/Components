#define BANK_SIZE 256
#define INTER_FACTOR 4

/**
 * @file component.hpp
 * @brief Public API of the component template class.
 */

class Linkable {};

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
  public:
    /* Other component methods. */

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
    inline ~Component() {}
};

struct InstructionMessage {
    int id;
    long int pc;
};

class TwoBitPredictor {};

struct btb_entry {
    private:
        TwoBitPredictor* simplePredictor;
        long int fetchTarget;
    public:
        btb_entry();
        bool getPrediction();
        long int getFetchTarget();
        ~btb_entry();
};

typedef btb_entry btb_bank[BANK_SIZE];

class BranchTargetBuffer : public Component<InstructionMessage> {
    private:
        long int latency;
        long int totalBranches;
        long int totalHits;
        bool* instructionValidBits;
        long int nextFetchBlock;
        btb_bank interleavedBTB[INTER_FACTOR];
    public:
        BranchTargetBuffer();
        bool* getValidInstructions();
        long int getNextFetchAddress();
        void updateInfo(long int fetchAddress, bool trueHit);
        void registerNewEntry(long int fetchAddress, long int* fetchTargets);
        ~BranchTargetBuffer();
};