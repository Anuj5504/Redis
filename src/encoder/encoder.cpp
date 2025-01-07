#include "resp_encoder.h"
#include <iostream>
#include <string>

std::vector<std::string> Encoder::convertIntoArray(const std::string &message)
{
    std::vector<std::string> res;
    std::string s;
    int n = message.size();
    for (int i = 0; i < n; i++)
    {
        if (message[i] == ' ')
        {
            if (!s.empty())
            {
                res.push_back(s);
            }
            s = "";
        }
        else
        {
            s += message[i];
        }
    }
    if (!s.empty())
    {
        res.push_back(s);
    }

    return res;
}

std::string Encoder::encodeSimpleString(const std::string &message)
{
    return "+" + message + "\r\n";
}

std::string Encoder::encodeError(const std::string &message)
{
    return "-" + message + "\r\n";
}

std::string Encoder::encodeInteger(const int value)
{
    return ":" + std::to_string(value) + "\r\n";
}

std::string Encoder::encodeBulkString(const std::string &message)
{
    return formatBulkString(message);
}

std::string Encoder::encodeArray(const std::vector<std::string> &elements)
{
    std::string result = "*" + std::to_string(elements.size()) + "\r\n";
    for (const auto &element : elements)
    {
        result += formatBulkString(element);
    }
    return result;
}

std::string Encoder::formatBulkString(const std::string &message)
{
    return "$" + std::to_string(message.length()) + "\r\n" + message + "\r\n";
}