# Oblig 6

## Requirements
- CMake 3.10+
- A C++20 compiler (GCC/Clang on Linux, MinGW or MSVC on Windows)
- vcpkg (https://github.com/microsoft/vcpkg)
- ninja (recommended) or make

## Setup

1. Clone vcpkg into this project folder as submodule:
```bash
   git submodule add  https://github.com/microsoft/vcpkg
   ./vcpkg/bootstrap-vcpkg.sh      # Linux/macOS
   .\vcpkg\bootstrap-vcpkg.bat     # Windows
```

2. Configure and build:

   **Linux:**
```bash
   cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build build
```

   **Windows (MinGW):**
```bash
   cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
   cmake --build build
```

3. Run:
  Linux:
``` bash
   ./build/linux/task1
   ./build/linux/task2
```

  Windows:
```bash
    .\build\windows\task1.exe
    .\build\windows\task2.exe
    ```

4. Open browser at localhost:8081/ and localhost:8081/en_side 
```
```
