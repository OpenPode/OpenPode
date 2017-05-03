//============================================================================
// Name        : BtSocket.cpp
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 29 avr. 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#include "BtSocket.h"
#include <cstring>

namespace bt
{

BtSocket::BtSocket(Type type) :
	m_type(type),
	m_socket(BtSocket::InvalidSocket)
{

}

BtSocket::~BtSocket()
{
	close();
}

int BtSocket::getHandle() const
{
	return m_socket;
}

void BtSocket::create()
{
	if(m_socket == InvalidSocket)
	{
		int socket;

		if(m_type == Type::Rfcomm)
			socket = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

		if(socket == InvalidSocket)
			std::cerr << "Failed to create socket" << std::endl;

		create(socket);
	}
}

void BtSocket::create(int handle)
{
	if(m_socket == InvalidSocket)
	{
		m_socket = handle;
	}
}

void BtSocket::close()
{
	if(m_socket != BtSocket::InvalidSocket)
	{
		::close(m_socket);
		m_socket = BtSocket::InvalidSocket;
	}
}

Status BtSocket::getErrorStatus()
{
	// The followings are sometimes equal to EWOULDBLOCK,
	// so we have to make a special case for them in order
	// to avoid having double values in the switch case
	if ((errno == EAGAIN) || (errno == EINPROGRESS))
		return Status::NotReady;

	switch (errno)
	{
		case EWOULDBLOCK:  return Status::NotReady;
		case ECONNABORTED: return Status::Disconnected;
		case ECONNRESET:   return Status::Disconnected;
		case ETIMEDOUT:    return Status::Disconnected;
		case ENETRESET:    return Status::Disconnected;
		case ENOTCONN:     return Status::Disconnected;
		case EPIPE:        return Status::Disconnected;
		default:           return Status::Error;
	}
}

sockaddr_rc BtSocket::createAddress(const bdaddr_t& addr, uint8_t port)
{
    sockaddr_rc address;
    std::memset(&address, 0, sizeof(address));
    bacpy(&address.rc_bdaddr, &addr);
    address.rc_family = AF_BLUETOOTH;
    address.rc_channel = port;

    return address;
}

}  // namespace bt


