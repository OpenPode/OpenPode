//============================================================================
// Name        : RfcommSocket.h
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 29 avr. 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#ifndef RFCOMMSOCKET_H_
#define RFCOMMSOCKET_H_

#include "BtSocket.h"
#include "BtPacket.h"
#include "BtAddress.h"

namespace bt
{

class RfcommSocket : public BtSocket
{
public:

	RfcommSocket(bool blocking = false);

    uint8_t getLocalPort() const;

    BtAddress getRemoteAddress() const;

    uint8_t getRemotePort() const;

    Status connect(const bt::BtAddress& remoteAddr, uint8_t remotePort);

    void close();

    Status send(const void* data, std::size_t size, std::size_t& sent);

    Status receive(void* data, std::size_t size, std::size_t& received);

    Status send(Packet& packet);

    Status receive(Packet& packet);

private:

    friend class RfcommListener;
};

} // namespace bt

#endif // RFCOMMSOCKET_H_
