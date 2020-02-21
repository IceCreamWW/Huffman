/**
 * huffman tree definition
 *
 * Author:
 */
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <string>
#include <vector>

#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

#ifndef ASCII_MAX
#define ASCII_MAX 256
#endif

using namespace std;

/** Huffman tree definition
 * functions for performing huffman encoding and decoding */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes

  public:
    /* allocate space for leaves vector */
    HCTree() { root = 0; leaves.assign(ASCII_MAX, nullptr); }

    /* recursively free from root */
    ~HCTree();

    /* Build Huffman Tree according to frequency of words;
     * Parameter: freqs
     */
    void build(const vector<unsigned int>& freqs);

    /* write the encoding bits of given symbol to given BitOutputStream */
    void encode(byte symbol, BitOutputStream& out) const;

    /* write the pseudo encoding string of given symbol to given output stream
     * Parameter: symbol - symbol to be encoded
     * Parameter: out - output stream to write coded string to
     */
    void encode(byte symbol, ostream& out) const;

    /* Return decoded byte of the sequence of bits from given BitInputStream
     */
    byte decode(BitInputStream& in) const;

    /* Return decoded byte of the sequence of chars from given input stream
     */
    byte decode(istream& in) const;

    /* public interface to encode a tree
     * recursively invoke encodeNode to do actual things
     * */
    void encodeTree(BitOutputStream& out);

    /* public interface to decode a tree
     * recursively invoke decodeNode to do actual things
     * */
    void decodeTree(BitInputStream& in);

  private:
    /* generate code for efficient decoding */
    void genCode(HCNode* node, string in_code);

    /* recursively free node */
    void freeNode(HCNode* node);

    /* Recursively encode node to bits
     * Parameter: node - node to be encoded
     * Parameter: out - bit output stream to write encoded node to
     * */
    void encodeNode(HCNode* node, BitOutputStream& out);

    /* Recursively return decoded node from bits
     * Parameter: in - bit input stream to read code from
     * */
    HCNode* decodeNode(BitInputStream& in);
};

#endif  // HCTREE_HPP
