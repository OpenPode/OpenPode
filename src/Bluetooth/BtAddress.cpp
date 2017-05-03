//============================================================================
// Name        : BtAddress.cpp
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 29 avr. 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#include "BtAddress.h"

namespace bt
{

const BtAddress BtAddress::None;
const BtAddress BtAddress::Any("00:00:00:00:00:00");
const BtAddress BtAddress::All("FF:FF:FF:FF:FF:FF");
const BtAddress BtAddress::Local("00:00:00:FF:FF:FF");

BtAddress::BtAddress()
{
	m_isValid = false;
}

BtAddress::BtAddress(const char (&addr)[18])
{
	int error = str2ba(addr, &m_addr);
	if(error)
		m_isValid = false;
	else
		m_isValid = true;
}

BtAddress::BtAddress(const std::string& addr)
{
	int error = str2ba(addr.c_str(), &m_addr);
	if(error)
		m_isValid = false;
	else
		m_isValid = true;
}

BtAddress::BtAddress(const bdaddr_t& addr)
{
	bacpy(&m_addr, &addr);
	m_isValid = true;
}

std::string BtAddress::toString() const
{
	if(m_isValid)
	{
		char tmp[18];
		ba2str(&m_addr, tmp);
		return tmp;
	}
	else
		return "";

}

bdaddr_t BtAddress::toBdaddr() const
{
	return m_addr;
}

bool operator ==(const BtAddress& left, const BtAddress& right)
{
	return (bacmp(&left.m_addr, &right.m_addr) == 0) and (left.m_isValid == right.m_isValid) ? true : false;
}

bool operator !=(const BtAddress& left, const BtAddress& right)
{
	return !(left == right);
}

} // namespace bt


