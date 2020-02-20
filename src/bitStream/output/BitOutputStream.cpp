/**
 * Implement output stream to enable bitwise output
 *
 * Author:
 */
#include "BitOutputStream.hpp"

BitOutputStream::~BitOutputStream() {
    free(buf);
}

void BitOutputStream::flush() {
    if (nbits == 0)
        return;

    int flushSize = (nbits - 1) / 8 + 1;
    out.write(buf, flushSize);
    nbits = 0;
    fill(buf, buf + bufSize, 0);
}

void BitOutputStream::writeBit(unsigned int i) {
    if (nbits == bufSize * 8)
        flush();
    buf[nbits / 8] |= (i << (7 - nbits % 8));
    ++nbits;
}

