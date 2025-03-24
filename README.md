# IIC3413-DB

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

## Lab 1 testing:

To create an example database:
```bash
./build/Debug/bin/create_db
```
that will create the database folder `data/test_example`.

You can change the schema or add more tables modifying the file `src/bin/create_db.cc`

Then you can query the database with `print_table`, passing as first parameter an existing database folder, and as a second parameter the table name:
```bash
./build/Debug/bin/print_table <db_folder> <table_name>
```

Example:
```bash
./build/Debug/bin/print_table data/test_example t1
```
