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

#ifndef CORE_VALUEIMPL_H_
#define CORE_VALUEIMPL_H_

#include "../Core/Value.h"

namespace twirre
{
template<typename T>
	class ValueImpl: public Parameter
	{
	public:
		ValueImpl(const std::string name, const T val);
		ValueImpl(const std::string name, const T val, const T min, const T max);
		ValueImpl(const std::string name, const T val, owned_mutex * actuatorMutex);
		ValueImpl(const std::string name, const T val, const T min, const T max, owned_mutex * actuatorMutex);

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

		virtual Scalar& getMin() override;
		virtual Scalar& getMax() override;

		virtual bool isValid() const override;
		virtual bool isArray() const override;

		virtual uint32_t getSize() const override;
		virtual size_t getElementSize() const override;
		virtual void* getBuffer() override;
	protected:
		T _val;
		ScalarImpl<T> _min;
		ScalarImpl<T> _max;
		virtual void copyTo(Parameter* parm) const override;
	};

	/* explicit template instantiations of ValueImpl */
	extern template class ValueImpl<uint8_t> ;
	extern template class ValueImpl<int8_t> ;
	extern template class ValueImpl<uint16_t> ;
	extern template class ValueImpl<int16_t> ;
	extern template class ValueImpl<uint32_t> ;
	extern template class ValueImpl<int32_t> ;
	extern template class ValueImpl<uint64_t> ;
	extern template class ValueImpl<int64_t> ;
	extern template class ValueImpl<float> ;
	extern template class ValueImpl<double> ;
}
#endif /* CORE_VALUEIMPL_H_ */
