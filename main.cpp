#include "Image.h"
#include <cstring>
using std::cout, std::cin;

// helpers for controller
int findCommand(string command) {
    const int NUM_COMMANDS = 10;
    string commands[NUM_COMMANDS] = {"read", "write", "vflip", "hflip", "+90", "-90",
                                     "border", "pointillism","exit", "help"};

    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (commands[i] == command) {
            return i;
        }
    }
    return -1;
}

channel_t componentsFromString(string comp) {
    if (comp == "GR")
        return GR;
    if (comp == "GRa")
        return GRa;
    if (comp == "RGB")
        return RGB;
    if (comp == "RGBa")
        return RGBa;
    return RGB;
}

format_t formatFromString(string fmt) {
    if (fmt == "PNG")
        return PNG;
    if (fmt == "JPG" || fmt == "JPEG")
        return JPG;
    if (fmt == "BMP")
        return BMP;
    return PNG;
}

bool imageExists(Image &img) {
    if (!img) {
        cout << "No loaded image. Please use the \"read\" command to load an image." << endl;
        return false;
    }
    return true;
}


// controller / driver
// supports terminal
int main(int argc, char* argv[]) {
    string flag = "-c";
    bool running = argc >= 2 && strcmp(flag.c_str(), argv[1]) == 0;
    if (!running) {
        cout << "Run with \"-c\" flag to run controller. Bye bye!" << endl;
        return 0;
    }

    // case variables
    Image img = Image();
    string inFilename {};
    string outFilename {};
    unsigned width {};
    unsigned height {};
    string compEnc {};
    string compDes {};
    string fmtStr {};
    format_t fmt {};
    unsigned borderR {};
    unsigned borderG {};
    unsigned borderB {};
    unsigned borderA {};
    unsigned borderWidth {};

    cout << "Welcome to Image Processor" << endl;
    cout << "Use the help command to display all commands" << endl;
    while (running) {
        string command {};
        cout << "Enter a command: ";
        cin >> command;

        switch (findCommand(command)) {
            case 0:
                cout << "Enter the name of the file to read from (files must be in /sampleImages/): ";
                cin >> inFilename;
                std::cin.ignore(10, '\n'); 
                cout << "Enter the width of the image: ";
                cin >> width;
                std::cin.ignore(10, '\n'); 
                cout << "Enter the height of the image: ";
                cin >> height;
                std::cin.ignore(10, '\n'); 
                cout << "How is this file encoded? (GR = gray, GRa = gray + alpha, RGB = color, RGBa = color + alpha): ";
                cin >> compEnc;
                std::cin.ignore(10, '\n'); 
                cout << "How will we encode the file? (GR = gray, GRa = gray + alpha, RGB = color, RGBa = color + alpha): ";
                cin >> compDes;
                std::cin.ignore(10, '\n'); 
                img = Image(inFilename, width, height, componentsFromString(compEnc), componentsFromString(compDes));
                cout << "Image loaded from " << inFilename << " (" << width << "x" << height << ") as a " << compDes << " image" << endl;
                break;
            case 1:
                if (!imageExists(img))
                    break;

                cout << "Enter the name of the file to write to, excluding the file extension (files will be in /sampleImages/): ";
                cin >> outFilename;
                std::cin.ignore(10, '\n'); 
                cout << "Enter the desired format of the file (PNG, JPG/JPEG, BMP): ";
                cin >> fmtStr;
                std::cin.ignore(10, '\n'); 
                fmt = formatFromString(fmtStr);

                img.write(outFilename, fmt);
                cout << "Image will be written to " << outFilename << " as a " << fmtStr << " file, after program termination" << endl;
                break;
            case 2:
                if (!imageExists(img))
                    break;

                img.flipVertical();
                cout << "Image flipped on vertical axis" << endl;
                break;
            case 3:
                if (!imageExists(img))
                    break;

                img.flipHorizontal();
                cout << "Image flipped on horizontal axis" << endl;
                break;
            case 4:
                if (!imageExists(img))
                    break;

                img.rotatePos90();
                cout << "Image rotated by +90 degrees" << endl;
                break;
            case 5:
                if (!imageExists(img))
                    break;

                img.rotateNeg90();
                cout << "Image rotated by -90 degrees" << endl;
                break;
            case 6:
                if (!imageExists(img))
                    break;

                cout << "Enter border R: ";
                cin >> borderR;
                std::cin.ignore(10, '\n'); 
                cout << "Enter border G: ";
                cin >> borderG;
                std::cin.ignore(10, '\n'); 
                cout << "Enter border B: ";
                cin >> borderB;
                std::cin.ignore(10, '\n'); 
                cout << "Enter border A: ";
                cin >> borderA;
                std::cin.ignore(10, '\n'); 
                cout << "Enter border width: ";
                cin >> borderWidth;
                std::cin.ignore(10, '\n');

                img.addBorder(Pixel(borderR % 256, borderG % 256, borderB % 256, borderA % 256), borderWidth);
                cout << "Border added to image" << endl;
                break;
            case 7:
                if (!imageExists(img))
                    break;

                img.pointillize();
                cout << "Pointillism filter applied to image" << endl;
                break;
            case 8:
                cout << "Exiting Image Processor..." << endl;
                running = false;
                break;
            case 9:
                cout << "COMMAND LIST:" << endl;
                cout << "read        - load an image from a file" << endl
                     << "write       - write an image to a file" << endl
                     << "vflip       - flip an image vertically" << endl
                     << "hflip       - flip an image horizontally" << endl
                     << "+90         - rotate an image +90deg" << endl
                     << "-90         - rotate an image -90deg" << endl
                     << "border      - add a border to an image" << endl
                     << "pointillism - apply a pointillism filter to an image" << endl
                     << "exit        - exit the image processor" << endl
                     << "help        - display this help message" << endl;
                break;
            default:
                cout << "\"" << command << "\": command not found" << endl;
                break;
        }
    }
}