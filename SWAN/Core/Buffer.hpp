#ifndef SWAN_BUFFER_HPP
#define SWAN_BUFFER_HPP

#include "Defs.hpp"
#include "Utility/Math.hpp"
#include <algorithm>

namespace SWAN
{
	class Buffer
	{
	  public:
		Buffer(unsigned Capacity = 4096);
		Buffer(const Buffer& other);
		Buffer(Buffer&& other);
		~Buffer();

		Buffer& operator=(const Buffer& other);
		Buffer& operator=(Buffer&& other);

		Buffer(const Vector<String>& v);
		Buffer& operator=(const Vector<String>& v);

		/// Storage.
		char* buff; // 4kb of storage, should be enough.

		/// Size of allocated character buffer. Usually useless, use Size instead.
		unsigned Capacity;

		/// Size of currently used part of buffer.
		unsigned Size = 0;

		/// Position of cursor in buffer.
		int CursorPosition = 0;

		/// Moves the cursor backwards a given amount of characters (1 by default).
		void Left(unsigned amt = 1) { CursorPosition = Util::Clamp<int>(CursorPosition - amt, 0, Size); }
		/// Moves the cursor forwards a given amount of characters (1 by default).
		void Right(unsigned amt = 1) { CursorPosition = Util::Clamp<int>(CursorPosition + amt, 0, Size); }

		/// Moves the cursor to the beginning of the previous line.
		void PreviousLine()
		{
			SeekBOL();
			Left(1);
			SeekBOL();
		}

		/// Moves the cursor to the beginning of the next line.
		void NextLine()
		{
			SeekEOL();
			Right(1);
		}

		/// Moves the cursor down a given amount of lines.
		void Down(unsigned amt)
		{
			for(int i = 0; i < amt; i++)
				Down();
		}
		/// Moves the cursor up a given amount of lines.
		void Up(unsigned amt)
		{
			for(int i = 0; i < amt; i++)
				Up();
		}

		/// Moves the cursor down by a line.
		void Down();
		/// Moves the cursor up by a line.
		void Up();

		/// Empties the buffer's contents.
		void Reset()
		{
			Size = 0;
			CursorPosition = 0;
		}

		/// Adds a vector of lines to the end of the buffer.
		void AddLinesVec(const Vector<String>& vec);

		/// Cast the buffer's contents to a vector<string> of each line.
		inline operator Vector<String>() const { return AsLinesVec(); }

		/// Convert the buffer's contents to a vector<string> of each line.
		Vector<String> AsLinesVec() const;

		/// Cast the buffer's contents to a string.
		inline operator String() const { return AsString(); }
		/// Convert the buffer's contents to a string.
		inline String AsString() const { return String(buff, Size); }

		/// Finds how far away from the start of the current line the cursor is.
		int GetCursorX() const { return CursorPosition - FindBOL(); }
		/// Alias for GetCurrentLineNumber(). Finds which line the cursor is on.
		inline int GetCursorY() const { return GetCurrentLineNumber(); }

		/// Finds which line the cursor is on.
		unsigned GetCurrentLineNumber() const { return std::count(buff, buff + CursorPosition, '\n'); }

		/// Calculates the length of the line the cursor is currently on.
		unsigned GetCurrentLineLength() const
		{
			int e = FindEOL();
			int b = FindBOL();
			return e - b;
		}

		/// Retrieves the current line as a string.
		String GetCurrentLine() const
		{
			int b = FindBOL();
			return String(buff + b, GetCurrentLineLength());
		}

		/// Replaces the contents of the current line with a given string.
		void ReplaceCurrentLine(String line);

		/// Deletes the current line.
		void DeleteCurrentLine();

		/// Counts the number of lines the buffer has.
		inline unsigned GetNumberOfLines() const
		{
			return std::max<unsigned>(1, std::count(buff, buff + Size, '\n') + (buff[Size] == '\n' ? 1 : 1));
		}

		/// Replaces the character at the cursor without moving the cursor.
		void SetCharacterUnderCursor(char c) { buff[CursorPosition] = c; }

		/// Either replaces the character at the cursor and moves the cursor forwards
		/// or calls InsertAtCursor(c) if at the end of the current line or the buffer.
		void OverwriteAtCursor(char c);

		/// Inserts a character at the cursor's given position, moves all
		/// characters after the cursor forwards and moves the cursor forwards.
		void InsertAtCursor(char c);

		/// Returns the character the cursor is marking.
		char GetCurrentChar() { return buff[CursorPosition]; }

		/// Finds the index for the beginning of the current line.
		int FindBOL() const;
		/// Finds the index for the end of the current line.
		int FindEOL() const;

		/// @brief Finds a character ahead of the cursor.
		/// @return The position of the character if found, -1 otherwise.
		int Find(char c) const;

		/// @brief Moves the cursor to the next closest occurence of the character.
		/// @return True if the character was found and the cursor was moved, false otherwise.
		bool Seek(char c);

		/// @brief Finds a character before the cursor.
		/// @return The position of the character if it's found, -1 otherwise.
		int FindBackwards(char c) const;

		/// @brief Moves the cursor to the previous closest occurence of the character c.
		/// @return True if the character was found and the cursor was moved, false otherwise.
		bool SeekBackwards(char c);

		/// Moves the cursor to the beginning of the current line or to the end of the buffer.
		void SeekBOL() { CursorPosition = FindBOL(); }

		/// Moves the cursor to the end of the current line or to the end of the buffer.
		void SeekEOL() { CursorPosition = FindEOL(); }

		/// Deletes the character that the cursor is on and moves it backwards.
		/// Equivalent to pressing the Backspace key.
		void DeleteAtCursor();

		/// Deletes the character that is after the cursor without moving the cursor itself.
		/// Equivalent to pressing the Delete key.
		void DeleteAfterCursor();
	};
} // namespace SWAN

#endif
