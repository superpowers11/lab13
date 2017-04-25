/* huffman.cc
 *
 * Implementation of the methods defined in huffman.h
 *
 * Copyright (c) 2013 - Russell C. Bjork
 *
 * MODIFIED BY: Anna Pelletier and Stephanie Powers
 */

#include "huffman.h"
#include <climits>

HuffmanTree::HuffmanTree()
: _root(NULL)
{ }

void HuffmanTree::read(istream & treefile)
{
    _root = Node::read(treefile);
}

void HuffmanTree::write(ostream & treefile) const
{
    _root -> write(treefile);
}

#ifndef PROFESSOR_VERSION

// Fills in  a Huffman Tree using character frequency data in a document
// Includes one instance of the EOF_CHAR.
// Uses preorder traversal of the tree.
// When a leaf is encountered, the character stored there will be written to the
//file. When internal node is encountered, a special character is written to file.
void HuffmanTree::fillIn(istream & document)
{

















}

// Uses Huffman Tree to compress a file (according to rules of frequency, etc.)
// Reads text file and applies compression rules to each of its characters, even
//new line and spaces.
// At end of document, compress EOF_CHAR and include it at end of compressed
//file.
void HuffmanTree::compress(istream & originalDocument,
  ostream & compressedDocument) const
  {
    int * bits;
    int * count;
    createCodeTable(bits, count);

    char c;
    while(! originalDocument.eof())
    {
      originalDocument.get(c);
      //originalDocument >> c;
      std::cerr << "Got char " << c << '\n';
      int bitsToWrite = bits[c];
      int countToWrite = count[c];
      if (! originalDocument.eof())
      {
        insertBits(compressedDocument, bitsToWrite, countToWrite);
        std::cerr << "inserted a character; not at end" << '\n';
      }
      else
      {
        insertBits(compressedDocument, bits[EOF_CHAR], count[EOF_CHAR]);
        std::cerr << "inserted end character" << '\n';
        flushBits(compressedDocument);
        std::cerr << "flushed characters" << '\n';
      }
    }

  }


// Uses Huffman Tree to translate compressed file into its decompressed form
// Stops at EOF_CHAR (doesn't add it to decompressed file.)
void HuffmanTree::decompress(istream & compressedDocument,
                             ostream & decompressedDocument) const
{
// We take a Huffman tree and compare the compressed document to it.
      // We do this by going bit by bit through the compressed file
      //(stored in binary) until we have reached a leaf node in the Huffman tree.
      //Then, we write the character we reached to the document we're creating
      //called decompressedDocument in our code.
      // We continue doing this until we reach the EOF_CHAR; then we stop before
      //we add the EOF_CHAR to the decompressedDocument.
    Node * currNode = _root;
    bool finished = false;
    while (!finished)
    {
      if (currNode->isInternal()) {
        int currentBit = extractBit(compressedDocument);
        std::cerr << "Current Bit: " << currentBit << '\n';
        if (currentBit == 0) {
          // go left
          std::cerr << "Going left" << '\n';
          currNode = currNode->getLChild();
        }
        else {
          // go right
          std::cerr << "Going right" << '\n';
          currNode = currNode->getRChild();
        }
      }
      else
      {
        // is leaf
        char newChar = currNode->getCharacter();
        std::cerr << "New character = " << newChar << '\n';
        if (newChar != EOF_CHAR)
        {
          decompressedDocument.put(newChar);
          currNode = _root;
        }
        else
        {
          finished = true;
        }
      }
    }
}

#endif

void HuffmanTree::createCodeTable(int * & bits, int * & count) const
{
    bits = new int[CHAR_MAX + 1];
    count = new int[CHAR_MAX + 1];
        // +1 to allow for 0 entry
    _root -> fillInCodeTable(bits, count, 0, 0);
}

// Global variables used by bit operations

#define BITS_PER_CHARACTER 8

static int bitsExtracted = BITS_PER_CHARACTER ;
static int bitsInserted = 0;
static char currentExtractChar, currentInsertChar;

int HuffmanTree::extractBit(istream & input)
{
    // Read a single character if need be

    if (bitsExtracted >= BITS_PER_CHARACTER)
    {
        input.get(currentExtractChar);
        bitsExtracted = 0;
    }

    // Extract next bit from current character

    int result = ((signed char) currentExtractChar) < 0; // Means leftmost bit 1
    currentExtractChar <<= 1;
    bitsExtracted ++;
    return result;
}

void HuffmanTree::insertBits(ostream & output, int bits, int count)
{
    for (int i = count - 1; i >= 0; i --)
    {
        // Insert bit at position i into current character

        currentInsertChar <<= 1;
        currentInsertChar |= ((bits & (1 << i)) != 0);
        bitsInserted ++;

        // Write character if full

        if (bitsInserted >= BITS_PER_CHARACTER)
        {
            output.put(currentInsertChar);
            bitsInserted = 0;
        }
    }
}

void HuffmanTree::flushBits(ostream & output)
{
    if (bitsInserted > 0)
    {
        // This loop effectively inserts 0's until currentInsertChar is full,
        // putting the first "real" bit in the leftmost position

        while (bitsInserted < BITS_PER_CHARACTER)
        {
            currentInsertChar <<= 1;
            bitsInserted ++;
        }
        output.put(currentInsertChar);
    }
}

#ifdef PROFESSOR_VERSION

#define QUOTE(Q) #Q
#define INCLUDE_NAME(X) QUOTE(X)
#include INCLUDE_NAME(PROFESSOR_VERSION)

#endif
