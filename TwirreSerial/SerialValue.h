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

#ifndef SERIALVALUE_H_
#define SERIALVALUE_H_

#include <string>
#include <cstdint>
#include <vector>

#include "../TwirreSerial/SerialDevice.h"
#include "../Serial/SerialRW.h"

#include "../Core/Value.h"
#include "../Core/owned_mutex.h"


namespace twirre
{
	class SerialValue
	{
		friend class SerialActuator;
		friend class SerialSensor;
	public:
		SerialValue(uint8_t ID, SerialRW & srw);
		virtual ~SerialValue() {};
		uint8_t getID() const;
	protected:
		virtual void updateFromSerial() = 0;
		virtual void addToMessage(std::vector<unsigned char> & data) const = 0;
		const uint8_t _id;
		SerialRW & _serial;
	};

	template<typename T>
	class SerialValueImpl: public ValueImpl<T>, public SerialValue
	{
	public:
		SerialValueImpl(const uint8_t ID, const std::string name, T val, SerialRW & srw, owned_mutex * actuateMutex = nullptr);
		virtual ~SerialValueImpl() {};
	protected:
		virtual void updateFromSerial() override;
		virtual void addToMessage(std::vector<unsigned char> & data) const override;
	};

	template<typename T>
	class SerialArrayValue: public ArrayValue<T>, public SerialValue
	{
	public:
		SerialArrayValue(const uint8_t ID, const std::string name, SerialRW & srw, owned_mutex * actuateMutex = nullptr);
		virtual ~SerialArrayValue() {};
	protected:
		virtual void updateFromSerial() override;
		virtual void addToMessage(std::vector<unsigned char> & data) const override;
	};

	/* explicit template instantiations of ValueImpl */
	extern template class SerialValueImpl<uint8_t> ;
	extern template class SerialValueImpl<int8_t> ;
	extern template class SerialValueImpl<uint16_t> ;
	extern template class SerialValueImpl<int16_t> ;
	extern template class SerialValueImpl<uint32_t> ;
	extern template class SerialValueImpl<int32_t> ;
	extern template class SerialValueImpl<uint64_t> ;
	extern template class SerialValueImpl<int64_t> ;
	extern template class SerialValueImpl<float> ;
	extern template class SerialValueImpl<double> ;

	/* explicit template instantiations of ArrayValue */
	extern template class SerialArrayValue<uint8_t> ;
	extern template class SerialArrayValue<int8_t> ;
	extern template class SerialArrayValue<uint16_t> ;
	extern template class SerialArrayValue<int16_t> ;
	extern template class SerialArrayValue<uint32_t> ;
	extern template class SerialArrayValue<int32_t> ;
	extern template class SerialArrayValue<uint64_t> ;
	extern template class SerialArrayValue<int64_t> ;
	extern template class SerialArrayValue<float> ;
	extern template class SerialArrayValue<double> ;
} /* namespace twirre */

#endif /* VALUE_H_ */
