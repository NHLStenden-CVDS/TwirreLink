/*
 * Twirre: architecture for autonomous UAVs using interchangeable commodity components
 *
 * Copyright© 2017 Centre of expertise in Computer Vision & Data Science, NHL Stenden University of applied sciences
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdexcept>
#include <iostream>
#include <cstring>

#include "../Core/Value.h"

using namespace std;

//copy paste machine
//as_... functions for the Value implementations
#define VALUE_GETTER(GET_T) 																	\
	GET_T ErrorValue::as_##GET_T ()																\
	{																							\
		return GET_T(0);																		\
	}																							\
																								\
	GET_T ErrorValue::as_##GET_T (uint32_t)														\
	{																							\
		throw std::out_of_range("tried to access ErrorValue by index"); 						\
		return GET_T(0); /*keep compiler happy*/												\
	}																							\
																								\
	template <typename T>																		\
	GET_T ScalarImpl<T>::as_##GET_T ()															\
	{																							\
		std::shared_lock<decltype(_rwMutex)> rwLock(_rwMutex);									\
		return static_cast<GET_T>(_val);														\
	}

//set(...) functions for the Value implementations
#define VALUE_SETTER(SET_T) 																	\
	void ErrorValue::set(const SET_T )															\
	{}																							\
																								\
	void ErrorValue::set(const SET_T *, const uint32_t)											\
	{																							\
		std::cerr << "warning: tried to use array set on error value" << std::endl;				\
	}																							\
																								\
	void ErrorValue::set(const vector<SET_T>& vals)												\
	{																							\
		set(vals.data(), static_cast<const uint32_t>(vals.size()));								\
	}

namespace twirre
{

	template<typename T>
	ScalarImpl<T>::ScalarImpl(const T val) :
			_val(val)
	{

	}

	template<typename T>
	NativeType ScalarImpl<T>::getNativeType()
	{
		return _getNativeType<T>();
	}

	template<typename T>
	void ScalarImpl<T>::set(const T val)
	{
		_val = val;
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

	void Parameter::setActuatorMutex(owned_mutex * actuatorMutex)
	{
		_actuatorMutex = actuatorMutex;
	}

	ErrorValue *ErrorValue::_instance = nullptr;
	ErrorValue * ErrorValue::getInstance()
	{
		if (_instance == nullptr) _instance = new ErrorValue("<error>");
		return _instance;
	}

	ErrorValue::ErrorValue(const string n) :
			Parameter(n), _min(0), _max(0)
	{
	}

	NativeType ErrorValue::getNativeType()
	{
		return NativeType::type_void;
	}

	std::string ErrorValue::as_string()
	{
		return "<error>";
	}

	bool ErrorValue::isValid() const
	{
		return false;
	}

	bool ErrorValue::isArray() const
	{
		return false;
	}

	Scalar& ErrorValue::getMin()
	{
		return _min;
	}

	Scalar& ErrorValue::getMax()
	{
		return _max;
	}

	uint32_t ErrorValue::getSize() const
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

	VALUE_GETTER(uint8_t)
	VALUE_GETTER(int8_t)
	VALUE_GETTER(uint16_t)
	VALUE_GETTER(int16_t)
	VALUE_GETTER(uint32_t)
	VALUE_GETTER(int32_t)
	VALUE_GETTER(uint64_t)
	VALUE_GETTER(int64_t)
	VALUE_GETTER(float)
	VALUE_GETTER(double)

	template<typename T>
	std::string ScalarImpl<T>::as_string()
	{
		std::shared_lock<std::shared_timed_mutex> rwLock(_rwMutex);
		return to_string(_val);
	}

	VALUE_SETTER(uint8_t)
	VALUE_SETTER(int8_t)
	VALUE_SETTER(uint16_t)
	VALUE_SETTER(int16_t)
	VALUE_SETTER(uint32_t)
	VALUE_SETTER(int32_t)
	VALUE_SETTER(uint64_t)
	VALUE_SETTER(int64_t)
	VALUE_SETTER(float)
	VALUE_SETTER(double)

	/* explicit template instantiations of ScalarImpl */
	template class ScalarImpl<uint8_t> ;
	template class ScalarImpl<int8_t> ;
	template class ScalarImpl<uint16_t> ;
	template class ScalarImpl<int16_t> ;
	template class ScalarImpl<uint32_t> ;
	template class ScalarImpl<int32_t> ;
	template class ScalarImpl<uint64_t> ;
	template class ScalarImpl<int64_t> ;
	template class ScalarImpl<float> ;
	template class ScalarImpl<double> ;

} /* namespace twirre */
