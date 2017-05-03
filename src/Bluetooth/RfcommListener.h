//============================================================================
// Name        : RfcommListener.h
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 1 mai 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#ifndef RFCOMMLISTENER_H_
#define RFCOMMLISTENER_H_

#include "BtSocket.h"
#include "BtAddress.h"
#include "RfcommSocket.h"

namespace bt
{

class RfcommListener : public BtSocket
{
public:

	RfcommListener();

    uint8_t getLocalPort() const;

    Status listen(const BtAddress& address, uint8_t port);

    Status accept(RfcommSocket& socket);

    void close();
};

}  // namespace bt

#endif // RFCOMMLISTENER_H_
