#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "HCTree.hpp"

using namespace std;
using namespace testing;

class SingleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SingleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs.assign(256, 0);
        freqs['a'] = 2;
        tree.build(freqs);
    }
};

TEST_F(SingleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "0");
}

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs;
        freqs.assign(256, 0);
        freqs['a'] = 2;
        freqs['c'] = 2;
        freqs['b'] = 3;
        tree.build(freqs);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "11");

    os.str("");
    tree.encode('c', os);
    ASSERT_EQ(os.str(), "10");

    os.str("");
    tree.encode('b', os);
    ASSERT_EQ(os.str(), "0");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("10");
    ASSERT_EQ(tree.decode(is), 'c');

    is.clear();
    is.str("11");
    ASSERT_EQ(tree.decode(is), 'a');

    is.clear();
    is.str("0");
    ASSERT_EQ(tree.decode(is), 'b');
}

class WorkSheetHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    WorkSheetHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs.assign(256, 0);
        freqs['a'] = 3;
        freqs['b'] = 5;
        freqs['c'] = 4;
        freqs['d'] = 26;
        freqs['e'] = 12;
        tree.build(freqs);
    }
};

TEST_F(WorkSheetHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    cout << "a: " << os.str() << endl;

    os.str("");
    tree.encode('b', os);
    cout << "b: " << os.str() << endl;

    os.str("");
    tree.encode('c', os);
    cout << "c: " << os.str() << endl;

    os.str("");
    tree.encode('d', os);
    cout << "d: " << os.str() << endl;

    os.str("");
    tree.encode('e', os);
    cout << "e: " << os.str() << endl;
}
