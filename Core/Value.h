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

#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include <cstdint>
#include <vector>
#include <shared_mutex>
#include <type_traits>
#include "../Serial/SerialRW.h"

#include "../Core/owned_mutex.h"

namespace twirre
{
	class Parameter;

	enum class NativeType
	{
		type_void, type_int8, type_uint8, type_int16, type_uint16, type_int32, type_uint32, type_int64, type_uint64, type_float, type_double
	};

	static constexpr inline const char * enumtostr(const NativeType t)
	{
		switch(t)
		{
		case NativeType::type_void:
			return "type_void";
		case NativeType::type_int8:
			return "type_int8";
		case NativeType::type_uint8:
			return "type_uint8";
		case NativeType::type_int16:
			return "type_int16";
		case NativeType::type_uint16:
			return "type_uint16";
		case NativeType::type_int32:
			return "type_int32";
		case NativeType::type_uint32:
			return "type_uint32";
		case NativeType::type_int64:
			return "type_int64";
		case NativeType::type_uint64:
			return "type_uint64";
		case NativeType::type_float:
			return "type_float";
		case NativeType::type_double:
			return "type_double";
		default:
			return nullptr;
		}
	}

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

	class Scalar
	{
	public:
		virtual ~Scalar()
		{
		}

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
		virtual std::string as_string() = 0;
	};

	class Value: public Scalar
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
		virtual std::string as_string() = 0;

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
		//virtual std::string as_string(uint32_t index) = 0;

		const std::string& getName();

		virtual uint32_t getSize() const = 0;
		virtual size_t getElementSize() const = 0;
		virtual void* getBuffer() = 0;

		virtual bool isValid() const = 0;
		virtual bool isArray() const = 0;
	protected:
		virtual void copyTo(Parameter* parm) const = 0;
		std::string _name;
		mutable std::shared_timed_mutex _rwMutex;
	};

	class Parameter: public Value
	{
		template<typename T> friend class ArrayValue;
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

		virtual void setActuatorMutex(owned_mutex * actuatorMutex);

		virtual Scalar& getMin() = 0;
		virtual Scalar& getMax() = 0;

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
	class ScalarImpl: public Scalar
	{
	public:
		ScalarImpl()
		{
		}
		explicit ScalarImpl(const T val);

		virtual ~ScalarImpl()
		{
		}

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
		virtual std::string as_string() override;

		virtual void set(const T val);

	protected:
		mutable std::shared_timed_mutex _rwMutex;
		T _val;
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
		virtual std::string as_string() override;

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

		virtual Scalar& getMin() override;
		virtual Scalar& getMax() override;

		virtual bool isValid() const override;
		virtual bool isArray() const override;

		virtual uint32_t getSize() const override;
		virtual size_t getElementSize() const override;
		virtual void* getBuffer() override;
	protected:
		ScalarImpl<uint8_t> _min;
		ScalarImpl<uint8_t> _max;

		virtual void copyTo(Parameter*) const override
		{
		}
	};

	/* explicit template instantiations of ScalarImpl */
	extern template class ScalarImpl<uint8_t> ;
	extern template class ScalarImpl<int8_t> ;
	extern template class ScalarImpl<uint16_t> ;
	extern template class ScalarImpl<int16_t> ;
	extern template class ScalarImpl<uint32_t> ;
	extern template class ScalarImpl<int32_t> ;
	extern template class ScalarImpl<uint64_t> ;
	extern template class ScalarImpl<int64_t> ;
	extern template class ScalarImpl<float> ;
	extern template class ScalarImpl<double> ;
} /* namespace twirre */




#include "../Core/ValueImpl.h"
#include "../Core/ArrayValue.h"




#endif /* VALUE_H_ */
