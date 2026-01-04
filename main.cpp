#include <vector>
#include <iostream>
#include <variant>
#include <unordered_map>
#include "./utils/check_type.hpp"
#include "./api/operations.hpp"
#include "./parser/parser.hpp"
#include "./instructions.h"

int main(int argc, char *argv[])
{
    std::string fileName = argv[1];
    std::string fileContents = loadFile(fileName);

    if (fileContents.empty())
    {
        return 1;
    }

    std::vector<std::variant<int, std::string>> stack;

    std::vector<std::variant<int, std::string>> instructions = fileToVector(fileContents);

    std::unordered_map<std::string, std::variant<int, std::string>> variables;
    int i = 0;
    while (i < instructions.size())
    {
        if (checkInt(instructions[i]))
        {
            switch (std::get<int>(instructions[i]))
            {
            case PUSH_I32:
                if (i + 1 < instructions.size() && checkInt(instructions[i + 1]))
                {
                    stack.push_back(std::get<int>(instructions[++i]));
                }
                else
                {
                    std::cerr << "Error: PUSH instruction not followed by an integer." << std::endl;
                    return 1;
                }
                break;
            case PUSH_STR:
                if (i + 1 < instructions.size() && checkStr(instructions[i + 1]))
                {
                    stack.push_back(std::get<std::string>(instructions[++i]));
                }
                else
                {
                    std::cerr << "Error: PUSH instruction not followed by a string." << std::endl;
                    return 1;
                }
                break;
            case CONCAT:
                if (stack.size() < 2)
                {
                    std::cerr << "Error: CONCAT instruction requires at least two strings on the stack." << std::endl;
                    return 1;
                }

                if (checkStr(stack[stack.size() - 2]) && checkStr(stack[stack.size() - 1]))
                {
                    std::string a = std::get<std::string>(stack[stack.size() - 2]);
                    std::string b = std::get<std::string>(stack[stack.size() - 1]);

                    stack[stack.size() - 2] = a + b;
                }
                else
                {
                    std::cerr << "Error: CONCAT instruction expects string operands." << std::endl;
                    return 1;
                }
                stack.pop_back();
                break;
            case EQUAL:
                if (stack.size() < 2)
                {
                    std::cerr << "Error: EQUAL instruction requires at least two values on the stack." << std::endl;
                    return 1;
                }

                if (checkInt(stack[stack.size() - 2]) && checkInt(stack[stack.size() - 1]))
                {
                    int a = std::get<int>(stack[stack.size() - 2]);
                    int b = std::get<int>(stack[stack.size() - 1]);

                    stack[stack.size() - 2] = a == b;
                }
                else if (
                    checkStr(stack[stack.size() - 2]) &&
                    checkStr(stack[stack.size() - 1]))
                {
                    std::string a = std::get<std::string>(stack[stack.size() - 2]);
                    std::string b = std::get<std::string>(stack[stack.size() - 1]);

                    stack[stack.size() - 2] = a == b;
                }
                else
                {
                    std::cerr << "Error: EQUAL instruction expects string or integer operands." << std::endl;
                    return 1;
                }
                stack.pop_back();
                break;
            case JZ:
            {
                int offset = std::get<int>(instructions[i + 1]);
                int condition = std::get<int>(stack.back());
                stack.pop_back(); // ¡Importante! Limpiar la pila

                if (condition == 0)
                {
                    i = offset;
                    continue; // Usamos continue para saltar el i++ del final del bucle
                }

                i += 1; // Si no saltamos, debemos saltar el argumento (offset)
                break;
            }

            case JMP:
            {
                int offset = std::get<int>(instructions[i + 1]);
                i = offset;
                continue; // Para que no se incremente i++ después de saltar
            }

            case JNZ:
            {
                int offset = std::get<int>(instructions[i + 1]);
                int condition = std::get<int>(stack.back());
                stack.pop_back(); // ¡Importante!

                if (condition != 0)
                {
                    i = offset;
                    continue;
                }
                i += 1; // Saltamos el argumento
                break;
            }
            case IS_GT:
            {
                if (stack.size() < 2)
                {
                    std::cerr << "IS_GT expects two values" << std::endl;
                    return 1;
                }
                // Get values first
                std::variant<int, std::string> valB = stack.back();
                stack.pop_back();
                std::variant<int, std::string> valA = stack.back();
                stack.pop_back();

                if (checkInt(valA) && checkInt(valB))
                {
                    stack.push_back(std::get<int>(valA) > std::get<int>(valB) ? 1 : 0);
                }
                else
                {
                    std::cerr << "Error: IS_GT expects integers." << std::endl;
                    return 1;
                }
                break;
            }
            case STORE:
                if (i + 1 >= instructions.size() || !checkStr(instructions[i + 1]))
                {
                    std::cerr << "Error: STORE instruction not followed by a string (variable name)." << std::endl;
                    return 1;
                }
                if (stack.empty())
                {
                    std::cerr << "Error: STORE instruction requires a value on the stack." << std::endl;
                    std::cerr << "ERR AT I: " << i << std::endl;
                    return 1;
                }
                else
                {
                    std::string varName = std::get<std::string>(instructions[++i]);
                    std::variant<int, std::string> varValue = stack.back();
                    variables[varName] = varValue;

                    if (checkInt(varValue))
                    {
                        std::cout << std::get<int>(varValue) << std::endl;
                    }

                    stack.pop_back();
                }
                break;
            case GET:
                if (i + 1 < instructions.size() && checkStr(instructions[i + 1]))
                {
                    std::string varName = std::get<std::string>(instructions[++i]);
                    if (variables.count(varName))
                    {
                        stack.push_back(variables[varName]);
                        break;
                    }
                    else
                    {
                        std::cerr << "Error: Variable '" << varName << "' not found." << std::endl;
                        return 1;
                    }
                }
                else
                {
                    std::cerr << "Error: LOAD_DEF instruction not followed by a string (variable name)." << std::endl;
                    return 1;
                }
                break;
            case ADD:
                add(stack);
                break;
            case SUB:
                sub(stack);
                break;
            case MUL:
                mul(stack);
                break;
            case DIV:
                div(stack);
                break;
            case PRINT:
                if (!stack.empty())
                {
                    if (checkInt(stack.back()))
                    {
                        std::cout << std::get<int>(stack.back()) << std::endl;
                    }
                    else if (checkStr(stack.back()))
                    {
                        std::cout << std::get<std::string>(stack.back()) << std::endl;
                    }
                }
                break;
            case EXIT:
                return 0;
            default:
                break;
            }
        }
        i++;
    }

    return 0;
}