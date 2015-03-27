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

using namespace std;

namespace twirre
{
	class Value
	{
		friend class Sensor;
		friend class Actuator;
	public:

		Value(const uint8_t ID, const string name, SerialRW * serialRW);
		virtual ~Value()
		{
		}
		;

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

		const uint8_t getId();
		const string& getName();

		virtual bool isValid() = 0;
	protected:
		SerialRW * _serialRW;
		virtual void UpdateFromSerial() = 0;
		const uint8_t _id;
		const string _name;

	};

	class Parameter: public Value
	{
		friend class Actuator;
	public:
		Parameter(const uint8_t ID, const string name, SerialRW * serialRW);
		virtual ~Parameter()
		{
		}
		;

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

	protected:
		virtual void addToMessage(vector<unsigned char> &data, uint16_t &payloadSize) const = 0;
		void resetModified();
		bool _modified;
	};

	template<typename T>
	class ValueImpl: public Parameter
	{
	public:
		ValueImpl(const uint8_t ID, const string name, T val, SerialRW * serialRW);
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

		virtual bool isValid() override;
	protected:
		T _val;
		virtual void addToMessage(vector<unsigned char> &data, uint16_t &payloadSize) const;
		virtual void UpdateFromSerial();
	};

	class ErrorValue: public Parameter
	{
	/* singleton */
	public:
		static ErrorValue * const getInstance();
	private:
		static ErrorValue *_instance;
		void operator delete( void * ) {} //prevent deletion

		ErrorValue(const uint8_t ID, const string name, SerialRW * serialRW);
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

		virtual bool isValid() override;
	protected:
		const string _errorMsg;
		virtual void addToMessage(vector<unsigned char> &data, uint16_t &payloadSize) const
		{
		}
		virtual void UpdateFromSerial()
		{
		}
	};

} /* namespace twirre */

#endif /* VALUE_H_ */
