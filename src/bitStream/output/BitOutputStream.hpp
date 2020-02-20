/**
 * Input stream to enable bitwise input
 *
 * Author:
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** output stream for allowing writing data bit by bit */
class BitOutputStream {
  private:
    ostream& out;          // reference to the output stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits have been writen to buf

  public:
    /* initialize with given bufsize
     * Parameter: bufsize - buffer size of BitOutputStream
     * */
    explicit BitOutputStream(ostream& os, unsigned int bufSize)
        : out(os), bufSize(bufSize), nbits(0) {
        buf = new char[bufSize];
    };

    /* release buffer */
    ~BitOutputStream();

    /* write part of buffer written, padding right with zero bits */
    void flush();

    /* write a bit to buffer
     * Parameter: i - 0 / 1 bit to be written
     * */
    void writeBit(unsigned int i);
};

#endif
