#include "Buffer.hpp"

namespace SWAN
{
    Buffer::Buffer(unsigned Capacity) : Capacity(Capacity) { buff = new char[Capacity]; }
    Buffer::Buffer(const Buffer& other)
	: Capacity(other.Capacity),
	  Size(other.Size),
	  CursorPosition(other.CursorPosition)
    {
	buff = new char[Capacity];
	for(int i = 0; i < Size; i++)
	    buff[i] = other.buff[i];
    }

    Buffer::Buffer(Buffer&& other) : Capacity(std::move(other.Capacity)),
				     Size(std::move(other.Size)),
				     CursorPosition(std::move(other.CursorPosition))
    {
	buff = std::move(other.buff);
	other.buff = nullptr;
    }

    Buffer::~Buffer() { delete[] buff; }

    Buffer& Buffer::operator=(const Buffer& other) {
	Capacity = other.Capacity;
	Size = other.Size;
	CursorPosition = other.CursorPosition;

	buff = new char[Capacity];
	for(int i = 0; i < Size; i++)
	    buff[i] = other.buff[i];

	return *this;
    }

    Buffer& Buffer::operator=(Buffer&& other) {
	Capacity = std::move(other.Capacity);
	Size = std::move(other.Size);
	CursorPosition = std::move(other.CursorPosition);
	buff = std::move(other.buff);
	other.buff = nullptr;
	return *this;
    }

    Buffer::Buffer(const Vector<String>& v) : Capacity(4096) {
	buff = new char[Capacity];
	Reset();
	AddLinesVec(v);
	CursorPosition = 0;
    }

    Buffer& Buffer::operator=(const Vector<String>& v){
	Reset();
	AddLinesVec(v);
	return *this;
    }

    void Buffer::Down() {
	int x = GetCursorX();
	NextLine();

	if(GetCurrentLineLength() < x) {
	    SeekEOL();
	} else {
	    Right(x);
	}
    }

    void Buffer::Up() {
	int x = GetCursorX();
	PreviousLine();

	if(GetCurrentLineLength() < x)
	    SeekEOL();
	else
	    Right(x);
    }

    void Buffer::AddLinesVec(const Vector<String>& vec) {
	for(const String& s : vec) {
	    ReplaceCurrentLine(s);
	    InsertAtCursor('\n');
	}
    }

    Vector<String> Buffer::AsLinesVec() const {
	int orig_pos = CursorPosition;

	Vector<String> res;
	res.reserve(GetNumberOfLines());

	((Buffer*)(this))->CursorPosition = 0;

	for(int i = 0; i < GetNumberOfLines(); i++) {
	    res.push_back(GetCurrentLine());
	    ((Buffer*)(this))->NextLine();
	}

	((Buffer*)(this))->CursorPosition = orig_pos;

	return res;
    }

    void Buffer::ReplaceCurrentLine(String line) {
	SeekBOL();
	while(Size > 0
	      && GetCurrentChar() != '\n'
	      && CursorPosition != FindEOL())
	    DeleteAfterCursor();

	for(int i = 0; i < line.length() && line[i] != '\n'; i++)
	    InsertAtCursor(line[i]);
    }

    void Buffer::DeleteCurrentLine() {
	SeekBOL();
	while(Size > 0 && GetCurrentChar() != '\n' && CursorPosition != FindEOL())
	    DeleteAfterCursor();
	DeleteAfterCursor();
    }

    void Buffer::OverwriteAtCursor(char c) {
	if(Size == 0
	   || GetCurrentChar() == '\n'
	   || CursorPosition == Size) {
	    InsertAtCursor(c);
	} else {
	    SetCharacterUnderCursor(c);
	    Right();
	}
    }

    void Buffer::InsertAtCursor(char c) {
	if(Size != 0)
	    for(int i = Size + 1; i > CursorPosition; i--)
		buff[i] = buff[i-1];

	Size = Util::Clamp<unsigned>(Size+1, 0, Capacity);

	buff[CursorPosition] = c;
	Right();
    }

    int Buffer::FindBOL() const {
	for(int i = CursorPosition; i > 0; i--)
	    if(buff[i-1] == '\n') return i;
	return 0;
    }

    int Buffer::FindEOL() const { 
	for(int i = CursorPosition; i <= Size; i++)
	    if(buff[i] == '\n') return i;
	return Size;
    }

    int Buffer::Find(char c) const {
	for(int i = CursorPosition; i <= Size; i++) {
	    if(buff[i] == c) return i;
	}
	return -1;
    }

    bool Buffer::Seek(char c) {
	int f = Find(c);
	if(f == -1) { return false; }
	else { CursorPosition = f; return true; }
    }

    int Buffer::FindBackwards(char c) const {
	for(int i = CursorPosition; i >= 0; i--) {
	    if(buff[i] == c) return i;
	}
	return -1;
    }

    bool Buffer::SeekBackwards(char c) {
	int f = FindBackwards(c);
	if(f == -1) { return false; }
	else { CursorPosition = f; return true; }
    }

    void Buffer::DeleteAtCursor() {
	if(Size < 1 || CursorPosition == 0)
	    return;

	Size--;
	Left();

	for(int i = CursorPosition; i < Size; i++)
	    buff[i] = buff[i+1];
    }

    void Buffer::DeleteAfterCursor() {
	if(Size < 1 || CursorPosition == Size)
	    return;

	Size--;

	for(int i = CursorPosition; i < Size; i++)
	    buff[i] = buff[i+1];
    }
}
