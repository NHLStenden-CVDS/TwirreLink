/*
 * Value.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#ifndef SERIALVALUE_H_
#define SERIALVALUE_H_

#include <string>
#include <cstdint>
#include <vector>

#include "TwirreSerial/SerialDevice.h"
#include "Serial/SerialRW.h"

#include "Core/Value.h"

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
		SerialValueImpl(const uint8_t ID, const std::string name, T val, SerialRW & srw);
		virtual ~SerialValueImpl() {};
	protected:
		virtual void updateFromSerial() override;
		virtual void addToMessage(std::vector<unsigned char> & data) const override;
	};

	template<typename T>
	class SerialArrayValue: public ArrayValue<T>, public SerialValue
	{
	public:
		SerialArrayValue(const uint8_t ID, const std::string name, SerialRW & srw);
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
