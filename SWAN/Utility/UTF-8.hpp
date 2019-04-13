#ifndef UTF8_HPP
#define UTF8_HPP

#include <cstdint>
#include <string>

using UTF8Char = uint32_t;
using UTF8String = std::basic_string<UTF8Char>;

/// Checks whether a given code point is overlong encoded (padded with extra zeroes).
extern bool UTF8_IsOverlong(const char* bytes, size_t len);

/// Checks whether a given sequence of bytes is a valid UTF-8 sequence.
extern bool UTF8_Validate(const char* bytes, bool overlongIsError = true);

/// Checks whether the given UTF-8 string could be generated from valid UTF-8.
extern bool UTF8_Validate(const UTF8String& str);

/// Counts the number of characters in a UTF-8 sequence. Does not validate input.
extern size_t UTF8_CountChars(const char* bytes);

/// Retrieves a character from a UTF-8 sequence according to the given length.
extern UTF8Char UTF8_GetChar(const char* bytes, size_t len);

/// Translates a UTF-8 sequence into a string of uint32_t.
extern UTF8String UTF8_Decode(const char* bytes);

/// Calls UTF8_Validate() and returns an empty string if it fails, otherwise decodes.
extern UTF8String UTF8_SafeDecode(const char* bytes);

/// Translates a string of uint32_t back into a UTF-8 sequence.
extern std::string UTF8_Encode(const UTF8String& str);

/// Calls UTF8_Validate() and returns an empty string if it fails, otherwise encodes.
extern std::string UTF8_SafeEncode(const UTF8String& str);

#endif
