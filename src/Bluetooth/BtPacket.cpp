//============================================================================
// Name        : BtPacket.cpp
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 29 avr. 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#include "BtPacket.h"

namespace bt
{

Packet::Packet() : byteSent(0), byteRead(0)
{
	m_data.resize(sizeof(std::size_t));
}

void Packet::clear()
{
	byteSent = 0;
	byteRead = 0;
	m_data.clear();
	m_data.resize(sizeof(std::size_t));
}

void Packet::append(const void* data, std::size_t sizeInBytes)
{
	m_data.insert(m_data.end(), (char*)data, (char*)data+sizeInBytes);
	updatePayloadSizeField();
}

void Packet::append(const std::string& str)
{
	append(str.c_str(), str.size());
}

void* Packet::data()
{
	return m_data.data();
}

size_t Packet::size() const
{
	return m_data.size();
}

void* Packet::payload()
{
	return &m_data[sizeof(size_t)];
}

size_t Packet::payloadSize() const
{
	return m_data.size() - sizeof(size_t);
}


void Packet::updatePayloadSizeField()
{
	size_t sz = payloadSize();
	std::copy((char*)&sz, ((char*)&sz) + sizeof(size_t), m_data.begin());
}

std::string Packet::toString()
{
	return {m_data.begin() + sizeof(size_t), m_data.end()};
}

std::vector<uint8_t> Packet::toByteArray()
{
	return {m_data.begin() + sizeof(size_t), m_data.end()};
}

} // namespace bt
