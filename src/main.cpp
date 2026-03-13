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
    std::size_t initsize = initbuf->pubseekoff(0, initFile.end, initFile.in);
    initbuf->pubseekpos(0, initFile.in);
    uint8_t* initbuffer = (uint8_t*)malloc(initsize);
    initbuf->sgetn(reinterpret_cast<char*>(initbuffer), initsize);

    std::filebuf* segmentbuf = segmentFile.rdbuf();
    std::size_t segmentsize = segmentbuf->pubseekoff(0, segmentFile.end, segmentFile.in);
    segmentbuf->pubseekpos(0, segmentFile.in);
    uint8_t* segmentbuffer = (uint8_t*)malloc(segmentsize);
    segmentbuf->sgetn(reinterpret_cast<char*>(segmentbuffer), segmentsize);

    initFile.close();
    segmentFile.close();

    std::vector<uint8_t> combinedBuffer(initbuffer, initbuffer + initsize);
    std::vector<uint8_t> segmentDataBuffer(segmentbuffer, segmentbuffer + segmentsize);
    combinedBuffer.insert(combinedBuffer.end(), segmentDataBuffer.begin(), segmentDataBuffer.end());

    free(initbuffer);
    free(segmentbuffer);

    uint8_t* finalBuffer = (uint8_t*)malloc(combinedBuffer.size());
    memcpy(finalBuffer, combinedBuffer.data(), combinedBuffer.size());

    const size_t finalSize = Ap4_Decrypt::decryptAndFragment(finalBuffer, combinedBuffer.size(), kid.c_str(), key.c_str());

    std::ofstream outFile("out.mp4", std::ios::out | std::ios::binary);
    outFile.write(finalBuffer, finalSize);
    outFile.close();

    free(finalBuffer);
    
    return 0;
}
