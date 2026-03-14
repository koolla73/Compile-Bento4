#include <cstring>
#include <fstream>
#include <vector>

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
    const size_t initsize = initbuf->pubseekoff(0, initFile.end, initFile.in);
    initbuf->pubseekpos(0, initFile.in);

    std::filebuf* segmentbuf = segmentFile.rdbuf();
    const size_t segmentsize = segmentbuf->pubseekoff(0, segmentFile.end, segmentFile.in);
    segmentbuf->pubseekpos(0, segmentFile.in);

    uint8_t* finalBuffer = (uint8_t*)malloc(initsize + segmentsize);
    
    initbuf->sgetn(reinterpret_cast<char*>(finalBuffer), initsize);
    segmentbuf->sgetn(reinterpret_cast<char*>(finalBuffer + initsize), segmentsize);

    initFile.close();
    segmentFile.close();

    const size_t finalSize = initsize + segmentsize;
    //const size_t finalSize = AP4_Decrypt::decrypt(finalBuffer, initsize + segmentsize, kid.c_str(), key.c_str());
    //const size_t finalSize = AP4_Decrypt::decryptAndFragment(finalBuffer, combinedBuffer.size(), kid.c_str(), key.c_str());

    if (finalSize == 0) {
        exit(-5);
    }

    std::ofstream outFile("out.mp4", std::ios::out | std::ios::binary);
    outFile.write(reinterpret_cast<char*>(finalBuffer), finalSize);
    outFile.close();

    free(finalBuffer);
    
    return 0;
}
