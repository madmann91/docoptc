# doctoptc

A [docopt](https://github.com/docopt) compiler written in C that generates C code to parse command line options.

## Building

    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=<Debug|Release>
    cmake --build .
    
## Running

Simply run the compiler with an input text file to generate C code:

    docoptc file.txt
