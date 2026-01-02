#include <vector>
#include <iostream>
#include <variant>
#include <unordered_map>
#include "./utils/check_type.hpp"
#include "./api/operations.hpp"
#include "./parser/parser.hpp"
#include "./instructions.h"

int main()
{

    std::vector<std::variant<int, std::string>> stack;
    std::string fileContents = loadFile("program.byte");
    if (fileContents.empty()) {
        return 1;
    }


    std::vector<std::variant<int, std::string>> instructions = fileToVector(fileContents);

    std::unordered_map<std::string, std::variant<int, std::string>> variables;

    for (int i = 0; i < instructions.size(); i++)
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
            case DEFINE:
                if (i + 1 >= instructions.size() || !checkStr(instructions[i + 1]))
                {
                    std::cerr << "Error: DEFINE instruction not followed by a string (variable name)." << std::endl;
                    return 1;
                }
                if (stack.empty())
                {
                    std::cerr << "Error: DEFINE instruction requires a value on the stack." << std::endl;
                    return 1;
                }

                { 
                    std::string varName = std::get<std::string>(instructions[++i]); 
                    std::variant<int, std::string> varValue = stack.back();
                    stack.pop_back();

                    variables[varName] = varValue;
                }
                break;
            case LOAD_DEF:
                /*
                    PUSH_STR, "Erik",
                    DEFINE, "name",
                    LOAD_DEF, "name",
                    PRINT,
                */
               // push the variable's value to the stack
               if (i + 1 < instructions.size() && checkStr(instructions[i + 1]))
               {
                   std::string varName = std::get<std::string>(instructions[++i]);
                   if (variables.count(varName)) {
                       stack.push_back(variables[varName]);
                   } else {
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
            default:
                break;
            }
        }
    }

    return 0;
}