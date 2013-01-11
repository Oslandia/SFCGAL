#include <streambuf>


///
/// Streambuf on a char*
///
/// http://www.mr-edd.co.uk/blog/beginners_guide_streambuf
///
class CharArrayBuffer : public std::streambuf
{
    public:
        CharArrayBuffer(const char *begin, const char *end);
        explicit CharArrayBuffer(const char *str);

    private:
        int_type underflow();
        int_type uflow();
        int_type pbackfail(int_type ch);
        std::streamsize showmanyc();

        // copy ctor and assignment not implemented;
        // copying not allowed
        CharArrayBuffer(const CharArrayBuffer &);
        CharArrayBuffer &operator= (const CharArrayBuffer &);

	std::streampos seekpos( std::streampos pos, std::ios_base::openmode );
	std::streampos seekoff ( std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode );
 private:
        const char * const begin_;
        const char * const end_;
        const char * current_;
};
