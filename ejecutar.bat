@echo off
g++ -std=c++17 main.cpp -o main.exe
if errorlevel 1 (
    echo Hubo un error al compilar.
    pause
    exit /b
)
start "Proyecto Matematica Discreta" cmd /c "mode con: cols=100 lines=50 & main.exe & pause"