/*
 * Value.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Value.h"
#include <stdexcept>
#include <iostream>
#include <cstring>

using namespace std;

//copy paste machine
//as_... functions for the Value implementations
#define VALUEIMPL_GETTER(GET_T) 																\
	template <typename T>																		\
	GET_T ValueImpl<T>::as_##GET_T () 															\
	{ 																							\
		return static_cast<GET_T>(_val); 														\
	}																							\
	template <typename T>																		\
	GET_T ArrayValue<T>::as_##GET_T ()															\
	{																							\
		if(_size == 0) return GET_T(0);															\
		return static_cast<GET_T>(_val[0]);														\
	}																							\
	GET_T ErrorValue::as_##GET_T ()																\
	{																							\
		return GET_T(0);																		\
	}																							\
																								\
	template <typename T>																		\
	GET_T ValueImpl<T>::as_##GET_T (uint16_t id)												\
	{																							\
		if(id > 0) throw std::out_of_range("index out of bounds on single-element value"); 		\
		return as_##GET_T ();																	\
	}																							\
	template <typename T>																		\
	GET_T ArrayValue<T>::as_##GET_T (uint16_t id)												\
	{																							\
		if(id >= _size) throw std::out_of_range("index out of bounds on value array"); 			\
		return static_cast<GET_T>(_val[id]);													\
	}																							\
	GET_T ErrorValue::as_##GET_T (uint16_t)														\
	{																							\
		throw std::out_of_range("tried to access ErrorValue by index"); 						\
		return GET_T(0); /*keep compiler happy*/												\
	}

//set(...) functions for the Value implementations
#define VALUEIMPL_SETTER(SET_T) 																\
	template <typename T>																		\
	void ValueImpl<T>::set(const SET_T val ) 													\
	{ 																							\
		_modified = true;																		\
		_val = static_cast<T>(val);																\
	}																							\
	template <typename T>																		\
	void ArrayValue<T>::set(const SET_T val)													\
	{																							\
		set(&val, 1);																			\
	}																							\
	void ErrorValue::set(const SET_T )															\
	{}																							\
																								\
	template <typename T>																		\
	void ValueImpl<T>::set(const SET_T * vals, const uint16_t size)								\
	{																							\
		if(size > 0)																			\
			_val = static_cast<T>(vals[0]);														\
		_modified = true;																		\
		std::cerr << "warning: tried to use array set on single-element value" << std::endl;	\
	}																							\
	template <typename T>																		\
	void ArrayValue<T>::set(const SET_T * vals, const uint16_t size)							\
	{																							\
		_modified = true;																		\
		_size = size;																			\
		if(_size != 0)																			\
		{																						\
			_val = reinterpret_cast<T*>(realloc(_val, size * sizeof(T)));						\
			for(uint16_t i = 0; i < size; i++)													\
			{																					\
				_val[i] = static_cast<T>(vals[i]);												\
			}																					\
		}																						\
	}																							\
	void ErrorValue::set(const SET_T *, const uint16_t)											\
	{																							\
		std::cerr << "warning: tried to use array set on error value" << std::endl;				\
	}																							\
																								\
	template <typename T>																		\
	void ValueImpl<T>::set(const vector<SET_T>& vals)											\
	{																							\
		set(vals.data(), static_cast<const uint16_t>(vals.size()));								\
	}																							\
	template <typename T>																		\
	void ArrayValue<T>::set(const vector<SET_T>& vals)											\
	{																							\
		set(vals.data(), static_cast<const uint16_t>(vals.size()));								\
	}																							\
	void ErrorValue::set(const vector<SET_T>& vals)												\
	{																							\
		set(vals.data(), static_cast<const uint16_t>(vals.size()));								\
	}

namespace twirre
{

Value::Value(const uint8_t ID, const string n, SerialRW * serialRW) : _id(ID), _name(n), _serialRW(serialRW)
{ }

uint8_t Value::getId()
{
	return _id;
}

const string& Value::getName()
{
	return _name;
}

Parameter::Parameter(const uint8_t ID, const string n, SerialRW * serialRW) : Value(ID, n, serialRW), _modified(false)
{ }

void Parameter::resetModified()
{
	_modified = false;
}

template <typename T>
ValueImpl<T>::ValueImpl(const uint8_t ID, const string n, T val, SerialRW * serialRW) : Parameter(ID, n, serialRW),_val(val)
{ }

template <typename T>
void ValueImpl<T>::addToMessage(vector<unsigned char> &data) const
{
	data.push_back(_id);
	const unsigned char * bytes = reinterpret_cast<const unsigned char*>(&_val);
	for(size_t i = 0; i < sizeof(_val); i++)
	{
		data.push_back(bytes[i]);
	}
}

template <typename T>
void ValueImpl<T>::UpdateFromSerial()
{
	_serialRW->Read<T>(_val);
}

template <typename T>
bool ValueImpl<T>::isValid() const
{
	return true;
}

template <typename T>
bool ValueImpl<T>::isArray() const
{
	return false;
}

template <typename T>
void ValueImpl<T>::copyTo(Parameter * parm) const
{
	//TODO: handle copy to array
	parm->set(_val);
}

template <typename T>
uint16_t ValueImpl<T>::getSize() const
{
	return 1;
}

template <typename T>
ArrayValue<T>::ArrayValue(const uint8_t ID, const string name, SerialRW * serialRW) : Parameter(ID, name, serialRW), _val(nullptr), _size(0)
{ }

template <typename T>
ArrayValue<T>::ArrayValue(const ArrayValue<T> & other) : Parameter(other._id, other._name, other._serialRW), _size(other._size)
{
	//make a copy of the _value buffer
	if(_size > 0)
	{
		_val = reinterpret_cast<T*>(malloc(_size * sizeof(T)));
		memcpy(_val, other._val, _size * sizeof(T));
	}
	else _val = nullptr;
}

template <typename T>
ArrayValue<T>::ArrayValue(ArrayValue<T> && other) noexcept : Parameter(other._id, other._name, other._serialRW), _size(other._size)
{
	//swap pointers
	_val = other._val;
	other._val = nullptr;
	other._size = 0;
}

template <typename T>
ArrayValue<T> & ArrayValue<T>::operator = (const ArrayValue<T> & other)
{
	ArrayValue<T> tmp(other); // re-use copy-constructor
	*this = std::move(tmp); // re-use move-assignment
	return *this;
}

template <typename T>
ArrayValue<T> & ArrayValue<T>::operator = (ArrayValue<T> && other) noexcept
{
	//need to swap data
	std::swap(_val, other._val);

	//can move other stuff
	_size = std::move(other._size);
	_id = std::move(other._id);
	_name = std::move(other._name);
	_serialRW = std::move(other._serialRW);

	return *this;
}


template <typename T>
ArrayValue<T>::~ArrayValue() noexcept
{
	delete _val;
}

template <typename T>
void ArrayValue<T>::UpdateFromSerial()
{
	_serialRW->Read(_size);

	if(_size > 0)
		_val = reinterpret_cast<T*>(realloc(_val, sizeof(T) * _size));

	for(uint16_t i = 0; i < _size; i++)
	{
		_serialRW->Read(_val[i]);
	}
}

template <typename T>
bool ArrayValue<T>::isValid() const
{
	return true;
}

template <typename T>
bool ArrayValue<T>::isArray() const
{
	return true;
}

template <typename T>
void ArrayValue<T>::copyTo(Parameter * parm) const
{
	if(parm->isArray())
		parm->set(_val, _size);
	else if(_size > 0)
		parm->set(_val[0]);
	else
		parm->_modified = true;
}

template <typename T>
uint16_t ArrayValue<T>::getSize() const
{
	return _size;
}

template <typename T>
void ArrayValue<T>::addToMessage(vector<unsigned char> & data) const
{
	const unsigned char * sizeBytes = reinterpret_cast<const unsigned char *> (&_size);
	data.push_back(sizeBytes[0]);
	data.push_back(sizeBytes[1]);

	uint16_t byteCount = _size * sizeof(T);
	const unsigned char * valBytes = reinterpret_cast<const unsigned char *> (&_val);
	for(uint16_t i = 0; i < byteCount; i++)
	{
		data.push_back(valBytes[i]);
	}
}

ErrorValue *ErrorValue::_instance = nullptr;
ErrorValue * ErrorValue::getInstance()
{
	if(_instance == nullptr)
		_instance = new ErrorValue(0xFF, "<error>", nullptr);
	return _instance;
}

ErrorValue::ErrorValue(const uint8_t ID, const string n, SerialRW * serialRW) : Parameter(ID, n, serialRW)
{ }

bool ErrorValue::isValid() const
{
	return false;
}

bool ErrorValue::isArray() const
{
	return false;
}

uint16_t ErrorValue::getSize() const
{
	return 0;
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

template<typename T>
void ValueImpl<T>::set(const Value& val)
{
	//The underlying value type of val is unknown, so it's needed to call its copyTo function
	//(which will in turn call the correct set(...) function of this object)
	val.copyTo(this);
}

template<typename T>
void ArrayValue<T>::set(const Value& val)
{
	//The underlying value type of val is unknown, so it's needed to call its copyTo function
	//(which will in turn call the correct set(...) function of this object)
	val.copyTo(this);
}

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

/* explicit template instantiations of ArrayValue */
template class ArrayValue<uint8_t>;
template class ArrayValue<int8_t>;
template class ArrayValue<uint16_t>;
template class ArrayValue<int16_t>;
template class ArrayValue<uint32_t>;
template class ArrayValue<int32_t>;
template class ArrayValue<uint64_t>;
template class ArrayValue<int64_t>;
template class ArrayValue<float>;
template class ArrayValue<double>;

} /* namespace twirre */
