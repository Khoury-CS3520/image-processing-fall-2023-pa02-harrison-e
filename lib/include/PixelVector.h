#ifndef IMAGELIB_PIXELVECTOR_H
#define IMAGELIB_PIXELVECTOR_H
#include "Pixel.h"

class PixelVector {
private:
    // the vector of pixels
    Pixel* vec {};
    // the length (# of elements) of the vector
    unsigned length;
    // the capacity of the vector
    unsigned capacity;
    // channel type
    channel_t componentsUsed;

public:
    /////////////////////////
    // constructors & destructor
    PixelVector();
    ~PixelVector();

    // 2-arg ctor
    PixelVector(unsigned capacity, channel_t componentsUsed);

    // copy ctor
    PixelVector(const PixelVector &copyMe);

    /////////////////////////
    // overloaded ops
    PixelVector& operator=(const PixelVector&);
    Pixel& operator[](unsigned index);
    friend std::ostream& operator<<(std::ostream& out, const PixelVector& v);


    /////////////////////////
    // transformations
    // resize
    // delta -> how much to change capacity by
    // offset -> by how much do we shift existing elements
    void resize(unsigned delta, unsigned offset);

    // push to back
    void pushBack(Pixel& pixel);

    // reverse this vec
    void reverse();

    /////////////////////////
    // getters

    // capacity
    unsigned getCapacity() { return this->capacity; }

    // length
    unsigned getLength() { return this->length; }

    // components used
    channel_t getCompUsed() { return this->componentsUsed; }

    // pixel at (const ref)
    Pixel& pixelAt(unsigned index) const;
};

#endif //IMAGELIB_PIXELVECTOR_H
