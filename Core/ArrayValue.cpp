/*
 * ArrayValue.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: mrd
 */

#include <cstring>
#include <cstdio>

#include "ArrayValue.h"

using namespace std;

//copy paste machine
//as_... functions for the Value implementations
#define ARRAYVALUE_GETTER(GET_T) 																\
	template <typename T>																		\
	GET_T ArrayValue<T>::as_##GET_T ()															\
	{																							\
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);								\
		if(_size == 0) return GET_T(0);															\
		return static_cast<GET_T>(_val[0]);														\
	}																							\
	template <typename T>																		\
	GET_T ArrayValue<T>::as_##GET_T (uint32_t id)												\
	{																							\
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);								\
		if(id >= _size) throw std::out_of_range("index out of bounds on value array"); 			\
		return static_cast<GET_T>(_val[id]);													\
	}																							\

//set(...) functions for the Value implementations
#define ARRAYVALUE_SETTER(SET_T) 																\
	template <typename T>																		\
	void ArrayValue<T>::set(const SET_T val)													\
	{																							\
		set(&val, 1);																			\
	}																							\
	template <typename T>																		\
	void ArrayValue<T>::set(const SET_T * vals, const uint32_t size)							\
	{																							\
		if(_actuatorMutex) _actuatorMutex->lock();												\
		std::unique_lock<std::shared_timed_mutex> rwLock(_rwMutex);								\
		_modified = true;																		\
		_size = size;																			\
		if(_size != 0)																			\
		{																						\
			_val = reinterpret_cast<T*>(realloc(_val, size * sizeof(T)));						\
			if(std::is_same<T, SET_T>::value)													\
			{																					\
				std::memcpy(_val, vals, size * sizeof(T));										\
			}																					\
			else																				\
			{																					\
				for(uint32_t i = 0; i < size; i++)												\
				{																				\
					_val[i] = static_cast<T>(vals[i]);											\
				}																				\
			}																					\
		}																						\
		else																					\
		{																						\
			free(_val);																			\
			_val = nullptr;																		\
		}																						\
	}																							\
	template <typename T>																		\
	void ArrayValue<T>::set(const vector<SET_T>& vals)											\
	{																							\
		set(vals.data(), static_cast<const uint32_t>(vals.size()));								\
	}

namespace twirre
{

	template<typename T>
	NativeType ArrayValue<T>::getNativeType()
	{
		return _getNativeType<T>();
	}

	template<typename T>
	T* ArrayValue<T>::nativeValue()
	{
		return _val;
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(const string name) :
			ArrayValue(name, nullptr)
	{
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(const string name, const uint32_t size, const T defaultValue) :
			ArrayValue(name, nullptr, size, defaultValue)
	{
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(const string name, const uint32_t size, const T* defaultArray) :
			ArrayValue(name, nullptr, size, defaultArray)
	{
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(const string name, owned_mutex * actuatorMutex) :
			Parameter(name, actuatorMutex), _val(nullptr), _size(0)
	{
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(const string name, owned_mutex * actuatorMutex, const uint32_t size, const T defaultValue) :
			Parameter(name, actuatorMutex), _val(reinterpret_cast<T*>(malloc(size * sizeof(T)))), _size(size)
	{
		for (uint32_t i = 0; i < size; i++)
		{
			_val[i] = defaultValue;
		}
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(const std::string name, owned_mutex * actuatorMutex, const uint32_t size, const T* defaultArray) :
			Parameter(name, actuatorMutex), _val(reinterpret_cast<T*>(malloc(size * sizeof(T)))), _size(size)
	{
		std::memcpy(_val, defaultArray, size * sizeof(T));
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(const ArrayValue<T> & other) :
			Parameter(other._name, other._actuatorMutex), _size(other._size)
	{
		//make a copy of the _value buffer
		if (_size > 0)
		{
			_val = reinterpret_cast<T*>(malloc(_size * sizeof(T)));
			memcpy(_val, other._val, _size * sizeof(T));
		}
		else
			_val = nullptr;
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(ArrayValue<T> && other) noexcept : Parameter(other._name, other._actuatorMutex), _size(other._size)
	{
		//swap pointers
		_val = other._val;
		other._val = nullptr;
		other._size = 0;
	}

	template<typename T>
	ArrayValue<T> & ArrayValue<T>::operator =(const ArrayValue<T> & other)
	{
		ArrayValue<T> tmp(other); // re-use copy-constructor
		*this = std::move(tmp); // re-use move-assignment
		return *this;
	}

	template<typename T>
	ArrayValue<T> & ArrayValue<T>::operator =(ArrayValue<T> && other) noexcept
	{
		if (_actuatorMutex) _actuatorMutex->lock();
		std::unique_lock<std::shared_timed_mutex> rwLock(_rwMutex);

		//need to swap data
		std::swap(_val, other._val);

		//can move other stuff
		_size = std::move(other._size);
		_name = std::move(other._name);

		//set modified
		_modified = true;

		return *this;
	}

	template<typename T>
	ArrayValue<T>::~ArrayValue() noexcept
	{
		delete _val;
	}

	template<typename T>
	bool ArrayValue<T>::isValid() const
	{
		return true;
	}

	template<typename T>
	bool ArrayValue<T>::isArray() const
	{
		return true;
	}

	template<typename T>
	void ArrayValue<T>::copyTo(Parameter * parm) const
	{
		if (parm->isArray())
			parm->set(_val, _size);
		else if (_size > 0)
			parm->set(_val[0]);
		else
		{
			if (parm->_actuatorMutex) parm->_actuatorMutex->lock();
			parm->_modified = true;
		}
	}

	template<typename T>
	Scalar& ArrayValue<T>::getMin()
	{
		return _min;
	}

	template<typename T>
	Scalar& ArrayValue<T>::getMax()
	{
		return _max;
	}

	template<typename T>
	uint32_t ArrayValue<T>::getSize() const
	{
		return _size;
	}

	template<typename T>
	size_t ArrayValue<T>::getElementSize() const
	{
		return sizeof(T);
	}

	template<typename T>
	void* ArrayValue<T>::getBuffer()
	{
		return reinterpret_cast<void*>(_val);
	}

	template<typename T>
	T* ArrayValue<T>::getNativeBuffer()
	{
		return _val;
	}

	template<typename T>
	void ArrayValue<T>::setSize(uint32_t size)
	{
		if (_actuatorMutex) _actuatorMutex->lock();
		_size = size;
		_val = reinterpret_cast<T*>(realloc(_val, size * sizeof(T)));
		_modified = true;
	}

	template<typename T>
	void ArrayValue<T>::setNative(T* data, uint32_t size)
	{
		if (_actuatorMutex) _actuatorMutex->lock();
		std::unique_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		_val = reinterpret_cast<T*>(realloc(_val, size * sizeof(T)));
		_size = size;
		memcpy(_val, data, size * sizeof(T));
		_modified = true;
	}

	template<typename T>
	T ArrayValue<T>::getNative(uint32_t id)
	{
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		return _val[id];
	}

	template<typename T>
	T ArrayValue<T>::operator [](uint32_t id)
	{
		return getNative(id);
	}

	ARRAYVALUE_GETTER(uint8_t)
	ARRAYVALUE_GETTER(int8_t)
	ARRAYVALUE_GETTER(uint16_t)
	ARRAYVALUE_GETTER(int16_t)
	ARRAYVALUE_GETTER(uint32_t)
	ARRAYVALUE_GETTER(int32_t)
	ARRAYVALUE_GETTER(uint64_t)
	ARRAYVALUE_GETTER(int64_t)
	ARRAYVALUE_GETTER(float)
	ARRAYVALUE_GETTER(double)

	template<typename T>
	std::string ArrayValue<T>::as_string()
	{
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		//for now, get underlying pointer, treat it as char* (forcing last byte to 0), and return that as string (restoring the last byte of the original value)
		char* str = reinterpret_cast<char*>(_val);
		size_t bytes = _size * (sizeof(T) / sizeof(char));
		char lastByte = str[bytes - 1];
		str[bytes - 1] = 0;
		std::string retStr(str);
		str[bytes - 1] = lastByte;
		return retStr;
	}

	ARRAYVALUE_SETTER(uint8_t)
	ARRAYVALUE_SETTER(int8_t)
	ARRAYVALUE_SETTER(uint16_t)
	ARRAYVALUE_SETTER(int16_t)
	ARRAYVALUE_SETTER(uint32_t)
	ARRAYVALUE_SETTER(int32_t)
	ARRAYVALUE_SETTER(uint64_t)
	ARRAYVALUE_SETTER(int64_t)
	ARRAYVALUE_SETTER(float)
	ARRAYVALUE_SETTER(double)

	template<typename T>
	void ArrayValue<T>::set(const Value& val)
	{
		std::unique_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		//The underlying value type of val is unknown, so it's needed to call its copyTo function
		//(which will in turn call the correct set(...) function of this object)
		val.copyTo(this);
	}

	/* explicit template instantiations of ArrayValue */
	template class ArrayValue<uint8_t> ;
	template class ArrayValue<int8_t> ;
	template class ArrayValue<uint16_t> ;
	template class ArrayValue<int16_t> ;
	template class ArrayValue<uint32_t> ;
	template class ArrayValue<int32_t> ;
	template class ArrayValue<uint64_t> ;
	template class ArrayValue<int64_t> ;
	template class ArrayValue<float> ;
	template class ArrayValue<double> ;
}
