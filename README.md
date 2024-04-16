# Overview of the Analyser

Read file in with the following format: [Stock],[Interval],[Volume Traded],[High],[Low]

Calculate the total volume traded per Stock

Calculate the total volume traded per Stock&Interval

Write the total volume traded per Stock&Interval as a percentage of the total volume traded per Stock to stdout: [Stock],[Interval],[%Volume Traded]

Write the delimiter '#' to stdout

Write the maximum High and minimum Low for each Stock to stdout: [Stock],[Day High],[Day Low]

example given input:
```
VOD.L 1 100 184 183.7
BT.LN 1 300 449.4 448.2
VOD.L 2 25 184.1 182.4
BT.LN 2 900 449.8 449.5
```

example given output:
```
VOD.L,1,80
BT.LN,1,25
VOD.L,2,20
BT.LN,2,75
#
VOD.L,184.1,182.4
BT.LN,449.8,448.2
```

## Overall File Structure

```
├── CMakeLists.txt                  # CMake configuration file
├── README.md                       # This README.md file
├── datafiles                       # folder containing input data in .txt format
│   ├── input.txt
│   └── testfile.txt
├── main.cpp                        # main executable
├── analyser.cpp                    # file with class and functions of the analyser
└── analyser_tests.cpp              # unit tests using GoogleTest framework
```

## Requirements
- **C++ Compiler**: The project requires a C++ compiler. The Makefile is configured to use `g++` as the default compiler.

- **C++ Standard**: The project code is written in C++17

- **CMake**: The project is built using a Cmake of version `3.22.1`. Please install according to your operating system. For Linux OS, install by running the following in the terminal:
    ```
    sudo apt-get install cmake
    ```

- **GoogleTest** Unit tests are run using the GoogleTest framework. In the `CmakeLists.txt` file, it is configured that GoogleTest will be downloaded from GitHub, as from: https://google.github.io/googletest/quickstart-cmake.html

## Tested Environment

Application tested and run in: `Ubuntu 22.04.1 LTS`

## Build Instructions
To build and run the main program or run test:
```bash
mkdir build
cmake -S . -B build
cmake --build build
```

## Execution and Results

#### With the given input
execution of the main file in the `new` directory
```bash
./build/analyser datafiles/input.txt
```
Output:
```
VOD.L,1,80
BT.LN,1,25
VOD.L,2,20
BT.LN,2,75
#
BT.LN,449.8,448.2
VOD.L,184.1,182.4
```

#### Extra custom input
execution of the custom test file in the `new` directory
```bash
./build/analyser datafiles/testfile.txt
```

Input data:
```
AAPL 2 307 170.6 160.7
TSLA 1 1013 750.1 721.9
MSFT 2 2517 260.7 255.0
GOOG 2 1509 1305.9 1290.5
NVDA 1 812 200.0 195.7
VOD.L 1 105 184.2 183.6
VOD.L 2 29 184.4 182.8
GOOG 1 1223 1205.0 1102.4
AAPL 3 203 165.3 160.8
MSFT 1 1817 251.6 231.9
```
Output
```
AAPL,2,60.1961
TSLA,1,100
MSFT,2,58.0757
GOOG,2,55.2343
NVDA,1,100
VOD.L,1,78.3582
VOD.L,2,21.6418
GOOG,1,44.7657
AAPL,3,39.8039
MSFT,1,41.9243
#
NVDA,200,195.7
GOOG,1305.9,1102.4
MSFT,260.7,231.9
TSLA,750.1,721.9
VOD.L,184.4,182.8
AAPL,170.6,160.7
```


execution of all the tests in the `new` directory using GoogleTest
```bash
cd build && ctest
```