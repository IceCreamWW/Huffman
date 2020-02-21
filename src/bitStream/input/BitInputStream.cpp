/**
 * Implement input stream to enable bitwise input
 *
 * Author:
 */
#include "BitInputStream.hpp"

BitInputStream::~BitInputStream() {
    delete[] buf;
}

void BitInputStream::fill() {
    in.read(buf, bufSize);
    nbits = 0;
}

bool BitInputStream::atEndOfFile() { return (!in) && (nbits == in.gcount() * 8); }

bool BitInputStream::eof() { return eofBit; }

unsigned int BitInputStream::readBit() {

    if (atEndOfFile()) {
        eofBit = true;
        return 0;
    }
    int bit = (buf[nbits / 8] >> (7 - nbits % 8) & 0x1);
    ++nbits;
    if (nbits == bufSize * 8) fill();
    return bit;
}
