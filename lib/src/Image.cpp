#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"


/////////////////////////
// constructors & destructor

Image::~Image() {
    delete[] pixels;
}

// make a grey gradient image of specified dimensions
Image::Image(const unsigned X, const unsigned Y) {
    this->x = X;
    this->y = Y;
    this->componentsUsed = GR;
    this->pixels = new PixelVector [Y];
    Pixel p = Pixel(100);
    Pixel* pp = nullptr;

    for (unsigned y = 0; y < this->y; y++) {
        Pixel pn = Pixel(p.getR() + 5*y);
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
// read constructor
Image::Image(string filename, const unsigned X, const unsigned Y, const channel_t encodedComponents, const channel_t desiredComponents) {
    int width = int(X);
    int height = int(Y);
    int encoded = int(encodedComponents);
    int n = int(desiredComponents);
    inputFilepath = "../sampleImages/" + filename;
    charArray data = stbi_load(inputFilepath.c_str(), &width, &height, &encoded, n);

    if (!data) {
        cerr << "Could not read image from file: ../sampleImages/" << inputFilepath << endl;
        this->pixels = nullptr;
    } else {
        this->x = X;
        this->y = Y;
        this->componentsUsed = desiredComponents;
        this->pixels = new PixelVector [Y];
        expandImage(data);
    }
    stbi_image_free(data);
}

void Image::write(string filename, format_t format) {
    outputFilepath = "../sampleImages/" + filename;
    int X = int(x);
    int Y = int(y);
    int N = int(componentsUsed);
    int stride = N * X;
    unsigned dataLength = X * Y * N;
    charArray data = new unsigned char [dataLength];
    flattenImage(data, dataLength);

    // convert from pixels[][] to data[]
    switch (format) {
        case PNG:
            stbi_write_png((outputFilepath + ".png").c_str(), X, Y, N, data, stride);
            break;
        case JPG:
            stbi_write_jpg((outputFilepath + ".jpg").c_str(), X, Y, N, data, 100);
            break;
        case BMP:
            stbi_write_bmp((outputFilepath + ".bmp").c_str(), X, Y, N, data);
            break;
    }

    delete[] data;
}



/////////////////////////
// transformations
void Image::flipHorizontal() {
    for (unsigned y = 0; y < this->y; y++) {
        PixelVector* r = &this->rowAt(y);
        r->reverse();
    }
}

void Image::flipVertical() {
    // very similar implementation to PixelVector::reverse()
    PixelVector* rev = new PixelVector [this->y];
    for (unsigned y = 0; y < this->y; y++)
        rev[y] = this->constRowAt(this->y - y - 1);
    delete[] this->pixels;
    this->pixels = rev;
}

void Image::rotatePos90() {
    unsigned temp_y = this->x;
    PixelVector* temp = new PixelVector [temp_y];
    for (unsigned i = 0; i < temp_y; i++) {
        temp[i] = this->columnAt(i);
        temp[i].reverse();
    }
    this->x = this->y;
    this->y = temp_y;
    delete[] this->pixels;
    this->pixels = temp;
}

void Image::rotateNeg90() {
    unsigned temp_y = this->x;
    PixelVector* temp = new PixelVector [temp_y];
    for (unsigned i = 0; i < temp_y; i++)
        temp[i] = this->columnAt(temp_y - i - 1);
    this->x = this->y;
    this->y = temp_y;
    delete[] this->pixels;
    this->pixels = temp;
}

void Image::addBorder(Pixel borderPx, unsigned width) {
    unsigned new_y = width + this->y + width;
    unsigned new_x = width + this->x + width;
    PixelVector* temp = new PixelVector [new_y];
    for (unsigned i = 0; i < new_y; i++) {
        for (unsigned j = 0; j < new_x; j++) {
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
    // calculate radius range and # dots based on image size
    unsigned int nDots = (x * y) / 12;
    unsigned int rMax  = min(int(min(x, y)) / 15, 99);
    unsigned int rMin  = max(int(rMax) / 2, 3);
    float intensityToR = (float(rMax) - float(rMin)) / 255;

    PixelVector* temp = new PixelVector [y];    // pointillism "canvas"
    for (unsigned y = 0; y < this->y; y++)
        temp[y] = this->rowAt(y);

    // randomly place dots, dot radius based on range & intensity
    for (int dot = 0; dot < nDots; dot++) {
        unsigned dotX = rand() % x;
        unsigned dotY = rand() % y;
        Pixel dotCopy = pixels[dotY][dotX];
        unsigned intensity = max(int(min(int(dotCopy.getIntensity()), 223)), 111);
        unsigned dotR = unsigned (intensityToR * intensity);
        for (int pointY = dotY - dotR; pointY <= dotY + dotR; pointY++) {
            for (int pointX = dotX - dotR; pointX <= dotX + dotR; pointX++) {
                float distance = sqrt(pow((float(pointX) - float(dotX)), 2) + pow((float(pointY) - float(dotY)), 2));
                if (pointX >= 0 && pointY >= 0 && pointX <= x - 1 && pointY <= y - 1
                && distance <= dotR) {
                    temp[pointY][pointX] = dotCopy;
                }
            }
        }
    }
    delete[] this->pixels;
    this->pixels = temp;
}



/////////////////////////
// convenience

PixelVector & Image::columnAt(unsigned X) {
    PixelVector* col = nullptr;
    if (X < this->x) {
        col = new PixelVector(this->y, this->componentsUsed);
        for (unsigned y = 0; y < this->y; y++) {
            col->pushBack(this->pixels[y].operator[](X));
        }
    } else {
        // throw exception
        cerr << "Column out of bounds" << endl;
    }
    return *col;
}

PixelVector & Image::rowAt(unsigned Y) {
    PixelVector* mt = nullptr;
    if (Y < this->y) {
        mt = &this->pixels[Y];
    } else {
        // throw exception
        cerr << "Row out of bounds" << endl;
    }
    return *mt;
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

void Image::expandImage(charArray data) {
    int n = int(componentsUsed);
    unsigned char gr, r, g, b, a {};
    Pixel p = Pixel();
    Pixel* pp = &p;
    // `i` indexes into `data`, increments by # of components
    for (unsigned i = 0; i < x * y * n; i += n) {
        unsigned py = i / (x * n);
        switch (this->componentsUsed) {
            case GR:
                gr = data[i];
                p = Pixel(gr);
                pixels[py].pushBack(*pp);
                break;
            case GRa:
                gr = data[i];
                a  = data[i+1];
                p = Pixel(gr, a);
                pixels[py].pushBack(*pp);
                break;
            case RGB:
                r = data[i];
                g = data[i+1];
                b = data[i+2];
                p = Pixel(r, g, b);
                pixels[py].pushBack(*pp);
                break;
            case RGBa:
                r = data[i];
                g = data[i+1];
                b = data[i+2];
                a = data[i+3];
                p = Pixel(r, g, b, a);
                pixels[py].pushBack(*pp);
                break;
        }
    }
}

void Image::flattenImage(charArray data, const unsigned dataLength) {
    int N = int(componentsUsed);
    for (unsigned i = 0; i < dataLength; i += N) {
        Pixel & currentPixel = pixels[i / (x * N)][(i / N) % x];
        switch (componentsUsed) {
            case GR:
                data[i] = currentPixel.getR();
                break;
            case GRa:
                data[i] = currentPixel.getR();
                data[i+1] = currentPixel.getA();
                break;
            case RGB:
                data[i] = currentPixel.getR();
                data[i+1] = currentPixel.getG();
                data[i+2] = currentPixel.getB();
                break;
            case RGBa:
                data[i] = currentPixel.getR();
                data[i+1] = currentPixel.getG();
                data[i+2] = currentPixel.getB();
                data[i+3] = currentPixel.getA();
                break;
        }
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

Image & Image::operator=(const Image & that) {
    if (this != &that) {
        delete[] this->pixels;
        this->x = that.x;
        this->y = that.y;
        this->componentsUsed = that.componentsUsed;
        this->inputFilepath = that.inputFilepath;
        this->outputFilepath = that.outputFilepath;
        this->pixels = new PixelVector [this->y];
        for (int y = 0; y < this->y; y++)
            this->pixels[y] = that.pixels[y];
    }
    return *this;
}

bool Image::operator!() {
    return this->pixels == nullptr;
}

Image::Image(const Image &that) {
    this->x = that.x;
    this->y = that.y;
    this->componentsUsed = that.componentsUsed;
    this->inputFilepath = that.inputFilepath;
    this->outputFilepath = that.outputFilepath;
    this->pixels = new PixelVector [this->y];
    for (unsigned y = 0; y < this->y; y++) {
        this->rowAt(y) = that.constRowAt(y);
    }
}