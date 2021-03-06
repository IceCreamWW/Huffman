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

#ifndef BUFSIZE
#define BUFSIZE 4000
#endif

/* Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(string inFileName, string outFileName) {

    ofstream outFile(outFileName, ios::binary);
    if (FileUtils::isEmptyFile(inFileName)) {
        outFile.close();
        return;
    }

    vector<unsigned int> freqs(ASCII_MAX);
    ifstream inFile(inFileName);

    unsigned int freq;
    for (int i = 0; i < ASCII_MAX; ++i) {
        inFile >> freq;
        freqs[i] = freq;
    }

    auto tree = HCTree();
    tree.build(freqs);


    char enter;
    inFile.get(enter);
    while (!inFile.eof()) {
        byte code = tree.decode(inFile);
        if (inFile.eof()) break;
        outFile.write(reinterpret_cast<char*>(&code), sizeof(code));
    }
    inFile.close();
    outFile.close();
}

/* True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {

    ofstream outFile(outFileName, ios::binary);
    if (FileUtils::isEmptyFile(inFileName)) {
        outFile.close();
        return;
    } 

    ifstream inFile(inFileName, ios::binary);


    // read size of original
    unsigned long size = 0;
    inFile.read(reinterpret_cast<char *>(&size), sizeof(size));

    BitInputStream in(inFile, BUFSIZE);
    auto tree = HCTree();
    tree.decodeTree(in);

    while (size != 0) {
        byte code = tree.decode(in);
        outFile.write(reinterpret_cast<char*>(&code), sizeof(code));
        --size;
    }
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
        pseudoDecompression(userOptions["input"].as<string>(),
                            userOptions["output"].as<string>());
    else
        trueDecompression(userOptions["input"].as<string>(),
                          userOptions["output"].as<string>());

    return 0;
}
