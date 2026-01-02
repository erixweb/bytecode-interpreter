#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <variant>
#include <vector>
#include <unordered_map>
#include "../instructions.h"

static const std::unordered_map<std::string, int> OP_CODES = {
    {"PUSH_I32", PUSH_I32}, {"PUSH_STR", PUSH_STR}, {"ADD", ADD}, {"PRINT", PRINT}, {"CONCAT", CONCAT}, {"SUB", SUB}, {"MUL", MUL}, {"DIV", DIV}, {"DEFINE", DEFINE}, {"LOAD_DEF", LOAD_DEF}};

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

int getTokenID(const std::string &token)
{
    auto it = OP_CODES.find(token);
    return (it != OP_CODES.end()) ? it->second : -1; // Not an instruction
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

    try
    {
        size_t idx = 0;
        int v = std::stoi(token, &idx);
        if (idx == token.size())
        {
            return v;
        }
    }
    catch (...)
    {
    }

    if (getTokenID(token) != -1)
    {
        return getTokenID(token);
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
