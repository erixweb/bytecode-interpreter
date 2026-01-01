@echo off

set "CMD_NAME=g++"

where %CMD_NAME% >nul 2>nul
if %errorlevel% neq 0 (
    powershell -Command "Write-Host '[X] The command %CMD_NAME% does not exist in your PATH.' -ForegroundColor Red"

    exit /b 1
)

powershell -Command "Write-Host '=> Compilando Bytecode Interpreter.' -ForegroundColor Green"

g++ .\main.cpp .\utils\check_type.cpp .\api\operations.cpp .\parser\parser.cpp -o bint
bint.exe