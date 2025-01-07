#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <vector>

class Encoder {
public:
    //convert into array
    std::vector<std::string>convertIntoArray(const std::string& message);

    // Encode a simple string
    std::string encodeSimpleString(const std::string& message);

    // Encode an error message  
    std::string encodeError(const std::string& errorMessage);

    // Encode an integer
    std::string encodeInteger(int value);

    // Encode a bulk string
    std::string encodeBulkString(const std::string& message);

    // Encode an array of strings
    std::string encodeArray(const std::vector<std::string>& elements);

private:
    // Helper function to format a bulk string
    std::string formatBulkString(const std::string& message);
};

#endif // ENCODER_H
