//============================================================================
// Name        : RfcommListener.cpp
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 1 mai 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#include "RfcommListener.h"

namespace bt
{

RfcommListener::RfcommListener() :
		BtSocket(Type::Rfcomm)
{

}


uint8_t RfcommListener::getLocalPort() const
{
    if (getHandle() != InvalidSocket)
    {
        // Retrieve informations about the local end of the socket
        sockaddr_in address;
        socklen_t size = sizeof(address);
        if (getsockname(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
        {
            return ntohs(address.sin_port);
        }
    }

    // We failed to retrieve the port
    return 0;
}

Status RfcommListener::listen(const BtAddress& address, uint8_t port)
{
    // Create the internal socket if it doesn't exist
    create();

    // Check if the address is valid
    if((address == BtAddress::None) || (address == BtAddress::All))
        return Status::Error;

    // Bind the socket to the specified port
    sockaddr_rc addr = createAddress(address.toBdaddr(), port);
    if(::bind(getHandle(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
    {
        std::cerr << "Failed to bind listener socket to port " << port << std::endl;
        return Status::Error;
    }

    // Listen to the bound port
    if (::listen(getHandle(), 1) == -1)
    {
    	std::cerr << "Failed to listen to port " << port << std::endl;
        return Status::Error;
    }

    return Status::Done;
}

Status RfcommListener::accept(RfcommSocket& socket)
{
    // Make sure that we're listening
    if (getHandle() == InvalidSocket)
    {
    	std::cerr << "Failed to accept a new connection, the socket is not listening" << std::endl;
        return Status::Error;
    }

    // Accept a new connection
    sockaddr_rc address;
    socklen_t length = sizeof(address);
    int remote = ::accept(getHandle(), reinterpret_cast<sockaddr*>(&address), &length);

    // Check for errors
    if (remote == InvalidSocket)
        return getErrorStatus();

    // Initialize the new connected socket
    socket.close();
    socket.create(remote);

    return Status::Done;
}

void RfcommListener::close()
{
	BtSocket::close();
}

}  // namespace bt


