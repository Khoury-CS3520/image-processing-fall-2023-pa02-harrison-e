#ifndef IMAGELIB_IMAGE_H
#define IMAGELIB_IMAGE_H
#include <string>
#include "PixelVector.h"

using std::string;

class Image {
private:
    // width, height, and components used
    unsigned x;
    unsigned y;
    channel_t componentsUsed;

    // CharArray is an alias for an array of chars
    // Used to interact with STBI
    typedef unsigned char* charArray;

    // PixelMatrix is an alias for a pointer to a PixelVector
    // A.K.A.: a pointer to a pointer to a Pixel
    // Represents, in data, the column of rows of pixels
    typedef PixelVector* PixelMatrix;

    // input filepath
    string inputFilepath;
    // output filepath
    string outputFilepath;
    // pointer to array of pixel vectors
    PixelMatrix pixels;

public:
    /////////////////////////
    // constructors
    // default constructor (for tests)
    Image() : x {0}, y {0}, componentsUsed{GR}, inputFilepath {""}, outputFilepath {""}, pixels {nullptr} {}

    // destructor
    ~Image();

    // constructor for grey box
    Image(unsigned X, unsigned Y);


    /////////////////////////
    // TODO file i/o
    // constructor reads from ../sampleImages/filename, with specified # components
    Image(const char* filename, unsigned X, unsigned Y, channel_t componentsUsed);

    // write an image to `filepath`, with specified # components
    // 2D PixelMatrix to 1D char array
    void write(string filepath, channel_t componentsUsed);


    /////////////////////////
    // TODO transformations
    void flipHorizontal();
    void flipVertical();
    void rotatePos90();
    void rotateNeg90();
    void addBorder(Pixel & borderPx, unsigned width);
    void pointillize();

    /////////////////////////
    // convenience

    // fetch a pixel at a given coordinate
    Pixel & pixelAt(unsigned X, unsigned Y);

    // fetch a row at a given X
    // NOTE: columnAt creates a new PixelVector
    // make sure to DELETE this if necessary
    PixelVector & columnAt(unsigned X);

    // fetch a column at a given Y
    PixelVector & rowAt(unsigned Y);
    // const version
    PixelVector & constRowAt(unsigned Y) const;

    // return width
    unsigned getWidth() const { return this->x; }

    // return height
    unsigned getHeight() const { return this->y; }

    /////////////////////////
    // overloaded operators
    friend std::ostream & operator<<(std::ostream& out, const Image& img);
    PixelVector & operator[](unsigned index);
    Image(const Image &that);
};

#endif //IMAGELIB_IMAGE_H
