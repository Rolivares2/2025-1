# IIC3413-DB

## Lab 2 Testing
There is one example test at `src/bin/test_lab2.cc`. After compiling the project you can run it
passing a parameter `t`. For example if you want `t` = 10000:
```bash
build/Debug/bin/test_lab2 10000
```

Be careful on running this on a too high number, as the size on disk increases with the parameter.

The aim of this test is not to be the only test you use, but to provide an example on how you can test your solution.

## Project Build

Install Dependencies:
--------------------------------------------------------------------------------
MillenniumDB needs the following dependencies:
- GCC >= 8.1
- CMake >= 3.12

On recent Debian and Ubuntu based distributions they can be installed by running:
```bash
sudo apt update && sudo apt install g++ cmake
```

For Mac you need to install the **Xcode Command Line Tools** and [Homebrew](https://brew.sh/). After installing homebrew you can install cmake:
```bash
brew install cmake
```

Build the Project:
--------------------------------------------------------------------------------
Open a terminal in the project root directory, then execute the commands:

For the Release version:
```bash
cmake -Bbuild/Release -DCMAKE_BUILD_TYPE=Release && cmake --build build/Release/
```

For the Debug Version:
```bash
cmake -Bbuild/Debug -DCMAKE_BUILD_TYPE=Debug && cmake --build build/Debug/
```

To use multiple cores during compilation (much faster) add `-j n` at the end of the previous commands, replacing `n` with the desired number of threads. Example for 8 threads:
```bash
cmake -Bbuild/Debug -DCMAKE_BUILD_TYPE=Debug && cmake --build build/Debug/ -j 8
```
