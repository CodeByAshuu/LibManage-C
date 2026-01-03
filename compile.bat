@echo off
if not exist build mkdir build
if not exist data mkdir data

echo Compiling...
gcc src/main.c src/utils.c src/file_io.c src/auth.c src/books.c src/transactions.c -I include -o build/libsys.exe

if %errorlevel% neq 0 (
    echo Compilation Failed!
    pause
    exit /b %errorlevel%
)

echo Compilation Success! Executable in build/libsys.exe
echo.
pause
