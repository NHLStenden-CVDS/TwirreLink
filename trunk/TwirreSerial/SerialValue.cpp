/*
 * Value.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include <stdexcept>
#include <cstring>

#include "Core/Value.h"

#include "TwirreSerial/SerialValue.h"


using namespace std;

namespace twirre
{
	SerialValue::SerialValue(SerialRW & srw) :
			_serial(srw)
	{
	}

	template<typename T>
	SerialValueImpl<T>::SerialValueImpl(const uint8_t ID, const std::string name, T val, SerialRW & srw) :
			ValueImpl<T>(ID, name, val), SerialValue(srw)
	{

	}

	template<typename T>
	SerialArrayValue<T>::SerialArrayValue(const uint8_t ID, const std::string name, SerialRW & srw) :
			ArrayValue<T>(ID, name), SerialValue(srw)
	{

	}

	template<typename T>
	void SerialValueImpl<T>::addToMessage(vector<unsigned char> &data) const
	{
		data.push_back(this->_id);
		const unsigned char * bytes = reinterpret_cast<const unsigned char*>(&this->_val);
		for (size_t i = 0; i < sizeof(T); i++)
		{
			data.push_back(bytes[i]);
		}
	}

	template<typename T>
	void SerialValueImpl<T>::updateFromSerial()
	{
		_serial.Read<T>(this->_val);
	}

	template<typename T>
	void SerialArrayValue<T>::updateFromSerial()
	{
		_serial.Read(this->_size);

		if (this->_size > 0) this->_val = reinterpret_cast<T*>(realloc(this->_val, sizeof(T) * this->_size));

		for (uint16_t i = 0; i < this->_size; i++)
		{
			_serial.Read(this->_val[i]);
		}
	}

	template<typename T>
	void SerialArrayValue<T>::addToMessage(vector<unsigned char> & data) const
	{
		const unsigned char * sizeBytes = reinterpret_cast<const unsigned char *>(&(this->_size));
		data.push_back(sizeBytes[0]);
		data.push_back(sizeBytes[1]);

		uint16_t byteCount = this->_size * sizeof(T);
		const unsigned char * valBytes = reinterpret_cast<const unsigned char *>(&(this->_val));
		for (uint16_t i = 0; i < byteCount; i++)
		{
			data.push_back(valBytes[i]);
		}
	}

	/* explicit template instantiations of ValueImpl */
	template class SerialValueImpl<uint8_t> ;
	template class SerialValueImpl<int8_t> ;
	template class SerialValueImpl<uint16_t> ;
	template class SerialValueImpl<int16_t> ;
	template class SerialValueImpl<uint32_t> ;
	template class SerialValueImpl<int32_t> ;
	template class SerialValueImpl<uint64_t> ;
	template class SerialValueImpl<int64_t> ;
	template class SerialValueImpl<float> ;
	template class SerialValueImpl<double> ;

	/* explicit template instantiations of ArrayValue */
	template class SerialArrayValue<uint8_t> ;
	template class SerialArrayValue<int8_t> ;
	template class SerialArrayValue<uint16_t> ;
	template class SerialArrayValue<int16_t> ;
	template class SerialArrayValue<uint32_t> ;
	template class SerialArrayValue<int32_t> ;
	template class SerialArrayValue<uint64_t> ;
	template class SerialArrayValue<int64_t> ;
	template class SerialArrayValue<float> ;
	template class SerialArrayValue<double> ;

} /* namespace twirre */