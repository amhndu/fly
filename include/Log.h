#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

#ifndef __FILENAME__
#define __FILENAME__ __FILE__
#endif

#define LOG(level) \
if (level > fly::Log::get().getLevel()) ; \
else fly::Log::get().getStream() << "["#level"]" << "[" << __FILENAME__ << ":" << std::dec << __LINE__ << "] "


namespace fly
{

enum Level
{
    Error,
    Info,
    Debug,
};

class Log
{
public:
    void setLogStream(std::ostream& stream);
    Log& setLevel(Level level);
    Level getLevel();

    std::ostream& getStream();
    static Log& get();
private:
    Level m_logLevel;
    std::ostream* m_logStream;
};


// Courtesy of http://wordaligned.org/articles/cpp-streambufs#toctee-streams
class TeeBuf : public std::streambuf
{
    public:
        // Construct a streambuf which tees output to both input
        // streambufs.
        TeeBuf(std::streambuf* sb1, std::streambuf* sb2);
    private:
        // This tee buffer has no buffer. So every character "overflows"
        // and can be put directly into the teed buffers.
        virtual int overflow(int c);
        // Sync both teed buffers.
        virtual int sync();
    private:
        std::streambuf* m_sb1;
        std::streambuf* m_sb2;
};

// Can be used to log into two ostream objects (like cout plus file)
class TeeStream : public std::ostream
{
    public:
        // Construct an ostream which tees output to the supplied
        // ostreams.
        TeeStream(std::ostream& o1, std::ostream& o2);
    private:
        TeeBuf m_tbuf;
};

};
#endif // LOG_H
