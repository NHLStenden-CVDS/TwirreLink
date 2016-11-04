/*
 * ValueImpl.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: mrd
 */

#include <string>
#include <iostream>

#include "ValueImpl.h"

using namespace std;


//copy paste machine
//as_... functions for the Value implementations
#define VALUEIMPL_GETTER(GET_T) 																\
	template <typename T>																		\
	GET_T ValueImpl<T>::as_##GET_T () 															\
	{ 																							\
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);								\
		return static_cast<GET_T>(_val); 														\
	}																							\
	template <typename T>																		\
	GET_T ValueImpl<T>::as_##GET_T (uint32_t id)												\
	{																							\
		if(id > 0) throw std::out_of_range("index out of bounds on single-element value"); 		\
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);									\
		return as_##GET_T ();																	\
	}

//set(...) functions for the Value implementations
#define VALUEIMPL_SETTER(SET_T) 																\
	template <typename T>																		\
	void ValueImpl<T>::set(const SET_T val ) 													\
	{ 																							\
		if(_actuatorMutex) _actuatorMutex->lock();												\
		std::unique_lock<std::shared_timed_mutex> rwLock(_rwMutex);								\
		_modified = true;																		\
		_val = static_cast<T>(val);																\
	}																							\
																								\
	template <typename T>																		\
	void ValueImpl<T>::set(const SET_T * vals, const uint32_t size)								\
	{																							\
		if(_actuatorMutex) _actuatorMutex->lock();												\
		std::unique_lock<std::shared_timed_mutex> rwLock(_rwMutex);								\
		if(size > 0)																			\
			_val = static_cast<T>(vals[0]);														\
		_modified = true;																		\
		std::cerr << "warning: tried to use array set on single-element value" << std::endl;	\
	}																							\
	template <typename T>																		\
	void ValueImpl<T>::set(const vector<SET_T>& vals)											\
	{																							\
		set(vals.data(), static_cast<const uint32_t>(vals.size()));								\
	}

namespace twirre
{
template<typename T>
	NativeType ValueImpl<T>::getNativeType()
	{
		return _getNativeType<T>();
	}

	template<typename T>
	T ValueImpl<T>::getNativeValue()
	{
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		return _val;
	}

	template<typename T>
	void ValueImpl<T>::setNativeValue(T val)
	{
		std::unique_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		_val = val;
	}

	template<typename T>
	ValueImpl<T>::ValueImpl(const string n, const T val) :
			ValueImpl(n, val, nullptr)
	{
	}

	template<typename T>
	ValueImpl<T>::ValueImpl(const string n, const T val, const T min, const T max) :
			ValueImpl(n, val, min, max, nullptr)
	{
	}

	template<typename T>
	ValueImpl<T>::ValueImpl(const string n, const T val, owned_mutex * actuatorMutex) :
			ValueImpl(n, val, std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max(), actuatorMutex)
	{
	}

	template<typename T>
	ValueImpl<T>::ValueImpl(const string n, const T val, const T min, const T max, owned_mutex * actuatorMutex) :
			Parameter(n, actuatorMutex), _val(val), _min(min), _max(max)
	{
	}

	template<typename T>
	bool ValueImpl<T>::isValid() const
	{
		return true;
	}

	template<typename T>
	bool ValueImpl<T>::isArray() const
	{
		return false;
	}

	template<typename T>
	void ValueImpl<T>::copyTo(Parameter * parm) const
	{
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		parm->set(_val);
	}

	template<typename T>
	Scalar& ValueImpl<T>::getMin()
	{
		return _min;
	}

	template<typename T>
	Scalar& ValueImpl<T>::getMax()
	{
		return _max;
	}

	template<typename T>
	uint32_t ValueImpl<T>::getSize() const
	{
		return 1;
	}

	template<typename T>
	size_t ValueImpl<T>::getElementSize() const
	{
		return sizeof(T);
	}

	template<typename T>
	void * ValueImpl<T>::getBuffer()
	{
		return reinterpret_cast<void *>(&_val);
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

	template<typename T>
	std::string ValueImpl<T>::as_string()
	{
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		return to_string(_val);
	}

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
		std::unique_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		//The underlying value type of val is unknown, so it's needed to call its copyTo function
		//(which will in turn call the correct set(...) function of this object)
		val.copyTo(this);
	}

	/* explicit template instantiations of ValueImpl */
	template class ValueImpl<uint8_t> ;
	template class ValueImpl<int8_t> ;
	template class ValueImpl<uint16_t> ;
	template class ValueImpl<int16_t> ;
	template class ValueImpl<uint32_t> ;
	template class ValueImpl<int32_t> ;
	template class ValueImpl<uint64_t> ;
	template class ValueImpl<int64_t> ;
	template class ValueImpl<float> ;
	template class ValueImpl<double> ;
}
