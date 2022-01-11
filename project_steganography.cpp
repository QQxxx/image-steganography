#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>
#include <sys/stat.h>

void flag_i(std::string &path);

void flag_e(std::string &path, std::string &message);

void flag_d(std::string &path);

void flag_c(std::string &path, std::string &message);

void flag_h();

int validate_file_format(std::string &path);

std::vector<int> turn_message_into_byte_vector(const char *message);

int main(int argc, char **argv) {

    std::cout << std::endl <<
              "Author: Dominik Suszek" << std::endl
              << "Project: Image Steganography\n" << std::endl
              << "This program allows user to encrypt and decrypt secret messages on the images using steganography techniques."
              << std::endl <<
              "The following file formats are supported: .bpm and .ppm." << std::endl
              << std::endl <<
              "Please choose one of the following flags:"
              << std::endl <<
              "-i (--info)"
              << std::endl <<
              "-e (--encrypt)"
              << std::endl <<
              "-d (--decrypt)"
              << std::endl <<
              "-c (--check)"
              << std::endl <<
              "-h (--help)\n" << std::endl;

    std::string flag = argv[1];
    int adjustedFlag;

    if (flag == "-i" || flag == "--info") {
        adjustedFlag = 1;
    } else if (flag == "-e" || flag == "--encrypt") {
        adjustedFlag = 2;
    } else if (flag == "-d" || flag == "--decrypt") {
        adjustedFlag = 3;
    } else if (flag == "-c" || flag == "--check") {
        adjustedFlag = 4;
    } else if (flag == "-h" || flag == "--help") {
        adjustedFlag = 5;
    } else {
        adjustedFlag = 6;
    }

    switch (adjustedFlag) {
        case 1: {
            if (argc != 3) {
                std::cerr << "You have entered incorrect number of arguments." << std::endl;
            }
            std::string path = argv[2];
            flag_i(path);
            break;
        }
        case 2: {
            if (argc != 4) {
                std::cerr << "You have entered incorrect number of arguments." << std::endl;
            }
            std::string path = argv[2];
            std::string message = argv[3];
            flag_e(path, message);
            break;
        }

        case 3: {
            if (argc != 3) {
                std::cerr << "You have entered incorrect number of arguments." << std::endl;
            }
            std::string path = argv[2];
            flag_d(path);
            break;
        }

        case 4: {
            if (argc != 4) {
                std::cerr << "You have entered incorrect number of arguments." << std::endl;
            }
            std::string path = argv[2];
            std::string message = argv[3];
            flag_c(path, message);
            break;
        }
        case 5: {
            if (argc != 2) {
                std::cerr << "You have entered incorrect number of arguments." << std::endl;
            }
            flag_h();
            break;
        }
        default:
            std::cerr << "You have entered incorrect flag. Please use flag -h or --help to get more information."
                      << std::endl;
    }
}

/**
 * Prints the information about the image and extension (.bpm or .pmm) itself.
 * @param path A path to the image.
 */
void flag_i(std::string &path) {

    const char *cpath = path.c_str();
    struct stat attr;
    stat(cpath, &attr);

    std::fstream file(path, std::ios::in | std::ios::binary);

    if (validate_file_format(path) == 0) {
        std::cerr << "This file format is not supported or you've made a typo in filename." << std::endl;
    }

    if (!file.is_open()) {
        std::cerr << "File could not be opened." << std::endl;
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char *>(header), 54);

    if (validate_file_format(path) == 1) { // .bmp format
        std::cout
                << "The .bmp file format (also known as bitmap) is capable of storing two-dimensional digital images both monochrome and color, in various color depths, and optionally with data compression, alpha channels, and color profiles."
                << std::endl <<
                "The details about the image: "
                << std::endl <<
                "- resolution: " << header[18] + (header[19] << 8) + (header[20] << 16) + (header[21] << 24) << " x "
                << header[22] + (header[23] << 8) + (header[24] << 16) + (header[25] << 24)
                << std::endl <<
                "- size: " << header[2] + (header[3] << 8) + (header[4] << 16) + (header[5] << 24) << " bytes"
                << std::endl <<
                "- last modified: " << ctime(&attr.st_mtime) << std::endl;


    } else if (validate_file_format(path) == 2) { // .ppm format
        std::cout
                << "The .ppm file format is a 24-bit color image formatted using a text format. It stores each pixel with a number from 0 to 65536, which specified the color of this pixel."
                <<
                std::endl << "The details about the image: " << std::endl <<
                "- resolution: " << header[18] + (header[19] << 8) + (header[20] << 16) + (header[21] << 24) << " x "
                << header[22] + (header[23] << 8) + (header[24] << 16) + (header[25] << 24) << std::endl <<
                "- size: " << header[2] + (header[3] << 8) + (header[4] << 16) + (header[5] << 24) << " bytes"
                << std::endl <<
                "- last modified: " << ctime(&attr.st_mtime) << std::endl;
    }
}

/**
 * Checks the format of the file: this program supports .bmp and .ppm extensions.
 * @param path A path to the image.
 * @return int value based on the file format.
 */
int validate_file_format(std::string &path) {
    std::ifstream image(path);
    unsigned char beginning[2];
    image.read((char *) beginning, sizeof(beginning));

    if (((beginning[0] == 0x42) && (beginning[1] == 0x4D))) {
        return 1;
    } else if (int(beginning[0]) == 80 && int(beginning[1]) == 54) {
        return 2;
    } else {
        return 0;
    }
}

/**
 * Prints user manual with detailed description of functionalities of this program.
 */
void flag_h() {
    std::cout << "Detailed user manual:" << std::endl
              << "This program allows user to encrypt and decrypt secret messages on the images using steganography."
              << std::endl <<
              "The following files extensions are supported: .bmp .ppn."
              << std::endl <<
              "Please enter the flag with parameter(s) (if necessary), with accordance to the following rules:\n"
              << std::endl <<
              "Flag -i (--info) checks if the file provided is in the supported format. If yes, more information about the file and the format itself will be printed. Arguments required:"
              << std::endl <<
              "1. filepath\n"
              << std::endl <<
              "Flag -e (--encrypt) opens the file and encrypts secret message. Arguments required: " << std::endl <<
              "1. filepath" << std::endl <<
              "2. message.\n"
              << std::endl <<
              "Flag -d (--decrypt) decrypts secret message. Arguments required:" << std::endl <<
              "1. filepath\n"
              << std::endl <<
              "Flag -c (--check) verifies if there is a possibility that a message was hidden. Arguments required:"
              << std::endl <<
              "1. filepath" << std::endl <<
              "2. message which could have been encrypted\n"
              << std::endl <<
              "Flag -h (--help) shows detailed user manual. No arguments required.\n" << std::endl;
}

/**
 * Turns a C-style string (an array of characters) into a byte vector.
 * @param message A pointer to the array of characters.
 * @return The vector storing a byte representation of the message.
 */
std::vector<int> turn_message_into_byte_vector(const char *message) {
    std::vector<int> messageInBytes;

    for (int i = 0; i < strlen(message); ++i) {
        int ASCII = int(unsigned(message[i]));
        int index = 7;

        while (index >= 0) {
            int value = int(pow(2, index));
            if (ASCII - value >= 0) {
                messageInBytes.push_back(1);
                ASCII -= value;
            } else
                messageInBytes.push_back(0);
            index--;
        }
    }
    return messageInBytes;
}

/**
 * Encrypts secret message in image.
 * @param path A path to the image.
 */
void flag_e(std::string &path, std::string &message) {

    // add BoF at the beginning and EoF at the end of the message
    message = message.insert(0, "BoF");
    message = message.append("EoF");

    // check if the file format is supported
    if (validate_file_format(path) == 0) {
        std::cerr << "This file format is not supported or you've made a typo in the filename." << std::endl;
        return;
    }

    std::fstream image(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!image.is_open()) {
        std::cerr << "File could not be opened." << std::endl;
        return;
    }

    std::vector<int> pixelsToBeModified;
    int offset;

    if (validate_file_format(path) == 1) { // .bmp file
        std::fstream headerStream(path, std::ios::in | std::ios::out | std::ios::binary);
        unsigned char headerBMP[14];
        headerStream.read(reinterpret_cast<char *>(headerBMP),14); // BMP file has 54-byte long header, but we need only 14 bytes to calculate the offset
        headerStream.close();
        // in BMP format exact offset is included in header (starting address, where the bitmap image data can be found)
        // offset is stored as 4-byte long value, so we need to shift the bits
        offset = headerBMP[10] + (headerBMP[11] << 8) + (headerBMP[12] << 16) + (headerBMP[13] << 24);
    } else {
        offset = 200; // in case of .ppm format, some arbitrarily chosen value
    }

    const char *message_to_hide = message.c_str();
    std::vector<int> messageInBytes = turn_message_into_byte_vector(message_to_hide);

    int size = (strlen(message_to_hide) + 6) * 8;
    char *buffer = new char[size];
    int position = offset;
    image.seekp(offset); // set the beginning of a stream to position i (in order to omit the problem with overwriting information from header)
    image.read(buffer, size);

    for (int i = 0; i < size; ++i) {
        pixelsToBeModified.push_back(int(buffer[i]));
    }

    for (int j = 0; j < messageInBytes.size(); ++j) {
        if ((messageInBytes[j] == 1 && pixelsToBeModified[j] % 2 == 0) ||
            (messageInBytes[j] == 0 && pixelsToBeModified[j] % 2 == 1)) {
            image.seekp(position);
            image << char(pixelsToBeModified[j] + 1);
            position++;
        } else {
            position++;
        }
    }
    delete[] buffer; // free up memory
    image.close();
    std::cout << "Message successfully encrypted." << std::endl;
}

/**
 * Decrypts secret message hidden in the image.
 * @param path A path to the image.
 */
void flag_d(std::string &path) {
    int offset;
    std::fstream image(path, std::ios::in | std::ios::out | std::ios::binary);

    if (validate_file_format(path) == 0) {
        std::cerr << "This file format is not supported or you've made a typo in filename." << std::endl;
        return;
    }

    if (!image.is_open()) {
        std::cerr << "File could not be opened." << std::endl;
        return;
    }

    if (validate_file_format(path) == 1) {
        unsigned char headerBMP[14];
        image.read(reinterpret_cast<char *>(headerBMP), 14);
        // BMP file has 54-byte long header and the information about offset is stored starting from 10th byte
        // in BMP format exact offset is included in header (starting address, where the bitmap image data can be found)
        // offset is stored as 4-byte long value, so we need to shift the bits
        offset = headerBMP[10] + (headerBMP[11] << 8) + (headerBMP[12] << 16) + (headerBMP[13] << 24);
    } else {
        offset = 200; // in case of .ppm format, some arbitrarily chosen value
    }

    std::vector<int> beginningOfsecretMessage;
    image.seekp(offset);

    char beginningOfTheFile[24];
    image.read(beginningOfTheFile, sizeof(beginningOfTheFile));
    int position = offset + sizeof(beginningOfTheFile);

    for (char c: beginningOfTheFile) {
        if (int(c) % 2 == 0) {
            beginningOfsecretMessage.push_back(0);
        } else {
            beginningOfsecretMessage.push_back(1);
        }
    }

    if (beginningOfsecretMessage == turn_message_into_byte_vector("BoF")) {
        std::cout << "File successfully decrypted. This is the secret message: ";
    } else {
        std::cout << "Decryption process failed." << std::endl << "Please use -e flag in order to hide a message.\n";
        return;
    }

    std::string secretMessage;
    while (secretMessage.find("EoF") == std::string::npos) {
        std::vector<int> toBeAdjusted;
        image.seekp(position);
        char buffer[8];
        image.read(buffer, sizeof(buffer));

        for (char c: buffer) {
            if (int(c) % 2 == 0)
                toBeAdjusted.push_back(0);
            else
                toBeAdjusted.push_back(1);
        }
        int index = 7;
        int result = 0;

        for (int j = 0; j < 8; ++j) {
            if (toBeAdjusted[j] == 1)
                result += int(pow(2, index));
            index--;
        }
        char character = char(result);
        secretMessage += character;
        position += 8;
    }

    image.close();
    std::string finalMessage = secretMessage.substr(0, secretMessage.size() - 3);
    std::cout << "\"" << finalMessage << "\"";
}

/**
 * Check if the image is big enough to store the secret message.
 * @param path A path to the image.
 * @param message A message which could be hidden in the image.
**/
void flag_c(std::string &path, std::string &message) {

    std::fstream image(path, std::ios::in | std::ios::out | std::ios::binary);

    if (validate_file_format(path) == 0) {
        std::cerr << "This file format is not supported or you've made a typo in filename." << std::endl;
        return;
    }

    if (!image.is_open()) {
        std::cerr << "File could not be opened." << std::endl;
        return;
    }

    int availableSpace;
    int requiredSize;

    if (validate_file_format(path) == 1) {
        unsigned char headerBMP[14];
        image.read(reinterpret_cast<char *>(headerBMP), 14); // BMP file has 54-byte long header, but only 14 bytes are required
        // in BMP format exact header size is included in header
        int fileSize = headerBMP[2] + (headerBMP[3] << 8) + (headerBMP[4] << 16) + (headerBMP[5] << 24);
        int offset = headerBMP[10] + (headerBMP[11] << 8) + (headerBMP[12] << 16) + (headerBMP[13] << 24);

        availableSpace = fileSize - offset;
        requiredSize = message.size();
    } else {
        unsigned char headerPPM[10];
        image.read(reinterpret_cast<char *> (headerPPM), 10);
        int width = headerPPM[2] + (headerPPM[3] << 8) + (headerPPM[4] << 16);
        int height = headerPPM[5] + (headerPPM[6] << 8) + (headerPPM[5] << 16);
        availableSpace = (width * height) - 200; // filesize - offset
    }

    if (availableSpace > requiredSize) {
        std::cout << "The message \"" << message << "\" could be encrypted in this file. There is enough space."
                  << std::endl;
    } else {
        std::cout << "The message \"" << message
                  << "\" couldn't be encrypted in this file due to insufficient filesize." << std::endl;
    }
    image.close();
}

