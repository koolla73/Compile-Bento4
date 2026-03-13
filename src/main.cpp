#include <fstream>

#include "Ap4Decrypt.hpp"

int main(int argc, char *argv[]) {

    if (argc != 5) {
        exit(-1);
    }

    const std::string kid = argv[1];
    const std::string key = argv[2];
    const std::string initFilepath = argv[3];
    const std::string segmentFilepath = argv[4];

    if (kid.size() != 32 || key.size() != 32) {
        exit(-2);
    }

    std::ifstream initFile(initFilepath, std::ifstream::binary);
    if (!initFile.is_open()) {
        exit(-3);
    }
    std::ifstream segmentFile(segmentFilepath, std::ifstream::binary);
    if (!segmentFile.is_open()) {
        exit(-4);
    }

    std::filebuf* initbuf = initFile.rdbuf();
    std::size_t initsize = initbuf->pubseekoff (0, initFile.end, initFile.in);
    initbuf->pubseekpos(0, initFile.in);
    uint8_t* initbuffer = new uint8_t[initsize];
    initbuf->sgetn(reinterpret_cast<char*>(initbuffer), initsize);

    std::filebuf* segmentbuf = segmentFile.rdbuf();
    std::size_t segmentsize = segmentbuf->pubseekoff (0, segmentFile.end, segmentFile.in);
    segmentbuf->pubseekpos(0, segmentFile.in);
    uint8_t* segmentbuffer = new uint8_t[segmentsize];
    segmentbuf->sgetn(reinterpret_cast<char*>(segmentbuffer), segmentsize);

    initFile.close();
    segmentFile.close();

    // Ap4Decrypt::decryptAndFragment();

    delete[] initbuffer;
    delete[] segmentbuffer;

    // std::ofstream outFile("out.mp4", std::ios::out | std::ios::binary);
    // outFile.write(buffer, size);
    // outFile.close();
    
    return 0;
}
