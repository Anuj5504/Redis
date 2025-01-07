#include "resp_decoder.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> Decoder::decodeCommand(const std::string &command)
{
    std::vector<std::string> decodedCommand;
    std::istringstream stream(command);
    std::string line;

    if (!std::getline(stream, line))
    {
        throw std::invalid_argument("Invalid command format: No arguments length found");
    }

    if (line.empty() || line[0] != '*')
    {
        throw std::invalid_argument("Invalid command format: Expected '*' at the start");
    }

    int n = std::stoi(line.substr(1)); // Remove '*' character

    getline(stream, line);

    for (int i = 0; i < n; i++)
    {
        int argLength = std::stoi(line.substr(1)); // remove the '$' character and get the argument length
        std::getline(stream, line);
        std::cout<<line<<std::endl;
        decodedCommand.push_back(line.substr(0, argLength));
    }

    return decodedCommand;
}
