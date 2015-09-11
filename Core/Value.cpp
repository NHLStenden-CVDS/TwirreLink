/*
 * Value.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include <stdexcept>
#include <iostream>
#include <cstring>

#include "../Core/Value.h"

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
		if(_actuatorMutex) _actuatorMutex->lock();												\
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
		if(_actuatorMutex) _actuatorMutex->lock();												\
		if(size > 0)																			\
			_val = static_cast<T>(vals[0]);														\
		_modified = true;																		\
		std::cerr << "warning: tried to use array set on single-element value" << std::endl;	\
	}																							\
	template <typename T>																		\
	void ArrayValue<T>::set(const SET_T * vals, const uint16_t size)							\
	{																							\
		if(_actuatorMutex) _actuatorMutex->lock();												\
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

	template<typename T>
	constexpr NativeType _getNativeType();

	template<>
	constexpr NativeType _getNativeType<int8_t>()
	{
		return NativeType::type_int8;
	}
	template<>
	constexpr NativeType _getNativeType<uint8_t>()
	{
		return NativeType::type_uint8;
	}
	template<>
	constexpr NativeType _getNativeType<int16_t>()
	{
		return NativeType::type_int16;
	}
	template<>
	constexpr NativeType _getNativeType<uint16_t>()
	{
		return NativeType::type_uint16;
	}
	template<>
	constexpr NativeType _getNativeType<int32_t>()
	{
		return NativeType::type_int32;
	}
	template<>
	constexpr NativeType _getNativeType<uint32_t>()
	{
		return NativeType::type_uint32;
	}
	template<>
	constexpr NativeType _getNativeType<int64_t>()
	{
		return NativeType::type_int64;
	}
	template<>
	constexpr NativeType _getNativeType<uint64_t>()
	{
		return NativeType::type_uint64;
	}
	template<>
	constexpr NativeType _getNativeType<float>()
	{
		return NativeType::type_float;
	}
	template<>
	constexpr NativeType _getNativeType<double>()
	{
		return NativeType::type_double;
	}

	Value::Value(const string n) :
			_name(n)
	{
	}

	const string& Value::getName()
	{
		return _name;
	}

	Parameter::Parameter(const string n) :
			Parameter(n, nullptr)
	{
	}

	Parameter::Parameter(const string n, owned_mutex * actuatorMutex) :
			Value(n), _modified(false), _actuatorMutex(actuatorMutex)
	{
	}

	bool Parameter::isModified() const
	{
		return _modified;
	}

	void Parameter::resetModified()
	{
		_modified = false;
	}

	template<typename T>
	NativeType ValueImpl<T>::getNativeType()
	{
		return _getNativeType<T>();
	}

	template<typename T>
	T& ValueImpl<T>::nativeValue()
	{
		return _val;
	}

	template<typename T>
	ValueImpl<T>::ValueImpl(const string n, T val) :
			Parameter(n), _val(val)
	{
	}

	template<typename T>
	ValueImpl<T>::ValueImpl(const string n, T val, owned_mutex * actuatorMutex) :
			Parameter(n, actuatorMutex), _val(val)
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
		parm->set(_val);
	}

	template<typename T>
	uint16_t ValueImpl<T>::getSize() const
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

	template<typename T>
	NativeType ArrayValue<T>::getNativeType()
	{
		return _getNativeType<T>();
	}

	template<typename T>
	T*& ArrayValue<T>::nativeValue()
	{
		return _val;
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(const string name) :
			Parameter(name), _val(nullptr), _size(0)
	{
	}

	template<typename T>
	ArrayValue<T>::ArrayValue(const string name, owned_mutex * actuatorMutex) :
			Parameter(name, actuatorMutex), _val(nullptr), _size(0)
	{
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
	uint16_t ArrayValue<T>::getSize() const
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
	void ArrayValue<T>::setSize(uint16_t size)
	{
		if (_actuatorMutex) _actuatorMutex->lock();
		_size = size;
		_val = reinterpret_cast<T*>(realloc(_val, size * sizeof(T)));
		_modified = true;
	}

	template<typename T>
	void ArrayValue<T>::setNative(T* data, uint16_t size)
	{
		if (_actuatorMutex) _actuatorMutex->lock();
		_val = reinterpret_cast<T*>(realloc(_val, size * sizeof(T)));
		_size = size;
		memcpy(_val, data, size * sizeof(T));
		_modified = true;
	}

	ErrorValue *ErrorValue::_instance = nullptr;
	ErrorValue * ErrorValue::getInstance()
	{
		if (_instance == nullptr) _instance = new ErrorValue("<error>");
		return _instance;
	}

	ErrorValue::ErrorValue(const string n) :
			Parameter(n)
	{
	}

	NativeType ErrorValue::getNativeType()
	{
		return NativeType::type_void;
	}

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

	size_t ErrorValue::getElementSize() const
	{
		return 0;
	}

	void * ErrorValue::getBuffer()
	{
		return nullptr;
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

} /* namespace twirre */
