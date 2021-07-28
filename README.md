# SCTE35Parser
## Introduction

SCTE35Parser is a SCTE-35 textual data parser written in C++ language. It supports parsing of all the splice informations available in SCTE-35 ISBE broadcasting standard. It supports parsing of the below mentioned textual data types :

1. Hexa-decimal SCTE-35 cue data
2. Base-64 SCTE-35 cue data
3. Binary SCTE-35 cue data

This project also includes an example in main.cpp file which parses the provided hexa-decimal SCTE-35 cue data and prints on the console in readable format. This example covers common use cases which are mentioned below :

1. Time_Signal - Placement Opportunity Start
2. Splice_Insert
3. Time_Signal - Placement Opportunity End
4. Time_Signal - Program Start/End
5. Time_Signal - Program Overlap Start
6. Time_Signal - Program Blackout Override / Program End
7. Time_Signal - Program End
8. Time_Signal - Program Start/End - Placement Opportunity End

It can also be used to parse custom SCTE-35 cue data encoded in any of the above mentioned encoding formats.

Beyond this "README.md" file, API level documentation can be generated via Doxygen (see "Documentation" section below).

## Version

Initial version 1.0.0.0

## License

License as provided with the repository.


## Compilation

All the steps can be done on Linux and Mac OS systems as well with minor differences through supported toolchains and packages on those platforms. First make new directories "build" and "doc" or delete all the contents of directories "build" and "doc" before performing below steps in source root directory.

On Windows System (using MinGW) if proper cmake environment path is setup use the below mentioned commands from source root directory :

    mkdir build
    cmake -DCMAKE_BUILD_TYPE:STRING=Release -Bbuild -G "MinGW Makefiles"
    cmake --build build --config Release --target all -j 10 --


## Documentation

On Windows System (using MinGW) if proper doxygen environment path is setup then to get the source code documentation do below commands from source root directory :

    mkdir doc
    doxygen SCTE35DoxyFile

Go inside "doc" directory this should have two directories "html" and "latex".

To generate PDF documentation from latex file (using MinGW) do below commands from source root directory :

    cd doc
    cd latex
    make


## Run

To run the console application on Windows System do below commands from source root directory :

    cd build
    SCTE35Parser
