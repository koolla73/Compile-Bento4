#include "Ap4Decrypt.hpp"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        exit(-1);
    }

    const std::string kid = argv[1];
    const std::string key = argv[2];
    
    return 0;
}
