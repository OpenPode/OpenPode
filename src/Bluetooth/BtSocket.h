//============================================================================
// Name        : BtSocket.h
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 29 avr. 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#ifndef BTSOCKET_H_
#define BTSOCKET_H_

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>

namespace bt
{

enum class Status
{
	Done,         ///< The socket has sent / received the data
	NotReady,     ///< The socket is not ready to send / receive data yet
	Partial,      ///< The socket sent a part of the data
	Disconnected, ///< The socket has been disconnected
	Error         ///< An unexpected error happened
};

enum class Type
{
	Rfcomm,

	// Others are unsupported right now
	//L2cap,
	//Lmp,
	//Sdp,
	//Hci,
};

class BtSocket
{
public:

    enum
    {
        AnyPort = 0,
		InvalidSocket = -1
    };

    virtual ~BtSocket();

protected:

    BtSocket(Type type = Type::Rfcomm);

    int getHandle() const;

    void create();

    void create(int handle);

    void close();

    Status getErrorStatus();

    sockaddr_rc createAddress(const bdaddr_t& addr, uint8_t port);

private:

    Type m_type;
    int m_socket;
};

} // namespace bt

#endif // BTSOCKET_H_
