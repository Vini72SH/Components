#include "queue.hpp"

enum componentType {

};

class clientBase {};

class serverLinkable {};

template <typename MessageType>
class clientLinkable : public clientBase{
    private:
        serverLinkable* server;
        Queue<MessageType>* messageQueue;
        componentType type;

    public:
        clientLinkable();
        void sendMessage();
        MessageType* recieveMessage();
        ~clientLinkable();
};