/*
 * Value.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include <cstdint>
#include <vector>
#include "SerialRW.h"

namespace twirre
{
	class Parameter;

	class Value
	{
		friend class Sensor;
		friend class Actuator;
		template<typename T> friend class ValueImpl;
		template<typename T> friend class ArrayValue;
	public:

		Value(const uint8_t ID, const std::string name, SerialRW * serialRW);
		virtual ~Value()
		{
		}

		int GetSize();
		void SetBuffer(unsigned char *buffer);

		virtual uint8_t as_uint8_t() = 0;
		virtual int8_t as_int8_t() = 0;
		virtual uint16_t as_uint16_t() = 0;
		virtual int16_t as_int16_t() = 0;
		virtual uint32_t as_uint32_t() = 0;
		virtual int32_t as_int32_t() = 0;
		virtual uint64_t as_uint64_t() = 0;
		virtual int64_t as_int64_t() = 0;
		virtual float as_float() = 0;
		virtual double as_double() = 0;

		virtual uint8_t as_uint8_t(uint16_t index) = 0;
		virtual int8_t as_int8_t(uint16_t index) = 0;
		virtual uint16_t as_uint16_t(uint16_t index) = 0;
		virtual int16_t as_int16_t(uint16_t index) = 0;
		virtual uint32_t as_uint32_t(uint16_t index) = 0;
		virtual int32_t as_int32_t(uint16_t index) = 0;
		virtual uint64_t as_uint64_t(uint16_t index) = 0;
		virtual int64_t as_int64_t(uint16_t index) = 0;
		virtual float as_float(uint16_t index) = 0;
		virtual double as_double(uint16_t index) = 0;

		uint8_t getId();
		const std::string& getName();

		virtual uint16_t getSize() const = 0;

		virtual bool isValid() const = 0;
		virtual bool isArray() const = 0;
	protected:
		virtual void UpdateFromSerial() = 0;
		virtual void copyTo(Parameter* const parm) const = 0;
		uint8_t _id;
		std::string _name;
		SerialRW * _serialRW;

	};

	class Parameter: public Value
	{
		friend class Actuator;
		template <typename T> friend class ArrayValue;
	public:
		Parameter(const uint8_t ID, const std::string name, SerialRW * serialRW);
		virtual ~Parameter()
		{
		}

		virtual void set(const uint8_t val) = 0;
		virtual void set(const int8_t val) = 0;
		virtual void set(const uint16_t val) = 0;
		virtual void set(const int16_t val) = 0;
		virtual void set(const uint32_t val) = 0;
		virtual void set(const int32_t val) = 0;
		virtual void set(const uint64_t val) = 0;
		virtual void set(const int64_t val) = 0;
		virtual void set(const float val) = 0;
		virtual void set(const double val) = 0;
		virtual void set(const Value& val) = 0;

		virtual void set(const uint8_t * vals, const uint16_t size) = 0;
		virtual void set(const int8_t * vals, const uint16_t size) = 0;
		virtual void set(const uint16_t * vals, const uint16_t size) = 0;
		virtual void set(const int16_t * vals, const uint16_t size) = 0;
		virtual void set(const uint32_t * vals, const uint16_t size) = 0;
		virtual void set(const int32_t * vals, const uint16_t size) = 0;
		virtual void set(const uint64_t * vals, const uint16_t size) = 0;
		virtual void set(const int64_t * vals, const uint16_t size) = 0;
		virtual void set(const float * vals, const uint16_t size) = 0;
		virtual void set(const double * vals, const uint16_t size) = 0;

		virtual void set(const std::vector<uint8_t>& vals) = 0;
		virtual void set(const std::vector<int8_t>& vals) = 0;
		virtual void set(const std::vector<uint16_t>& vals) = 0;
		virtual void set(const std::vector<int16_t>& vals) = 0;
		virtual void set(const std::vector<uint32_t>& vals) = 0;
		virtual void set(const std::vector<int32_t>& vals) = 0;
		virtual void set(const std::vector<uint64_t>& vals) = 0;
		virtual void set(const std::vector<int64_t>& vals) = 0;
		virtual void set(const std::vector<float>& vals) = 0;
		virtual void set(const std::vector<double>& vals) = 0;

		template<typename T>
		Parameter& operator =(const T & value)
		{
			set(value);
			return *this;
		}

	protected:
		virtual void addToMessage(std::vector<unsigned char> &data) const = 0;
		void resetModified();
		bool _modified;
	};

	template<typename T>
	class ValueImpl: public Parameter
	{
	public:
		ValueImpl(const uint8_t ID, const std::string name, T val, SerialRW * serialRW);
		virtual ~ValueImpl()
		{
		}

		virtual uint8_t as_uint8_t() override;
		virtual int8_t as_int8_t() override;
		virtual uint16_t as_uint16_t() override;
		virtual int16_t as_int16_t() override;
		virtual uint32_t as_uint32_t() override;
		virtual int32_t as_int32_t() override;
		virtual uint64_t as_uint64_t() override;
		virtual int64_t as_int64_t() override;
		virtual float as_float() override;
		virtual double as_double() override;

		virtual uint8_t as_uint8_t(uint16_t index) override;
		virtual int8_t as_int8_t(uint16_t index) override;
		virtual uint16_t as_uint16_t(uint16_t index) override;
		virtual int16_t as_int16_t(uint16_t index) override;
		virtual uint32_t as_uint32_t(uint16_t index) override;
		virtual int32_t as_int32_t(uint16_t index) override;
		virtual uint64_t as_uint64_t(uint16_t index) override;
		virtual int64_t as_int64_t(uint16_t index) override;
		virtual float as_float(uint16_t index) override;
		virtual double as_double(uint16_t index) override;

		virtual void set(const uint8_t val) override;
		virtual void set(const int8_t val) override;
		virtual void set(const uint16_t val) override;
		virtual void set(const int16_t val) override;
		virtual void set(const uint32_t val) override;
		virtual void set(const int32_t val) override;
		virtual void set(const uint64_t val) override;
		virtual void set(const int64_t val) override;
		virtual void set(const float val) override;
		virtual void set(const double val) override;
		virtual void set(const Value& val) override;

		virtual void set(const uint8_t * vals, const uint16_t size) override;
		virtual void set(const int8_t * vals, const uint16_t size) override;
		virtual void set(const uint16_t * vals, const uint16_t size) override;
		virtual void set(const int16_t * vals, const uint16_t size) override;
		virtual void set(const uint32_t * vals, const uint16_t size) override;
		virtual void set(const int32_t * vals, const uint16_t size) override;
		virtual void set(const uint64_t * vals, const uint16_t size) override;
		virtual void set(const int64_t * vals, const uint16_t size) override;
		virtual void set(const float * vals, const uint16_t size) override;
		virtual void set(const double * vals, const uint16_t size) override;

		virtual void set(const std::vector<uint8_t>& vals) override;
		virtual void set(const std::vector<int8_t>& vals) override;
		virtual void set(const std::vector<uint16_t>& vals) override;
		virtual void set(const std::vector<int16_t>& vals) override;
		virtual void set(const std::vector<uint32_t>& vals) override;
		virtual void set(const std::vector<int32_t>& vals) override;
		virtual void set(const std::vector<uint64_t>& vals) override;
		virtual void set(const std::vector<int64_t>& vals) override;
		virtual void set(const std::vector<float>& vals) override;
		virtual void set(const std::vector<double>& vals) override;

		virtual bool isValid() const override;
		virtual bool isArray() const override;

		virtual uint16_t getSize() const override;
	protected:
		T _val;
		virtual void copyTo(Parameter* parm) const override;
		virtual void addToMessage(std::vector<unsigned char> &data) const;
		virtual void UpdateFromSerial();
	};

	template<typename T>
	class ArrayValue: public Parameter
	{
	public:
		ArrayValue(const uint8_t ID, const std::string name, SerialRW * serialRW);
		virtual ~ArrayValue() noexcept;

		/* copy, move constructors and operators */
		ArrayValue(const ArrayValue<T> & val);
		ArrayValue(ArrayValue<T> && val) noexcept;
		ArrayValue<T> & operator = (const ArrayValue<T> & other);
		ArrayValue<T> & operator = (ArrayValue<T> && other) noexcept;

		virtual uint8_t as_uint8_t() override;
		virtual int8_t as_int8_t() override;
		virtual uint16_t as_uint16_t() override;
		virtual int16_t as_int16_t() override;
		virtual uint32_t as_uint32_t() override;
		virtual int32_t as_int32_t() override;
		virtual uint64_t as_uint64_t() override;
		virtual int64_t as_int64_t() override;
		virtual float as_float() override;
		virtual double as_double() override;

		virtual uint8_t as_uint8_t(uint16_t index) override;
		virtual int8_t as_int8_t(uint16_t index) override;
		virtual uint16_t as_uint16_t(uint16_t index) override;
		virtual int16_t as_int16_t(uint16_t index) override;
		virtual uint32_t as_uint32_t(uint16_t index) override;
		virtual int32_t as_int32_t(uint16_t index) override;
		virtual uint64_t as_uint64_t(uint16_t index) override;
		virtual int64_t as_int64_t(uint16_t index) override;
		virtual float as_float(uint16_t index) override;
		virtual double as_double(uint16_t index) override;

		virtual void set(const uint8_t val) override;
		virtual void set(const int8_t val) override;
		virtual void set(const uint16_t val) override;
		virtual void set(const int16_t val) override;
		virtual void set(const uint32_t val) override;
		virtual void set(const int32_t val) override;
		virtual void set(const uint64_t val) override;
		virtual void set(const int64_t val) override;
		virtual void set(const float val) override;
		virtual void set(const double val) override;
		virtual void set(const Value& val) override;

		virtual void set(const uint8_t * vals, const uint16_t size) override;
		virtual void set(const int8_t * vals, const uint16_t size) override;
		virtual void set(const uint16_t * vals, const uint16_t size) override;
		virtual void set(const int16_t * vals, const uint16_t size) override;
		virtual void set(const uint32_t * vals, const uint16_t size) override;
		virtual void set(const int32_t * vals, const uint16_t size) override;
		virtual void set(const uint64_t * vals, const uint16_t size) override;
		virtual void set(const int64_t * vals, const uint16_t size) override;
		virtual void set(const float * vals, const uint16_t size) override;
		virtual void set(const double * vals, const uint16_t size) override;

		virtual void set(const std::vector<uint8_t>& vals) override;
		virtual void set(const std::vector<int8_t>& vals) override;
		virtual void set(const std::vector<uint16_t>& vals) override;
		virtual void set(const std::vector<int16_t>& vals) override;
		virtual void set(const std::vector<uint32_t>& vals) override;
		virtual void set(const std::vector<int32_t>& vals) override;
		virtual void set(const std::vector<uint64_t>& vals) override;
		virtual void set(const std::vector<int64_t>& vals) override;
		virtual void set(const std::vector<float>& vals) override;
		virtual void set(const std::vector<double>& vals) override;

		virtual bool isValid() const override;
		virtual bool isArray() const override;

		virtual uint16_t getSize() const override;
	protected:
		T* _val;
		uint16_t _size;
		virtual void copyTo(Parameter* parm) const override;
		virtual void addToMessage(std::vector<unsigned char> &data) const;
		virtual void UpdateFromSerial();
	};

	class ErrorValue: public Parameter
	{
		/* singleton */
	public:
		static ErrorValue * getInstance();
	private:
		static ErrorValue *_instance;
		void operator delete(void *)
		{
		} //prevent deletion

		ErrorValue(const uint8_t ID, const std::string name, SerialRW * serialRW);
		virtual ~ErrorValue()
		{
		}
	public:
		virtual uint8_t as_uint8_t() override;
		virtual int8_t as_int8_t() override;
		virtual uint16_t as_uint16_t() override;
		virtual int16_t as_int16_t() override;
		virtual uint32_t as_uint32_t() override;
		virtual int32_t as_int32_t() override;
		virtual uint64_t as_uint64_t() override;
		virtual int64_t as_int64_t() override;
		virtual float as_float() override;
		virtual double as_double() override;

		virtual uint8_t as_uint8_t(uint16_t index) override;
		virtual int8_t as_int8_t(uint16_t index) override;
		virtual uint16_t as_uint16_t(uint16_t index) override;
		virtual int16_t as_int16_t(uint16_t index) override;
		virtual uint32_t as_uint32_t(uint16_t index) override;
		virtual int32_t as_int32_t(uint16_t index) override;
		virtual uint64_t as_uint64_t(uint16_t index) override;
		virtual int64_t as_int64_t(uint16_t index) override;
		virtual float as_float(uint16_t index) override;
		virtual double as_double(uint16_t index) override;

		virtual void set(const uint8_t val) override;
		virtual void set(const int8_t val) override;
		virtual void set(const uint16_t val) override;
		virtual void set(const int16_t val) override;
		virtual void set(const uint32_t val) override;
		virtual void set(const int32_t val) override;
		virtual void set(const uint64_t val) override;
		virtual void set(const int64_t val) override;
		virtual void set(const float val) override;
		virtual void set(const double val) override;
		virtual void set(const Value&) override
		{
		}

		virtual void set(const uint8_t * vals, const uint16_t size) override;
		virtual void set(const int8_t * vals, const uint16_t size) override;
		virtual void set(const uint16_t * vals, const uint16_t size) override;
		virtual void set(const int16_t * vals, const uint16_t size) override;
		virtual void set(const uint32_t * vals, const uint16_t size) override;
		virtual void set(const int32_t * vals, const uint16_t size) override;
		virtual void set(const uint64_t * vals, const uint16_t size) override;
		virtual void set(const int64_t * vals, const uint16_t size) override;
		virtual void set(const float * vals, const uint16_t size) override;
		virtual void set(const double * vals, const uint16_t size) override;

		virtual void set(const std::vector<uint8_t>& vals) override;
		virtual void set(const std::vector<int8_t>& vals) override;
		virtual void set(const std::vector<uint16_t>& vals) override;
		virtual void set(const std::vector<int16_t>& vals) override;
		virtual void set(const std::vector<uint32_t>& vals) override;
		virtual void set(const std::vector<int32_t>& vals) override;
		virtual void set(const std::vector<uint64_t>& vals) override;
		virtual void set(const std::vector<int64_t>& vals) override;
		virtual void set(const std::vector<float>& vals) override;
		virtual void set(const std::vector<double>& vals) override;

		virtual bool isValid() const override;
		virtual bool isArray() const override;

		virtual uint16_t getSize() const override;
	protected:
		virtual void copyTo(Parameter*) const override
		{
		}
		virtual void addToMessage(std::vector<unsigned char> &) const
		{
		}
		virtual void UpdateFromSerial()
		{
		}
	};

	/* explicit template instantiations of ValueImpl */
	extern template class ValueImpl<uint8_t>;
	extern template class ValueImpl<int8_t>;
	extern template class ValueImpl<uint16_t>;
	extern template class ValueImpl<int16_t>;
	extern template class ValueImpl<uint32_t>;
	extern template class ValueImpl<int32_t>;
	extern template class ValueImpl<uint64_t>;
	extern template class ValueImpl<int64_t>;
	extern template class ValueImpl<float>;
	extern template class ValueImpl<double>;

	/* explicit template instantiations of ArrayValue */
	extern template class ArrayValue<uint8_t>;
	extern template class ArrayValue<int8_t>;
	extern template class ArrayValue<uint16_t>;
	extern template class ArrayValue<int16_t>;
	extern template class ArrayValue<uint32_t>;
	extern template class ArrayValue<int32_t>;
	extern template class ArrayValue<uint64_t>;
	extern template class ArrayValue<int64_t>;
	extern template class ArrayValue<float>;
	extern template class ArrayValue<double>;

} /* namespace twirre */

#endif /* VALUE_H_ */
