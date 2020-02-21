/**
 * This file contains implement to pseudo compress or truely compress a given
 * input file and write the result to given output file
 *
 * Author:
 */
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

#ifndef ASCII_MAX
#define ASCII_MAX 256
#endif

#define BUFSIZE 4000

/* add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(string inFileName, string outFileName) {

    ofstream outFile(outFileName, ios::binary);
    if (FileUtils::isEmptyFile(inFileName)) {
        outFile.close();
        return;
    }

    vector<unsigned int> freqs(ASCII_MAX);

    ifstream inFile(inFileName, ios::binary);

    byte ch;
    while (true) {
        inFile.read(reinterpret_cast<char*>(&ch), sizeof(ch));
        if (inFile.eof()) break;
        freqs[ch]++;
    }

    auto tree = HCTree();
    tree.build(freqs);

    /* write freqs */
    for (int i = 0; i < ASCII_MAX; ++i) outFile << freqs[i] << endl;

    /* write encoded file */
    inFile.clear();
    inFile.seekg(0, ios::beg);
    while (true) {
        inFile.read(reinterpret_cast<char*>(&ch), sizeof(ch));
        if (inFile.eof()) break;
        tree.encode(ch, outFile);
    }
    inFile.close();
    outFile.close();
}

/* True compression with bitwise i/o and small header (final) */
void trueCompression(string inFileName, string outFileName) {

    ofstream outFile(outFileName, ios::binary);
    if (FileUtils::isEmptyFile(inFileName)) {
        outFile.close();
        return;
    }

    ifstream inFile(inFileName, ios::binary);
    vector<unsigned int> freqs(ASCII_MAX);

    byte ch;
    while (true) {
        inFile.read(reinterpret_cast<char*>(&ch), sizeof(ch));
        if (inFile.eof()) break;
        freqs[ch]++;
    }
    auto tree = HCTree();
    tree.build(freqs);

    inFile.clear();

    /* write header */

    // write size of file
    inFile.seekg(0, ios::end);
    unsigned long size = inFile.tellg();
    outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

    BitOutputStream out(outFile, BUFSIZE);
    // write huffman tree structure
    tree.encodeTree(out);

    /* write encoded file */
    inFile.seekg(0, ios::beg);
    while (true) {
        inFile.read(reinterpret_cast<char*>(&ch), sizeof(ch));
        if (inFile.eof()) break;
        tree.encode(ch, out);
    }
    out.flush();
    inFile.close();
    outFile.close();
}

/* Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");

    options.positional_help("./path_to_input_file ./path_to_output_file");
    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);
    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help() << std::endl;
        return 0;
    }

    if (userOptions.count("ascii"))
        pseudoCompression(userOptions["input"].as<string>(),
                          userOptions["output"].as<string>());
    else
        trueCompression(userOptions["input"].as<string>(),
                        userOptions["output"].as<string>());

    return 0;
}
