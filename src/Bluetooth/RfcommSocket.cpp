//============================================================================
// Name        : RfcommSocket.cpp
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 1 mai 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#include "RfcommSocket.h"

namespace bt
{

RfcommSocket::RfcommSocket(bool blocking) :
		BtSocket(Type::Rfcomm)
{
	setBlocking(blocking);
}

uint8_t RfcommSocket::getLocalPort() const
{
    if (getHandle() != InvalidSocket)
    {
        // Retrieve informations about the local end of the socket
        sockaddr_rc address;
        socklen_t size = sizeof(address);
        if (::getsockname(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
            return address.rc_channel;
    }

    // We failed to retrieve the port
    return 0;
}

BtAddress RfcommSocket::getRemoteAddress() const
{
	if (getHandle() != InvalidSocket)
	{
		// Retrieve informations about the remote end of the socket
		sockaddr_rc address;
		socklen_t size = sizeof(address);
		if (::getpeername(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
			return BtAddress(address.rc_bdaddr);
	}

	// We failed to retrieve the address
	return BtAddress::None;
}

uint8_t RfcommSocket::getRemotePort() const
{
    if (getHandle() != InvalidSocket)
    {
        // Retrieve informations about the remote end of the socket
        sockaddr_rc address;
        socklen_t size = sizeof(address);
        if (::getpeername(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
        {
            return address.rc_channel;
        }
    }

    // We failed to retrieve the port
    return 0;
}

Status RfcommSocket::connect(const BtAddress& remoteAddr, uint8_t remotePort)
{
    // Create the internal socket if it doesn't exist
    create();

    // Create the remote address
    sockaddr_rc address = createAddress(remoteAddr.toBdaddr(), remotePort);

	// Connect the socket
	if (::connect(getHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
		return getErrorStatus();

	// Connection succeeded
	return Status::Done;
}

void RfcommSocket::close()
{
	BtSocket::close();
}

Status RfcommSocket::send(const void* data, std::size_t size, std::size_t& sent)
{
    // Check the parameters
    if (!data || (size == 0))
    {
    	std::cerr << "Cannot send data over the network (no data to send)" << std::endl;
        return Status::Error;
    }

    // Loop until every byte has been sent
    int result = 0;
    for (sent = 0; sent < size; sent += result)
    {
        // Send a chunk of data
        result = ::write(getHandle(), static_cast<const char*>(data) + sent, size - sent);

        // Check for errors
        if (result < 0)
        {
            Status status = getErrorStatus();

            if ((status == Status::NotReady) && sent)
                return Status::Partial;

            return status;
        }
    }

    return Status::Done;
}

Status RfcommSocket::receive(void* data, std::size_t size, std::size_t& received)
{
    // First clear the variables to fill
    received = 0;

    // Check the destination buffer
    if (!data)
    {
    	std::cerr << "Cannot receive data from the network (the destination buffer is invalid)" << std::endl;
        return Status::Error;
    }

    // Receive a chunk of bytes
    int sizeReceived = read(getHandle(), static_cast<char*>(data), static_cast<int>(size));

    // Check the number of bytes received
    if (sizeReceived > 0)
    {
        received = static_cast<std::size_t>(sizeReceived);
        return Status::Done;
    }
    else if (sizeReceived == 0)
    {
        return Status::Disconnected;
    }
    else
    {
        return getErrorStatus();
    }
}

Status RfcommSocket::send(Packet& packet)
{
	return send(packet.data(), packet.size(), packet.byteSent);
}

Status RfcommSocket::receive(Packet& packet)
{
	// First clear the variables to fill
	packet.clear();

	// We start by getting the size of the incoming packet
	std::size_t packetSize = 0;
	std::size_t received = 0;

	Status status = receive(reinterpret_cast<void*>(&packetSize), sizeof(packetSize), received);

	if (status != Status::Done)
		return status;

	// Loop until we receive all the packet data
	char buffer[1024];
	while (packet.size() < packetSize)
	{
		// Receive a chunk of data
		std::size_t sizeToGet = std::min(static_cast<std::size_t>(packetSize - packet.payloadSize()), sizeof(buffer));
		Status status = receive(buffer, sizeToGet, received);
		if (status != Status::Done)
			return status;

		// Append it into the packet
		if (received > 0)
			packet.append(buffer, received);
	}

	return Status::Done;
}

}  // namespace bt
