//============================================================================
// Name        : BtPacket.h
// Author      : Julien Combattelli
// EMail       : julien.combattelli@hotmail.com
// Date		   : 29 avr. 2017
// Version     : 1.0.0
// Copyright   : This file is part of MUL project which is released under
//               MIT license. See file LICENSE.txt for full license details
// Description :
//============================================================================

#ifndef BTPACKET_H_
#define BTPACKET_H_

#include <string>
#include <vector>
#include <cstdint>

namespace bt
{

class Packet
{
public:
	Packet();

	void clear();

	void append(const void* data, std::size_t sizeInBytes);

	void append(const std::string& str);

	template<typename T>
	void append(const std::vector<T>& vect);

	template <class InputIterator>
	void append(InputIterator first, InputIterator last);

	void* data();

	size_t size() const;

	void* payload();

	size_t payloadSize() const;

	std::string toString();

	std::vector<uint8_t> toByteArray();

	size_t byteSent;
	size_t byteRead;

private:

	void updatePayloadSizeField();

	std::vector<char> m_data;
};

template<typename T>
void Packet::append(const std::vector<T>& vect)
{
	append(vect.data(), vect.size()*sizeof(T));
}

template <class InputIterator>
void Packet::append(InputIterator first, InputIterator last)
{
	std::copy(first, last, std::back_inserter(m_data));
	updatePayloadSizeField();
}

} // namespace bt

#endif // BTPACKET_H_
