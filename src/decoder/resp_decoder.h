#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>

class Decoder {
public:
    //decoding function
    std::vector<std::string> decodeCommand(const std::string& command);
};

#endif // DECODER_H
