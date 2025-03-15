#ifndef SINUCA3_ENGINE_COMPONENT_HPP_
#define SINUCA3_ENGINE_COMPONENT_HPP_

//
// Copyright (C) 2024  HiPES - Universidade Federal do Paraná
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

/**
 * @file component.hpp
 * @brief Public API of the component template class.
 */

#include "linkable.hpp"
#include <cstdio>

namespace sinuca {

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
class Component : public engine::Linkable {
  public:
    /**
     * @param messageSize The size of the message that will be used by the
     * component.
     */
    inline Component() : engine::Linkable(sizeof(MessageType)) {}

    /**
     * @brief Connect to *this* component.
     * @param bufferSize The size of the buffer used in the connection.
     * @param messageSize The size of the message stored in the buffer.
     * @details Method used by other components to connect to *this* component,
     * establishing a connection where *this* component is the one that responds
     * to received messages.
     * @return Returns the id of connection on the receiving component
     */
    inline int ConnectToComponent(int bufferSize) {
        return this->Connect(bufferSize);
    };

    /**
     * @brief Wrapper to SendRequestToLinkable method
     */
    inline bool SendRequestToComponent(Linkable* component, int connectionID,
                                       void* messageInput) {
        return this->SendRequestToLinkable(component, connectionID,
                                           messageInput);
    };

    /**
     * @brief Wrapper to SendResponseToLinkable method
     */
    inline bool SendResponseToComponent(Linkable* component, int connectionID,
                                        void* messageInput) {
        return this->SendResponseToLinkable(component, connectionID,
                                            messageInput);
    };

    /**
     * @brief Wrapper to ReceiveRequestFromLinkable method
     */
    inline bool ReceiveRequestFromComponent(Linkable* component,
                                            int connectionID,
                                            void* messageOutput) {
        return this->ReceiveRequestFromLinkable(component, connectionID,
                                                messageOutput);
    };

    /**
     * @brief Wrapper to ReceiveResponseFromLinkable method
     */
    inline bool ReceiveResponseFromComponent(Linkable* component,
                                             int connectionID,
                                             void* messageOutput) {
        return this->ReceiveResponseFromLinkable(component, connectionID,
                                                 messageOutput);
    };

    /**
     * @brief Wrapper to SendRequestToConnection method
     */
    inline bool SendRequestForConnection(int connectionID, void* messageInput) {
        return this->SendRequestToConnection(connectionID, messageInput);
    };

    /**
     * @brief Wrapper to SendResponseToConnection method
     */
    inline bool SendResponseForConnection(int connectionID,
                                          void* messageInput) {
        return this->SendResponseToConnection(connectionID, messageInput);
    };

    /**
     * @brief Wrapper to ReceiveRequestFromConnection method
     */
    inline bool ReceiveRequestForAConnection(int connectionID,
                                             void* messageOutput) {
        return this->ReceiveRequestFromConnection(connectionID, messageOutput);
    };

    /**
     * @brief Wrapper to ReceiveResponseFromConnection method
     */
    inline bool ReceiveResponseForAConnection(int connectionID,
                                              void* messageOutput) {
        return this->ReceiveResponseFromConnection(connectionID, messageOutput);
    };

    inline ~Component() {};
};

class EngineDebugComponent
    : public sinuca::Component<int> {
  public:
    bool send;
    int connectionID;
    EngineDebugComponent* otherComponent;
    inline EngineDebugComponent() : send(false), connectionID(0), otherComponent(nullptr) {};
    int FinishSetup() { return 1; };
    void Clock() {
        printf("CLOCK!\n");
        int messsageOutput, messageInput;
        if (otherComponent) {
            if (!(send)) {
                messageInput = 10;
                this->SendRequestToComponent(otherComponent, connectionID, &messageInput);
                printf("Mensagem Enviada: %d\n", messageInput);
                send = true;
            } else {
                this->ReceiveResponseFromComponent(otherComponent, connectionID, &messageInput);
                if (messageInput) {
                    printf("Mensagem Recebida: %d\n", messsageOutput);
                }
            }
        } else {
            for (unsigned int i = 0; i < this->connections.size(); ++i) {
                this->ReceiveRequestForAConnection(i, &messsageOutput);
                if (messsageOutput) {
                    printf("Mensagem Recebida de Conexão: %d\n", messsageOutput);
                    messsageOutput = messsageOutput + 1;
                    this->SendResponseForConnection(i, &messsageOutput);
                    printf("Mensagem Enviada para Conexão: %d\n", messsageOutput);
                }
            }
        }
    };

    virtual ~EngineDebugComponent() {};
};

}  // namespace sinuca

#endif  // SINUCA3_ENGINE_COMPONENT_HPP_
