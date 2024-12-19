/**
 * @file interleavedBTB.hpp
 * @brief Implementation of Interleaved Branch Target Buffer
 */
#include <cstdint>
#include <sys/types.h>
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

struct btb_entry;
typedef btb_entry* btb_bank;

struct btb_entry {
    private:
        TwoBitPredictor* simplePredictor;
        long int fetchTarget;
        bool validBit;

    public:
        btb_entry();

        /**
         * @brief Only allocates the BTB entry
         */
        void allocate();
        ~btb_entry();
};

class BranchTargetBuffer : public Component<InstructionMessage> {
    private:
        uint totalBranches;
        uint32_t totalHits;
        uint32_t nextFetchBlock;
        bool* instructionValidBits;
        btb_bank* banks;
        uint numBanks, numEntries;

    public:
        BranchTargetBuffer();

        /**
         * @brief Allocate the BTB
         * @param numBanks Number of bits used to index the banks (2 bits = 4 banks)
         * @param numEntries Number of bits used to index the entries (8 bits = 256 entries)
         */
        void allocate(uint numBanks, uint numEntries);

        ~BranchTargetBuffer();
};