//============================================================================
// Name        : BtAddress.h
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 29 avr. 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#ifndef BTADDRESS_H_
#define BTADDRESS_H_

#include <bluetooth/bluetooth.h>
#include <string>

namespace bt
{

class BtAddress
{
public:

	BtAddress();
	BtAddress(const char (&addr)[18]);
	BtAddress(const std::string& addr);
	BtAddress(const bdaddr_t& addr);

	std::string toString() const;
	bdaddr_t toBdaddr() const;

	static const BtAddress None;
	static const BtAddress Any;
	static const BtAddress All;
	static const BtAddress Local;

private:

	friend bool operator ==(const BtAddress& left, const BtAddress& right);
	friend bool operator !=(const BtAddress& left, const BtAddress& right);

	bool m_isValid;
	bdaddr_t m_addr;
};

bool operator ==(const BtAddress& left, const BtAddress& right);
bool operator !=(const BtAddress& left, const BtAddress& right);

}  // namespace bt

#endif // BTADDRESS_H_
