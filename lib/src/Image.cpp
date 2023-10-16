#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"


/////////////////////////
// constructors & destructor

Image::~Image() {
    delete[] pixels;
    cout << "img dtor" << endl;
}

// make a grey image of specified dimensions
Image::Image(const unsigned X, const unsigned Y) {
    this->x = X;
    this->y = Y;
    this->componentsUsed = GR;
    this->pixels = new PixelVector [Y];
    auto p = Pixel(100);
    Pixel* pp = nullptr;

    for (unsigned y = 0; y < this->y; y++) {
        auto pn = Pixel(p.getR() + 5*y);
        pp = &pn;
        for (unsigned x = 0; x < this->x; x++) {
            auto pnn = Pixel(pp->getR() + 5);
            pp = &pnn;
            pixels[y].pushBack(*pp);
        }
    }
}



/////////////////////////
// file i/o

Image::Image(const char* filename, const unsigned X, const unsigned Y, const channel_t componentsUsed) {
    cout << "reading image from ../sampleImages/" << filename;
    int width = int(X);
    int height = int(Y);
    int n = int(componentsUsed);
    const char* filepath = strcat("../sampleImages/",filename);
    charArray data = stbi_load(filepath, &width, &height, &n, 0);
    if (!data) {
        cerr << "Could not read image from file: ../sampleImages/" << filepath << endl;
    } else {
        this->x = X;
        this->y = Y;
        this->componentsUsed = componentsUsed;
        this->pixels = new PixelVector [Y];
        unsigned gr, r, g, b, a = 0;
        // `i` indexes into `data`, increments by # of components
        for (unsigned i = 0; i < x * y * n; i += n) {
            unsigned px = i % x;
            unsigned py = i / x;
            switch (this->componentsUsed) {
                case GR:
                    gr = data[i];
                    pixels[py][px] = Pixel(gr);
                    break;
                case GRa:
                    gr = data[i];
                    a  = data[i+1];
                    pixels[py][px] = Pixel(gr, a);
                    break;
                case RGB:
                    r = data[i];
                    g = data[i+1];
                    b = data[i+2];
                    pixels[py][px] = Pixel(r, g, b);
                    break;
                case RGBa:
                    r = data[i];
                    g = data[i+1];
                    b = data[i+2];
                    a = data[i+3];
                    pixels[py][px] = Pixel(r, g, b, a);
                    break;
            }
        }
    }
}


void Image::write(string filepath, channel_t componentsUsed) {
    // TODO writing here
}



/////////////////////////
// transformations
void Image::flipHorizontal() {
    for (int y = 0; y < this->y; y++) {
        PixelVector* r = &this->rowAt(y);
        r->reverse();
    }
}

void Image::flipVertical() {
    // very similar implementation to PixelVector::reverse()
    PixelVector* rev = new PixelVector [this->y];
    for (int y = 0; y < this->y; y++)
        rev[y] = this->constRowAt(this->y - y - 1);
    delete[] this->pixels;
    this->pixels = rev;
}

void Image::rotatePos90() {
    cout << "+90 deg" << endl;
    unsigned temp_y = this->x;
    PixelVector* temp = new PixelVector [temp_y];
    for (int i = 0; i < temp_y; i++) {
        temp[i] = this->columnAt(i);
        temp[i].reverse();
    }
    this->x = this->y;
    this->y = temp_y;
    delete[] this->pixels;
    this->pixels = temp;
}

void Image::rotateNeg90() {
    cout << "-90deg" << endl;
    unsigned temp_y = this->x;
    PixelVector* temp = new PixelVector [temp_y];
    for (int i = 0; i < temp_y; i++)
        temp[i] = this->columnAt(temp_y - i - 1);
    this->x = this->y;
    this->y = temp_y;
    delete[] this->pixels;
    this->pixels = temp;
}

void Image::addBorder(Pixel & borderPx, unsigned width) {
    cout << "adding border" << endl;
    unsigned new_y = width + this->y + width;
    unsigned new_x = width + this->x + width;
    PixelVector* temp = new PixelVector [new_y];
    for (int i = 0; i < new_y; i++) {
        for (int j = 0; j < new_x; j++) {
            if (i < width || j < width
            ||  i > (new_y - width - 1) || j > (new_x - width - 1)) {
                temp[i].pushBack(borderPx);
            } else {
                temp[i].pushBack(this->pixels[i - width][j - width]);
            }
        }
    }
    this->x = new_x;
    this->y = new_y;
    delete[] this->pixels;
    this->pixels = temp;
}

void Image::pointillize() {
    // TODO
}



/////////////////////////
// convenience
Pixel & Image::pixelAt(const unsigned X, const unsigned Y) {
    if (pixels)
        return this->pixels[Y].pixelAt(X);
}

PixelVector & Image::columnAt(unsigned X) {
    cout << "fetching col..." << endl;
    PixelVector* col = nullptr;
    if (X < this->x) {
        col = new PixelVector(this->y, this->componentsUsed);
        for (int y = 0; y < this->y; y++) {
            col->pushBack(this->pixels[y].operator[](X));
        }
    } else {
        // throw exception
        cerr << "Column out of bounds" << endl;
    }
    return *col;
}

PixelVector & Image::rowAt(unsigned Y) {
    if (Y < this->y) {
        return this->pixels[Y];
    } else {
        // throw exception
        cerr << "Row out of bounds" << endl;
        PixelVector* mt = nullptr;
        return *mt;
    }
}

PixelVector & Image::constRowAt(unsigned Y) const {
    if (Y < this->y) {
        return this->pixels[Y];
    } else {
        // throw exception
        cerr << "Row out of bounds" << endl;
        PixelVector* mt = nullptr;
        return *mt;
    }
}

/////////////////////////
// overloaded operators
ostream & operator<<(ostream& out, const Image& img) {
    out << endl;
    for(unsigned y = 0; y < img.getHeight(); y++) {
        out << img.pixels[y] << endl;
    }
    return out;
}


PixelVector & Image::operator[](unsigned Y) {
    if (Y < y)
        return pixels[Y];
}

Image::Image(const Image &that) {
    cout << "img copy ctor" << endl;
    this->x = that.x;
    this->y = that.y;
    this->componentsUsed = that.componentsUsed;
    this->inputFilepath = that.inputFilepath;
    this->outputFilepath = that.outputFilepath;
    this->pixels = new PixelVector [this->y];
    for (int y = 0; y < this->y; y++) {
        this->rowAt(y) = that.constRowAt(y);
    }
}