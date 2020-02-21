/**
 * huffman tree implement
 *
 * Author:
 */
#include "HCTree.hpp"

HCTree::~HCTree() {
    if (root) freeNode(root);
}

/* build HCTree from given freqs vector
 * Parameter: freqs - the vector to build tree from
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    for (int i = 0; i < ASCII_MAX; ++i) {
        int freq = freqs.at(i);
        if (freq == 0)
            continue;
        auto node = new HCNode(freq, i);
        leaves[i] = node;
        pq.push(node);
    }
    if (pq.empty()) return;

    // fake a node if there is only one character in tree;
    if (pq.size() == 1)
        pq.push(new HCNode(pq.top()->symbol, pq.top()->count + 1));

    while (pq.size() >= 2) {
        HCNode* c0 = pq.top();
        pq.pop();
        HCNode* c1 = pq.top();
        pq.pop();
        HCNode* parent = new HCNode(c0->count + c1->count, c0->symbol, c0, c1);
        c0->p = parent;
        c1->p = parent;
        pq.push(parent);
    }
    root = pq.top();
    genCode(root, "");
}

void HCTree::encode(byte symbol, BitOutputStream& out) const {
    string code = leaves.at(symbol)->code;
    for (char& c : code) out.writeBit((unsigned int)c - '0');
}

void HCTree::encode(byte symbol, ostream& out) const {
    string code = leaves.at(symbol)->code;
    out.write(code.c_str(), code.size());
}

byte HCTree::decode(BitInputStream& in) const {
    HCNode* node = root;
    unsigned int bit;
    while (true) {
        bit = in.readBit();
        if (in.eof()) return 0xFF;
        node = (bit == 0 ? node->c0 : node->c1);
        if (node->c0 == 0) return node->symbol;
    }
}

byte HCTree::decode(istream& in) const {
    HCNode* node = root;
    char c;
    while (in.get(c)) {
        node = (c == '0' ? node->c0 : node->c1);
        if (node->c0 == 0) return node->symbol;
    }
    return 0xFF;
}

void HCTree::genCode(HCNode* node, string in_code) {
    node->code = in_code;
    if (node->c0 != 0) {
        genCode(node->c0, in_code + "0");
        genCode(node->c1, in_code + "1");
    }
}

void HCTree::freeNode(HCNode* node) {
    if (node->c0 != 0) {
        freeNode(node->c0);
        freeNode(node->c1);
    }
    delete node;
}


void HCTree::encodeTree(BitOutputStream& out) {
    encodeNode(root, out);
}

void HCTree::decodeTree(BitInputStream& in) {
    root = decodeNode(in);
}

void HCTree::encodeNode(HCNode* node, BitOutputStream& out) {
    if (node->c0 == 0) {
        out.writeBit(1);
        for (int i = 7; i >= 0; --i)
            out.writeBit((node->symbol >> i) & 0x1);
    } else {
        out.writeBit(0);
        encodeNode(node->c0, out);
        encodeNode(node->c1, out);
    }
}

HCNode* HCTree::decodeNode(BitInputStream& in) {
    if (in.readBit() == 1) {
        char symbol = 0;
        for (int i = 7; i >= 0; --i) symbol |= (in.readBit() << i);
        return new HCNode(0, symbol);
    }
    else {
        HCNode* c0 = decodeNode(in);
        HCNode* c1 = decodeNode(in);
        return new HCNode(0, c0->symbol, c0, c1);
    }
}
