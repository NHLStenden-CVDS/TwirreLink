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
#include "SerialRW.h"


using namespace std;

namespace twirre {
class Value {
public:
	int id;
	string name;

	Value(int ID, string name, SerialRW & serialRW);
	virtual ~Value(){};

	int GetSize();
	void SetBuffer(unsigned char *buffer);

	virtual uint8_t getAs_uint8_t() = 0;
	virtual int8_t getAs_int8_t() = 0;
	virtual uint16_t getAs_uint16_t() = 0;
	virtual int16_t getAs_int16_t() = 0;
	virtual uint32_t getAs_uint32_t() = 0;
	virtual int32_t getAs_int32_t() = 0;
	virtual uint64_t getAs_uint64_t() = 0;
	virtual int64_t getAs_int64_t() = 0;
	virtual float getAs_float() = 0;
	virtual double getAs_double() = 0;
protected:
	SerialRW & _serialRW;

};

class Parameter: public Value {
public:
	Parameter(int ID, string name, SerialRW & serialRW);
	virtual ~Parameter(){};

	virtual void set(uint8_t val) = 0;
	virtual void set(int8_t val) = 0;
	virtual void set(uint16_t val) = 0;
	virtual void set(int16_t val) = 0;
	virtual void set(uint32_t val) = 0;
	virtual void set(int32_t val) = 0;
	virtual void set(uint64_t val) = 0;
	virtual void set(int64_t val) = 0;
	virtual void set(float val) = 0;
	virtual void set(double val) = 0;

protected:
	bool _modified;
};

template<typename T>
class ValueImpl: public Parameter {
public:
	ValueImpl(int ID, string name, T val, SerialRW & serialRW);
	virtual ~ValueImpl(){};

	virtual uint8_t getAs_uint8_t() override;
	virtual int8_t getAs_int8_t() override;
	virtual uint16_t getAs_uint16_t() override;
	virtual int16_t getAs_int16_t() override;
	virtual uint32_t getAs_uint32_t() override;
	virtual int32_t getAs_int32_t() override;
	virtual uint64_t getAs_uint64_t() override;
	virtual int64_t getAs_int64_t() override;
	virtual float getAs_float() override;
	virtual double getAs_double() override;

	virtual void set(uint8_t val) override;
	virtual void set(int8_t val) override;
	virtual void set(uint16_t val) override;
	virtual void set(int16_t val) override;
	virtual void set(uint32_t val) override;
	virtual void set(int32_t val) override;
	virtual void set(uint64_t val) override;
	virtual void set(int64_t val) override;
	virtual void set(float val) override;
	virtual void set(double val) override;

protected:
	T _val;
};

} /* namespace twirre */

#endif /* VALUE_H_ */
