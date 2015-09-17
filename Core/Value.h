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
#include <shared_mutex>
#include "../Serial/SerialRW.h"

#include "../Core/owned_mutex.h"

namespace twirre
{
	class Parameter;

	enum class NativeType
	{
		type_void,
		type_int8,
		type_uint8,
		type_int16,
		type_uint16,
		type_int32,
		type_uint32,
		type_int64,
		type_uint64,
		type_float,
		type_double
	};

	class Value
	{
		template<typename T> friend class ValueImpl;
		template<typename T> friend class ArrayValue;
	public:

		Value(const std::string name);
		virtual ~Value()
		{
		}

		int GetSize();
		void SetBuffer(unsigned char *buffer);

		virtual NativeType getNativeType() = 0;

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

		virtual uint8_t as_uint8_t(uint32_t index) = 0;
		virtual int8_t as_int8_t(uint32_t index) = 0;
		virtual uint16_t as_uint16_t(uint32_t index) = 0;
		virtual int16_t as_int16_t(uint32_t index) = 0;
		virtual uint32_t as_uint32_t(uint32_t index) = 0;
		virtual int32_t as_int32_t(uint32_t index) = 0;
		virtual uint64_t as_uint64_t(uint32_t index) = 0;
		virtual int64_t as_int64_t(uint32_t index) = 0;
		virtual float as_float(uint32_t index) = 0;
		virtual double as_double(uint32_t index) = 0;

		const std::string& getName();

		virtual uint32_t getSize() const = 0;
		virtual size_t getElementSize() const = 0;
		virtual void* getBuffer() = 0;

		virtual bool isValid() const = 0;
		virtual bool isArray() const = 0;
	protected:
		virtual void copyTo(Parameter* parm) const = 0;
		std::string _name;
		std::shared_timed_mutex _rwMutex;
	};

	class Parameter: public Value
	{
		template <typename T> friend class ArrayValue;
	public:
		Parameter(const std::string name);
		Parameter(const std::string name, owned_mutex * actuatorMutex);
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

		virtual void set(const uint8_t * vals, const uint32_t size) = 0;
		virtual void set(const int8_t * vals, const uint32_t size) = 0;
		virtual void set(const uint16_t * vals, const uint32_t size) = 0;
		virtual void set(const int16_t * vals, const uint32_t size) = 0;
		virtual void set(const uint32_t * vals, const uint32_t size) = 0;
		virtual void set(const int32_t * vals, const uint32_t size) = 0;
		virtual void set(const uint64_t * vals, const uint32_t size) = 0;
		virtual void set(const int64_t * vals, const uint32_t size) = 0;
		virtual void set(const float * vals, const uint32_t size) = 0;
		virtual void set(const double * vals, const uint32_t size) = 0;

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

		bool isModified() const;
		void resetModified();

	protected:
		bool _modified;
		owned_mutex * _actuatorMutex;
	};

	template<typename T>
	class ValueImpl: public Parameter
	{
	public:
		ValueImpl(const std::string name, T val);
		ValueImpl(const std::string name, T val, owned_mutex * actuatorMutex);
		virtual ~ValueImpl()
		{
		}

		virtual NativeType getNativeType() override;
		T getNativeValue();
		void setNativeValue(T val);

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

		virtual uint8_t as_uint8_t(uint32_t index) override;
		virtual int8_t as_int8_t(uint32_t index) override;
		virtual uint16_t as_uint16_t(uint32_t index) override;
		virtual int16_t as_int16_t(uint32_t index) override;
		virtual uint32_t as_uint32_t(uint32_t index) override;
		virtual int32_t as_int32_t(uint32_t index) override;
		virtual uint64_t as_uint64_t(uint32_t index) override;
		virtual int64_t as_int64_t(uint32_t index) override;
		virtual float as_float(uint32_t index) override;
		virtual double as_double(uint32_t index) override;

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

		virtual void set(const uint8_t * vals, const uint32_t size) override;
		virtual void set(const int8_t * vals, const uint32_t size) override;
		virtual void set(const uint16_t * vals, const uint32_t size) override;
		virtual void set(const int16_t * vals, const uint32_t size) override;
		virtual void set(const uint32_t * vals, const uint32_t size) override;
		virtual void set(const int32_t * vals, const uint32_t size) override;
		virtual void set(const uint64_t * vals, const uint32_t size) override;
		virtual void set(const int64_t * vals, const uint32_t size) override;
		virtual void set(const float * vals, const uint32_t size) override;
		virtual void set(const double * vals, const uint32_t size) override;

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

		virtual uint32_t getSize() const override;
		virtual size_t getElementSize() const override;
		virtual void* getBuffer() override;
	protected:
		T _val;
		virtual void copyTo(Parameter* parm) const override;
	};

	template<typename T>
	class ArrayValue: public Parameter
	{
	public:
		ArrayValue(const std::string name);
		ArrayValue(const std::string name, owned_mutex * actuatorMutex);
		virtual ~ArrayValue() noexcept;

		virtual NativeType getNativeType() override;
		T* nativeValue();

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

		virtual uint8_t as_uint8_t(uint32_t index) override;
		virtual int8_t as_int8_t(uint32_t index) override;
		virtual uint16_t as_uint16_t(uint32_t index) override;
		virtual int16_t as_int16_t(uint32_t index) override;
		virtual uint32_t as_uint32_t(uint32_t index) override;
		virtual int32_t as_int32_t(uint32_t index) override;
		virtual uint64_t as_uint64_t(uint32_t index) override;
		virtual int64_t as_int64_t(uint32_t index) override;
		virtual float as_float(uint32_t index) override;
		virtual double as_double(uint32_t index) override;

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

		virtual void set(const uint8_t * vals, const uint32_t size) override;
		virtual void set(const int8_t * vals, const uint32_t size) override;
		virtual void set(const uint16_t * vals, const uint32_t size) override;
		virtual void set(const int16_t * vals, const uint32_t size) override;
		virtual void set(const uint32_t * vals, const uint32_t size) override;
		virtual void set(const int32_t * vals, const uint32_t size) override;
		virtual void set(const uint64_t * vals, const uint32_t size) override;
		virtual void set(const int64_t * vals, const uint32_t size) override;
		virtual void set(const float * vals, const uint32_t size) override;
		virtual void set(const double * vals, const uint32_t size) override;

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

		virtual uint32_t getSize() const override;
		virtual size_t getElementSize() const override;
		virtual void* getBuffer() override;
		virtual T* getNativeBuffer();
		virtual void setSize(uint32_t size);
		virtual void setNative(T* data, uint32_t size);
	protected:
		T* _val;
		uint32_t _size;
		virtual void copyTo(Parameter* parm) const override;
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

		ErrorValue(const std::string name);
		virtual ~ErrorValue()
		{
		}
	public:
		virtual NativeType getNativeType() override;

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

		virtual uint8_t as_uint8_t(uint32_t index) override;
		virtual int8_t as_int8_t(uint32_t index) override;
		virtual uint16_t as_uint16_t(uint32_t index) override;
		virtual int16_t as_int16_t(uint32_t index) override;
		virtual uint32_t as_uint32_t(uint32_t index) override;
		virtual int32_t as_int32_t(uint32_t index) override;
		virtual uint64_t as_uint64_t(uint32_t index) override;
		virtual int64_t as_int64_t(uint32_t index) override;
		virtual float as_float(uint32_t index) override;
		virtual double as_double(uint32_t index) override;

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

		virtual void set(const uint8_t * vals, const uint32_t size) override;
		virtual void set(const int8_t * vals, const uint32_t size) override;
		virtual void set(const uint16_t * vals, const uint32_t size) override;
		virtual void set(const int16_t * vals, const uint32_t size) override;
		virtual void set(const uint32_t * vals, const uint32_t size) override;
		virtual void set(const int32_t * vals, const uint32_t size) override;
		virtual void set(const uint64_t * vals, const uint32_t size) override;
		virtual void set(const int64_t * vals, const uint32_t size) override;
		virtual void set(const float * vals, const uint32_t size) override;
		virtual void set(const double * vals, const uint32_t size) override;

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

		virtual uint32_t getSize() const override;
		virtual size_t getElementSize() const override;
		virtual void* getBuffer() override;
	protected:
		virtual void copyTo(Parameter*) const override
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
