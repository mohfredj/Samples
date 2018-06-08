#ifndef MFG_FOUNDATION_DATASTREAM_H
#define MFG_FOUNDATION_DATASTREAM_H

#include <sys/types.h>
#include <string>

#include "axfoundation/axfoundation_global.h"

namespace MFG {
namespace foundation {

class DataStreamPrivate;

class FOUNDATION_SHARED_EXPORT DataStream
{
	public:
		DataStream(char * data, size_t size, size_t capacity);
		virtual ~DataStream();

		/** DataStream copy constructor */
		DataStream(const DataStream & other) noexcept;

		/** DataStream move constructor */
		DataStream(DataStream && other) noexcept;

		/** DataStream move assignment operator */
		DataStream & operator=(DataStream && other) ;

		/** DataStream copy assignment operator */
		DataStream & operator=(const DataStream & other) noexcept;

		/** Message swap method */
		void swap(DataStream & other) noexcept;

		bool atEnd() const;
		size_t size();

		DataStream & operator<<(bool);
		DataStream & operator<<(u_int8_t);
		DataStream & operator<<(u_int16_t);
		DataStream & operator<<(u_int32_t);
		DataStream & operator<<(u_int64_t);
		DataStream & operator<<(int8_t);
		DataStream & operator<<(int16_t);
		DataStream & operator<<(int32_t);
		DataStream & operator<<(int64_t);
		DataStream & operator<<(float);
		DataStream & operator<<(double);
		DataStream & operator<<(const char *);
		DataStream & operator<<(const std::string & value);

		DataStream & operator>>(bool & value);
		DataStream & operator>>(u_int8_t & value);
		DataStream & operator>>(u_int16_t & value);
		DataStream & operator>>(u_int32_t & value);
		DataStream & operator>>(u_int64_t & value);
		DataStream & operator>>(int8_t & value);
		DataStream & operator>>(int16_t & value);
		DataStream & operator>>(int32_t & value);
		DataStream & operator>>(int64_t & value);
		DataStream & operator>>(float & value);
		DataStream & operator>>(double & value);
		DataStream & operator>>(const char *& value);
		DataStream & operator>>(std::string & value);

	protected:
		DataStreamPrivate * m_priv;
};

} // namespace foundation
} // namespace MFG

#endif // MFG_FOUNDATION_DATASTREAM_H

