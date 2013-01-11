#include <iostream>

#include <SFCGAL/tools/CharArrayBuffer.h>

#include <cstring>

CharArrayBuffer::CharArrayBuffer(const char *begin, const char *end) :
    begin_(begin),
    end_(end),
    current_(begin_)
{
}

CharArrayBuffer::CharArrayBuffer(const char *str) :
    begin_(str),
    end_(begin_ + std::strlen(str)),
    current_(begin_)
{
}


std::streampos CharArrayBuffer::seekoff( std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode )
{
	if ( way == std::ios_base::cur ) {
		if ( current_ + off < end_ ) {
			current_ += off;
		}
		else {
			return -1;
		}
	}
	else if ( way == std::ios_base::beg ) {
		if ( begin_ + off < end_ ) {
			current_ = begin_ + off;
		}
		else {
			return -1;
		}
	}
	return current_ - begin_;
}

std::streampos CharArrayBuffer::seekpos( std::streampos pos, std::ios_base::openmode )
{
	if ( begin_ + pos >= end_ ) {
		return -1;
	}
	current_ = begin_ + pos;
	return current_ - begin_;
}

CharArrayBuffer::int_type CharArrayBuffer::underflow()
{
    if (current_ == end_)
        return traits_type::eof();

    return traits_type::to_int_type(*current_);
}

CharArrayBuffer::int_type CharArrayBuffer::uflow()
{
    if (current_ == end_)
        return traits_type::eof();

    return traits_type::to_int_type(*current_++);
}

CharArrayBuffer::int_type CharArrayBuffer::pbackfail(int_type ch)
{
    if (current_ == begin_ || (ch != traits_type::eof() && ch != current_[-1]))
        return traits_type::eof();

    return traits_type::to_int_type(*--current_);
}

std::streamsize CharArrayBuffer::showmanyc()
{
    return end_ - current_;
}
