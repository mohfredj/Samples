#ifndef AX_FOUNDATION_BYTEARRAY
#define AX_FOUNDATION_BYTEARRAY

#include <stdint.h>
#include <sys/types.h>

#include "mfgfoundation_global.h"

typedef unsigned char uchar;

namespace AX {

namespace foundation {

class ByteArrayPriv;

/**
 *  * @brief The ByteArray class
 *   */
class FOUNDATION_SHARED_EXPORT ByteArray
{
	ByteArrayPriv * m_priv;

	public:
	/** ByteArray constructor */
	ByteArray();
	/** Constructs a new ByteArray with a string value. */
	ByteArray(const char * str);
	/** Constructs a new ByteArray with a data value and size. */
	ByteArray(const char * data, size_t size);
	/** Constructs a new ByteArray with a data value and size. */
	ByteArray(const unsigned char * data, size_t size);
	/** ByteArray copy constructor */
	ByteArray(const ByteArray &);
	/** Destroys the ByteArray and the contained object. */
	~ByteArray();

#if __cplusplus >= 201103L
	/** ByteArray move constructor */
	ByteArray(ByteArray && other) noexcept;

	/** ByteArray move assignment operator */
	ByteArray & operator=(ByteArray && other);
#endif

	/** ByteArray copy assignment operator */
	ByteArray &operator=(const ByteArray &);
	/** ByteArray assignment operator with a string value */
	ByteArray &operator=(const char *str);
	/** Returns true if this byte array is equal to other byte array; otherwise returns false. */
	bool operator ==(const ByteArray & b) const;
	
	void swap(ByteArray & other) noexcept;

	/** Sets the size of the byte array to size bytes. */
	void resize(size_t size);

	/** Returns a pointer to the data stored in the byte array. The pointer can be used to access and modify the bytes that compose the array. */
	char * data();

	/** Returns a pointer to the data stored in the byte array. */
	const char * data() const;

	/** Returns the number of bytes in this byte array. */
	size_t length() const;

	/** Returns the number of bytes in this byte array. */
	size_t size() const;

	/** Returns true if the byte array has size 0; otherwise returns false. */
	bool isEmpty() const;

	/** eturns true if this byte array is null; otherwise returns false. */
	bool isNull() const;

	/** Returns a pointer to the data stored in the byte array. */
	operator const char * () const;

	/** Returns the character at index position i in the byte array. */
	char at(size_t i) const;

	/** Returns the character at index position i in the byte array. */
	char operator[](size_t i) const;

	/** Returns the byte at index position i as a modifiable reference. 
 * 	  * If an assignment is made beyond the end of the byte array, the array is extended with resize() before the assignment takes place.
 * 	  	  */
	char & operator[](size_t i);

	/** Returns a copy of the byte array, encoded as Base64. */
	ByteArray toBase64() const;

	/** Returns a hex encoded copy of the byte array. */
	ByteArray toHex() const;

	/** Truncates the byte array at index position pos. */
	void truncate(size_t pos);

	/** Returns a decoded copy of the hex encoded array hexEncoded. */
	static ByteArray fromHex(const ByteArray & hexEncoded);
	
	/// for testu
	//	int refCount() const;
	//	};
	//
	//	} // namespace foundation
	//	} // namespace AX
	//
	//	#endif // !BYTEARRAY 上
