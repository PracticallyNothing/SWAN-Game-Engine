#include "UTF-8.hpp"
#include <cstring>

template <typename T>
static inline auto GetBit(T b, size_t bit)
{
	return b & (1 << bit);
}

static inline bool
InRange(uint32_t min, uint32_t max, uint32_t value)
{
	return min <= value && value <= max;
}

bool UTF8_IsOverlong(const char* bytes, size_t len)
{
	UTF8Char c = UTF8_GetChar(bytes, len);
	switch(len) {
		case 2:
			if(!InRange(0x0080, 0x07FF, c))
				return true;
			break;
		case 3:
			if(!InRange(0x0800, 0xFFFF, c))
				return true;
			break;
		case 4:
			if(!InRange(0x10000, 0x10FFFF, c))
				return true;
			break;
	}
	return false;
}

/// Unused.
enum class UTF8_Error : uint32_t {
	OK = 0,
	/// There are too many continuation bytes.
	TooManyBytes,
	/// A code point has been overlong encoded.
	OverlongEncoding,
	/// There weren't enough continuation bytes.
	NotEnoughBytes,
};

bool UTF8_Validate(const char* bytes, bool overlongIsError)
{
	for(size_t i = 0; i < strlen(bytes); i++) {
		if(GetBit(bytes[i], 7)) {
			// Invalid continuation byte where starting byte should be.
			// Code poitns must have at least two 1's at their first byte.
			if(!GetBit(bytes[i], 6))
				return false;

			// Count the number of ones in the first byte.
			unsigned count = 0;
			while(count <= 7 && GetBit(bytes[i], 7 - count))
				count++;

			// Count number of continuation bytes.
			for(size_t j = 1; j < count; j++) {
				// A continuation byte can only have a form resembling 10xxxxxx.
				if(!GetBit(bytes[i + j], 7) || GetBit(bytes[i + j], 6))
					return false;
			}

			// An overlong encoding (padding) counts as an error.
			// UTF-8 sequences must have code points encoded with the smallest number of bytes possible.
			if(overlongIsError && UTF8_IsOverlong(bytes + i, count))
				return false;

			/// Unicode goes no further than this value, so we don't either.
			if(UTF8_GetChar(bytes + i, count) > 0x10FFFF)
				return false;

			i += count - 1;
		}
	}
	return true;
}

size_t
UTF8_CountChars(const char* bytes)
{
	size_t nChars = 0;
	for(size_t i = 0; i < strlen(bytes); i++) {
		if(GetBit(bytes[i], 7)) {
			unsigned count = 0;
			while(GetBit(bytes[i], 7 - count))
				count++;
			i += count;
		}
		nChars++;
	}
	return nChars;
}

UTF8Char
UTF8_GetChar(const char* bytes, size_t len)
{
	uint32_t ch = 0;
	{
		uint8_t b = bytes[0];
		b <<= len;
		size_t pos = 5 * len - 6;
		ch = b;
		ch <<= pos;
	}

	for(size_t j = 1; j < len; j++) {
		uint8_t b = bytes[j];
		b <<= 2;
		b >>= 2;
		ch |= ((uint32_t) b) << ((len - 1 - j) * 6);
	}

	return ch;
}

UTF8String
UTF8_Decode(const char* bytes)
{
	UTF8String res;
	res.reserve(UTF8_CountChars(bytes));

	for(size_t i = 0; i < strlen(bytes); i++) {
		if(!GetBit(bytes[i], 7)) {
			res += bytes[i];
		} else {
			unsigned count = 0;
			while(GetBit(bytes[i], 7 - count)) {
				count++;
			}

			res += UTF8_GetChar(bytes + i, count);
			i += count - 1;
		}
	}

	return res;
}

UTF8String
UTF8_SafeDecode(const char* bytes)
{
	if(!UTF8_Validate(bytes))
		return UTF8String();
	return UTF8_Decode(bytes);
}

bool UTF8_Validate(const UTF8String& str)
{
	for(size_t i = 0; i < str.length(); i++) {
		if(str[i] > 0x10FFFF)
			return false;
	}
	return true;
}

std::string
UTF8_Encode(const UTF8String& str)
{
	std::string res;
	res.reserve(str.length());

	for(size_t i = 0; i < str.length(); i++) {
		UTF8Char c = str[i];
		size_t len = 0;

		if(InRange(0x0000, 0x007f, c))
			len = 1;
		else if(InRange(0x0080, 0x07FF, c))
			len = 2;
		else if(InRange(0x0800, 0xFFFF, c))
			len = 3;
		else if(InRange(0x10000, 0x10FFFF, c))
			len = 4;

		switch(len) {
			case 1:
				res += (char) c;
				break;
			case 2:
				res += (char) (0xC0 | c >> 6);
				res += (char) (0x80 | uint8_t(c & 0x0000003F));
				break;
			case 3:
				res += (char) (0xE0 | c >> 12);
				res += (char) (0x80 | uint8_t((c & 0x00000FC0) >> 6));
				res += (char) (0x80 | uint8_t(c & 0x0000003F));
				break;
			case 4:
				res += (char) (0xF0 | c >> 21);
				res += (char) (0x80 | uint8_t((c & 0x0003F000) >> 12));
				res += (char) (0x80 | uint8_t((c & 0x00000FC0) >> 6));
				res += (char) (0x80 | uint8_t(c & 0x0000003F));
				break;
		}
	}

	return res;
}

std::string
UTF8_SafeEncode(const UTF8String& str)
{
	if(!UTF8_Validate(str))
		return "";
	return UTF8_Encode(str);
}
