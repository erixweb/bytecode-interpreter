#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <variant>
#include <vector>

std::string trim(std::string s)
{
    auto is_space = [](unsigned char c)
    { return std::isspace(c); };

    while (!s.empty() && is_space(s.front()))
        s.erase(s.begin());
    while (!s.empty() && is_space(s.back()))
        s.pop_back();
    return s;
}

std::string loadFile(std::string fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file '" << fileName << "'." << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
std::variant<int, std::string> parse_token(std::string token)
{
    token = trim(token);

    if (token.size() >= 2 && token.front() == '"' && token.back() == '"')
    {
        return token.substr(1, token.size() - 2);
    }

    // Intentar número
    try
    {
        size_t idx = 0;
        int v = std::stoi(token, &idx);
        if (idx == token.size())
        { // todo el token es número
            return v;
        }
    }
    catch (...)
    {
        // caerá abajo y se tratará como string
    }

    if (token == "PUSH_I32")
    {
        return 1;
    }
    else if (token == "PUSH_STR")
    {
        return 2;
    }
    else if (token == "ADD")
    {
        return 3;
    }
    else if (token == "PRINT")
    {
        return 4;
    }
    else if (token == "CONCAT")
    {
        return 5;
    }
    else if (token == "SUB")
    {
        return 6;
    }
    else if (token == "MUL")
    {
        return 7;
    }
    else if (token == "DIV")
    {
        return 8;
    }
    else if (token == "DEFINE")
    {
        return 9;
    }
    else if (token == "LOAD_DEF")
    {
        return 10;
    }
    return token;
}

std::vector<std::variant<int, std::string>> fileToVector(const std::string &line)
{
    std::vector<std::variant<int, std::string>> result;
    std::stringstream ss(line);
    std::string tok;

    while (std::getline(ss, tok, ','))
    {
        result.push_back(parse_token(tok));
    }
    return result;
}
