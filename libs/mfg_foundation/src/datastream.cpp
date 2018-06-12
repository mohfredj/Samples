#include "mfgfoundation/datastream.h"
#include "mfgfoundation/exception.h"

#include <string.h>
#include <utility>
#include <algorithm>


namespace AX {
namespace foundation {

class FOUNDATION_SHARED_HIDDEN DataStreamPrivate
{
	public:
		DataStreamPrivate(char * data, size_t size, size_t capacity) :
			m_data(data),
			m_size(size),
			m_capacity(capacity),
			m_read_cursor(m_data),
			m_write_cursor(m_data + std::min(size, capacity))
		{
		}

		char * m_data;
		size_t m_size;
		size_t m_capacity;

		char * m_read_cursor;
		char * m_write_cursor;

		void write(const void * data, size_t size);
		void read(void * data, size_t size);
};

void DataStreamPrivate::write(const void * data, size_t size)
{
	if ((m_write_cursor + size) >= (m_data + m_capacity))
	{
		AX_THROW_EXCEPTION("DataStream buffer capacity reach");
	}

	memcpy(m_write_cursor, data, size);
	m_write_cursor += size;
}

void DataStreamPrivate::read(void * data, size_t size)
{
	if ((m_read_cursor + size) > m_write_cursor)
	{
		AX_THROW_EXCEPTION("DataStream not enought data");
	}

	memcpy(data, m_read_cursor, size);
	m_read_cursor += size;
}

DataStream::DataStream(char * data, size_t size, size_t capacity) :
	m_priv(new DataStreamPrivate(data, size, capacity))
{
}

DataStream::~DataStream()
{
	delete m_priv;
}


DataStream::DataStream(const DataStream & other) noexcept :
	m_priv(new DataStreamPrivate(*other.m_priv))
{

}

DataStream::DataStream(DataStream && other) noexcept :
	m_priv(other.m_priv)

{
	other.m_priv=nullptr;
}

DataStream & DataStream::operator=(DataStream && other)
{
	if (this != &other)
	{
		swap(other);
	}

	return *this;
}

DataStream & DataStream::operator=(const DataStream & other) noexcept
{
	if (this != &other)
	{
		DataStream(other).swap(*this);
	}

	return *this;
}

void DataStream::swap(DataStream & other) noexcept
{
	std::swap(this->m_priv, other.m_priv);
}

bool DataStream::atEnd() const
{
	return m_priv->m_write_cursor == m_priv->m_read_cursor;
}

size_t DataStream::size()
{
	return static_cast<size_t>(m_priv->m_write_cursor - m_priv->m_data);
}

DataStream & DataStream::operator<<(bool value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(u_int8_t value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(u_int16_t value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(u_int32_t value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(u_int64_t value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(int8_t value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(int16_t value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(int32_t value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(int64_t value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(float value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(double value)
{
	m_priv->write(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator<<(const char * value)
{
	m_priv->write(value, strlen(value) + 1);
	return *this;
}

DataStream & DataStream::operator<<(const std::string & value)
{
	m_priv->write(value.c_str(), value.length() + 1);
	return *this;
}

DataStream & DataStream::operator>>(bool & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(u_int8_t & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(u_int16_t & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(u_int32_t & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(u_int64_t & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(int8_t & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(int16_t & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(int32_t & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(int64_t & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(float & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(double & value)
{
	m_priv->read(&value, sizeof (value));
	return *this;
}

DataStream & DataStream::operator>>(const char *& value)
{
	size_t size = static_cast<size_t>(m_priv->m_write_cursor - m_priv->m_read_cursor);
	size_t str_size = strnlen(m_priv->m_read_cursor, size) + 1;
	if ((m_priv->m_read_cursor + str_size) > m_priv->m_write_cursor)
	{
		AX_THROW_EXCEPTION("Fail to read c-string");
	}
	value = m_priv->m_read_cursor;
	m_priv->m_read_cursor += str_size;

	return *this;
}

DataStream & DataStream::operator>>(std::string & value)
{
	const char * data = 0;
	this->operator>>(data);
	if (data)
	{
		value = data;
	}
	return *this;
}

} // namespace foundation
} // namespace AX

