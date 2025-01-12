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

    if (!getline(stream, line))
    {
        throw std::invalid_argument("Invalid command format: No arguments length found");
    }

    if (line.empty() || line[0] != '*')
    {
        throw std::invalid_argument("Invalid command format: Expected '*' at the start");
    }
    int n = std::stoi(line.substr(1)); // Remove '*' 
    getline(stream, line);

    for (int i = 0; i <n; i++)
    {
        int argLength = std::stoi(line.substr(1)); // remove the '$'
        std::getline(stream, line);
        decodedCommand.push_back(line.substr(0, argLength));
        std::getline(stream, line);//go to new line
    }

    // for(int i=0;i<n;i++) {
    //     std::cout<<decodedCommand[i]<<std::endl;
    // }

    return decodedCommand;
}

// int main() {
//     Decoder d;
//     d.decodeCommand("");
// }