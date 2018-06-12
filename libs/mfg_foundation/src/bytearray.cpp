#include "axfoundation/bytearray.h"

#include <string.h>
#include <utility>
#include <memory>

namespace AX {

namespace foundation {


class ByteArrayPriv
{
	public:
		ByteArrayPriv() : m_data(0), m_size(0), m_ref_count(new int(1)) {}
		ByteArrayPriv(const ByteArrayPriv & other);
		~ByteArrayPriv();
		
		ByteArrayPriv(const char * str);
		ByteArrayPriv(const char * data, size_t size);
		ByteArrayPriv(const unsigned char * data, size_t size);
		
		void detach();

		char * m_data;
		size_t m_size;
		int * m_ref_count;


ByteArrayPriv::ByteArrayPriv(const ByteArrayPriv & other) : 
	m_data(other.m_data), 
	m_size(other.m_size), 
	m_ref_count(other.m_ref_count)
{
	++*m_ref_count;
}

ByteArrayPriv::ByteArrayPriv(const char * str) : 
	m_data(0),
	m_size(0),
	m_ref_count(new int(1))
{
	m_size = strlen(str);
	m_data = new char[m_size + 1];
	m_data[m_size] = 0;
	memcpy(m_data, str, m_size);
}

ByteArrayPriv::ByteArrayPriv(const char * data, size_t size) :
	m_data(new char[size + 1]),
	m_size(size),
	m_ref_count(new int(1))
{
	m_data[m_size] = 0;
	memcpy(m_data, data, size);
}

ByteArrayPriv::ByteArrayPriv(const unsigned char * data, size_t size) :
	m_data(new char[size + 1]),
	m_size(size),
	m_ref_count(new int(1))
{
	m_data[m_size] = 0;
	memcpy(m_data, data, size);
}

ByteArrayPriv::~ByteArrayPriv()
{
	if (*m_ref_count > 1)
	{
		--*m_ref_count;
	}
	else
	{
		delete [] m_data;
		m_data = 0;
		m_size = 0;
		delete m_ref_count;
		m_ref_count = 0;
	}
}

void ByteArrayPriv::detach()
{
	if (*m_ref_count > 1)
	{
		--*m_ref_count;
		m_ref_count = new int(1);
		if (m_data)
		{
			char * data = new char[(size_t)m_size + 1];
			memcpy(data, m_data, (size_t)m_size + 1);
			m_data = data;
		}
	}

}

ByteArray::ByteArray() : 
	m_priv(new ByteArrayPriv())
{
}

ByteArray::~ByteArray()
{
	delete m_priv;
}

ByteArray::ByteArray(const char * str) :
	m_priv(new ByteArrayPriv(str))
{
}

ByteArray::ByteArray(const char * data, size_t size) :
	m_priv(new ByteArrayPriv(data, size))
{
}

ByteArray::ByteArray(const unsigned char * data, size_t size) :
	m_priv(new ByteArrayPriv(data, size))
{
}

ByteArray::ByteArray(const ByteArray & other) : 
	m_priv(new ByteArrayPriv(*other.m_priv))
{
}

ByteArray & ByteArray::operator=(const ByteArray & other)
{
	if (this != &other)
	{
		ByteArray(other).swap(*this);
	}

	return *this;
}

ByteArray & ByteArray::operator=(const char *str)
{
	ByteArray(str).swap(*this);

	return *this;
}

#if __cplusplus >= 201103L
ByteArray::ByteArray(ByteArray && other) noexcept : 
	m_priv(std::move(other.m_priv))
{
	other.m_priv = nullptr;
}

ByteArray & ByteArray::operator=(ByteArray && other)
{
	if (this != &other)
	{
		delete m_priv;
		m_priv = nullptr;
		#if __cplusplus >= 201402L
		m_priv = std::exchange(other.m_priv, nullptr);
		#else
		other.swap(*this);
		#endif
	}

	return *this;
}
#endif

bool ByteArray::operator ==(const ByteArray & b) const
{
	if (m_priv->m_data == b.m_priv->m_data)
		return true;
	if (m_priv->m_data == 0 || b.m_priv->m_data == 0)
		return false;
	if (m_priv->m_size != b.m_priv->m_size)
		return false;
	return (memcmp(m_priv->m_data, b.m_priv->m_data, m_priv->m_size) == 0);
}

void ByteArray::swap(ByteArray & other) noexcept
{
	std::swap(this->m_priv, other.m_priv);
}

void ByteArray::resize(size_t size)
{
	m_priv->detach();
	char * data = m_priv->m_data;
	if (size)
	{
		m_priv->m_data = new char[size + 1];
		m_priv->m_data[size] = 0;
		if (data)
		{
			memcpy(m_priv->m_data, data, std::min(m_priv->m_size, size));
		}
	}
	m_priv->m_size = size;

	delete [] data;
}

char * ByteArray::data() 
{
	m_priv->detach();
	return m_priv->m_data;
}

const char * ByteArray::data() const 
{
	return m_priv->m_data;
}

size_t ByteArray::length() const
{
	return m_priv->m_size;
}

size_t ByteArray::size() const
{
	return m_priv->m_size;
}

bool ByteArray::isEmpty() const 
{
	return m_priv->m_size == 0;
}

bool ByteArray::isNull() const 
{
	return m_priv->m_data == 0;
}

ByteArray::operator const char * () const 
{
	return m_priv->m_data;
}

char ByteArray::at(size_t i) const
{
	if (m_priv->m_data == 0)
		return 0;
	return m_priv->m_data[i];
}

char ByteArray::operator[](size_t i) const
{
	return at(i);
}

char & ByteArray::operator[](size_t i)
{
	m_priv->detach();
	if (i > m_priv->m_size || m_priv->m_data == 0)
	{
		resize(i);
	}
	return m_priv->m_data[i];
}

ByteArray ByteArray::toBase64() const
{
	const char alphabet[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
					"ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
	const char padchar = '=';
	int padlen = 0;

	ByteArray tmp;
	tmp.resize(((m_priv->m_size * 4) / 3) + 3);

	size_t i = 0;
	char *out = tmp.data();
	while (i < m_priv->m_size) {
		int chunk = 0;
		chunk |= int(uchar(m_priv->m_data[i++])) << 16;
		if (i == m_priv->m_size) {
			padlen = 2;
		} else {
			chunk |= int(uchar(m_priv->m_data[i++])) << 8;
			if (i == m_priv->m_size) padlen = 1;
			else chunk |= int(uchar(m_priv->m_data[i++]));
		}

		int j = (chunk & 0x00fc0000) >> 18;
		int k = (chunk & 0x0003f000) >> 12;
		int l = (chunk & 0x00000fc0) >> 6;
		int m = (chunk & 0x0000003f);
		*out++ = alphabet[j];
		*out++ = alphabet[k];
		if (padlen > 1) *out++ = padchar;
		else *out++ = alphabet[l];
		if (padlen > 0) *out++ = padchar;
		else *out++ = alphabet[m];
	}

	tmp.m_priv->m_size = size_t(out - tmp.data());
	return tmp;
}


ByteArray ByteArray::toHex() const
{
	ByteArray hex;
	hex.resize(m_priv->m_size * 2);
	char *hexData = hex.data();
	const uchar *data = (const uchar *)m_priv->m_data;
	for (size_t i = 0; i < m_priv->m_size; ++i) {
		int j = (data[i] >> 4) & 0xf;
		if (j <= 9)
			hexData[i*2] = char(j + '0');
		 else
			hexData[i*2] = char(j + 'a' - 10);
		j = data[i] & 0xf;
		if (j <= 9)
			hexData[i*2+1] = char(j + '0');
		 else
			hexData[i*2+1] = char(j + 'a' - 10);
	}
	return hex;
}

void ByteArray::truncate(size_t pos)
{
	if (pos < m_priv->m_size)
		resize(pos);
}

ByteArray ByteArray::fromHex(const ByteArray & hexEncoded)
{
    ByteArray res;
    res.resize(hexEncoded.size() / 2);
    uchar *result = (uchar *)res.data();

    bool first = true;
    for (size_t i = 0; i < hexEncoded.size(); ++i) {
        int ch = hexEncoded.at(i);
        int tmp;
        if (ch >= '0' && ch <= '9')
            tmp = ch - '0';
        else if (ch >= 'a' && ch <= 'f')
            tmp = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'F')
            tmp = ch - 'A' + 10;
        else
            continue;
        if (first) {
            *result = 0xFF & tmp << 4;
            first = false;
        } else {
            *result |= tmp;
            ++result;
            first = true;
        }
    }

    res.truncate(result - (const uchar *)res.data());
	return res;
}

int ByteArray::refCount() const
{
	return *m_priv->m_ref_count;
}

} // namespace foundation
} // namespace AX

