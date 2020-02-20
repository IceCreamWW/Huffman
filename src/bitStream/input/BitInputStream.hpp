/**
 * Input stream to enable bitwise input
 *
 * Author:
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** input stream for allowing reading data bit by bit */
class BitInputStream {
  private:
    istream& in;           // reference to the input stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits that have been read from buf
    bool eofBit;           // true when trying to read past the end of the file

    /* returns true when trying to read more bytes than remaining byte in file
     * and buffer was then also fully read.
     * Otherwise, return false */
    bool atEndOfFile();

  public:
    /* initialize with given bufsize and fill the buffer
     * Parameter: bufsize - buffer size of BitIutputStream
     * */
    explicit BitInputStream(istream& is, unsigned int bufSize)
        : in(is), bufSize(bufSize), nbits(0) {
        buf = new char[bufSize];
        fill();
    };

    /* release buffer */
    ~BitInputStream();

    /* fill the buffer of size bufSize from input stream */
    void fill();

    /* returns true when trying to read past the end of the file, false otherwise */
    bool eof();

    /* read a bit from buffer, fill the bufer from input stream if all bits have
     * already been read   */
    unsigned int readBit();
};

#endif