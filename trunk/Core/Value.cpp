/*
 * Value.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Value.h"

//copy paste machine
#define VALUEIMPL_GETTER(GET_T) 				\
	template <typename T>						\
	GET_T ValueImpl<T>::as_##GET_T () 		\
	{ 											\
		return static_cast<GET_T>(_val); 		\
	}

#define VALUEIMPL_SETTER(SET_T) 				\
	template <typename T>						\
	void ValueImpl<T>::set( SET_T val ) 		\
	{ 											\
		_modified = true;						\
		_val = static_cast<T>(val);				\
	}





namespace twirre
{

Value::Value(const uint8_t ID, const string n, SerialRW & serialRW) : _id(ID), _name(n), _serialRW(serialRW)
{

}


const uint8_t Value::getId()
{
	return _id;
}

const string Value::getName()
{
	return _name;
}

Parameter::Parameter(const uint8_t ID, const string n, SerialRW & serialRW) : Value(ID, n, serialRW), _modified(false)
{

}

int Value::GetSize()
{
	return 0;
}

void Value::SetBuffer(unsigned char *buffer)
{

}





template <typename T>
ValueImpl<T>::ValueImpl(const uint8_t ID, const string n, T val, SerialRW & serialRW) : Parameter(ID, n, serialRW),_val(val)
{
	//nothing left to do
}

template <typename T>
void ValueImpl<T>::UpdateFromSerial()
{
	_serialRW.Read<T>(_val);
}

VALUEIMPL_GETTER(uint8_t)
VALUEIMPL_GETTER(int8_t)
VALUEIMPL_GETTER(uint16_t)
VALUEIMPL_GETTER(int16_t)
VALUEIMPL_GETTER(uint32_t)
VALUEIMPL_GETTER(int32_t)
VALUEIMPL_GETTER(uint64_t)
VALUEIMPL_GETTER(int64_t)
VALUEIMPL_GETTER(float)
VALUEIMPL_GETTER(double)

VALUEIMPL_SETTER(uint8_t)
VALUEIMPL_SETTER(int8_t)
VALUEIMPL_SETTER(uint16_t)
VALUEIMPL_SETTER(int16_t)
VALUEIMPL_SETTER(uint32_t)
VALUEIMPL_SETTER(int32_t)
VALUEIMPL_SETTER(uint64_t)
VALUEIMPL_SETTER(int64_t)
VALUEIMPL_SETTER(float)
VALUEIMPL_SETTER(double)


/* explicit template instantiations of ValueImpl */
template class ValueImpl<uint8_t>;
template class ValueImpl<int8_t>;
template class ValueImpl<uint16_t>;
template class ValueImpl<int16_t>;
template class ValueImpl<uint32_t>;
template class ValueImpl<int32_t>;
template class ValueImpl<uint64_t>;
template class ValueImpl<int64_t>;
template class ValueImpl<float>;
template class ValueImpl<double>;



} /* namespace twirre */
