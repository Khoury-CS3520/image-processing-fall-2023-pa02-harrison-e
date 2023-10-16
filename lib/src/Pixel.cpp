#include "Pixel.h"

Pixel::Pixel(unsigned char gr) {
    this->r = this->b = this->g = gr;
    this->componentsUsed = channel_t(1);
}

Pixel::Pixel(unsigned char gr,
             unsigned char a) {
    this->r = this->b = this->g = gr;
    this->a = a;
    this->componentsUsed = channel_t(2);
}

Pixel::Pixel(unsigned char r,
             unsigned char g,
             unsigned char b) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->componentsUsed = channel_t(3);
}

Pixel::Pixel(unsigned char r,
             unsigned char g,
             unsigned char b,
             unsigned char a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    this->componentsUsed = channel_t(4);
}

Pixel::Pixel(const Pixel &copyMe) {
    this->r = copyMe.r;
    this->g = copyMe.g;
    this->b = copyMe.b;
    this->a = copyMe.a;
    this->componentsUsed = copyMe.componentsUsed;
}

Pixel& Pixel::operator=(const Pixel & p) {
    this->r = p.r;
    this->g = p.g;
    this->b = p.b;
    this->a = p.a;
    this->componentsUsed = p.componentsUsed;

    // return invoking obj, dereferenced
    return *this;
}

ostream& operator<<(ostream& out, const Pixel& px) {
    switch (px.componentsUsed) {
        case GR:
            out << +px.r;
            break;
        case GRa:
            out << +px.r << ", " << +px.a;
            break;
        case RGB:
            out << +px.r << ", " << +px.g << ", " << +px.b;
            break;
        case RGBa:
            out << +px.r << ", " << +px.g << ", " << +px.b << ", " << +px.a;
    }
    return out;
}
