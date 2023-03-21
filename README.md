# Summary 
This repo contains the "testable" implementation of our current approach of producing probable provenance graph from the source code. There are two parts of the approach. One is done offline while the other one is done on the fly based on the input source code. Both approaches rely on static code analysis based on clang-llvm compiler toolchain.

### Online part:

This part works on the source code of the program of which we are trying to generate the provenance graph. We execute the following actions sequentially during online analysis of our source code.

- Get the input source code.
- Generate the LLVM-IR of the code. 
- Run the pass on the generated IR.
- Get the pruned data-dependency-graph (PDDG) of the code.
- Get the augmented control flow graph (ACFG) of the code.
- Extract the execution paths from the ACFG
- Parse each function in the ACFG sequentially based on the following 3 criteria:
    - Is it a user-defined function or a libc function?
    - If the above condition is true, then does this function ends in a syscall or calls another function that ends in a syscall?
    - If the above condition is true, we refer to a table called relevant table, which will be discussed in the later section. 
    - Based on the information in the relevant table, we deduce which parameter of the syscall points to the object argument of the syscall. subject is the process which initiated the syscall.
    - Afterwards, based on the information in the PDDG, we deduce if this object is an already existing one or a new object. 
    - If it is an already existing object, we draw a new edge between the already existing object and the subject and label the edge with the current syscall.
    - Else if it is a new object we insert the new object in the graph, draw an edge from the subject with the current syscall as label.
    - If any of the above condition fail at any given point, we jump to the first step and start processing the next function until we reach the end of the execution path
- Return the Generated Probable Provenance Graph.

### Offline Part:
The goal of this part is to generate the relevant functions table we mentioned above. This part works on the standard C library (libc from here on). Our implementation of this part is based on musl-libc (Link: https://musl.libc.org/ ). It is a lightweight, standard-compliant libc implementation that is not dependent on the linux-headers. Additionally, you can build musl-libc from source using clang-llvm compiler toolchain which is critical for our implementation strategy. 

- Clone the musl-libc repo
- Generate the LLVM-IR of the musl-libc source code on file-by-file basis. Assume there are N number of source files. You should generate N corresponding IR files. Each Ir file usually correspond to the intermediate representation of a specific function. There are a few excpetions of this. However, we are not currently taking that into account.
- Run the Analysis pass on each IR file and determine the functions that have syscall endpoints. They are ususally inline assembly functions with syscall assembly command or special utility functions such as ```syscall_ret``` or ```syscall_cp```. Please refer to the code for the implementation details. 
- Let's call our relevant function set <bold>S</bold>
- Assume that after running the analysis pass, we have N<sub>1</sub> number of functions who have syscall endpoints. So, S = {N<sub>1</sub>}
- Among the remaining (N-N<sub>1</sub>) functions, We see how many of them have endpoints in N<sub>1</sub>. We call this new set N<sub>2</sub>. Then S will be, S = {N<sub>1</sub>,N<sub>2</sub>}
- We keep continuing this until we can't find any relevant functions anymore. 
- At the end of the process, the S will be the set of relevant functions.

# Current Implementation Status

## Source Code Analysis (Online Phase):

**Programming Structure** | **Implementation Status** | **Exception Case**
:-----:|:-----:|:-----:
Single Statment| Complete| None
If-Else| Complete| None
For Loop| Complete| None
While Loop| Complete| None
Do-While Loop| Complete| None
Switch Statement| Not Started Yet| N/A 
Pointer| Out of scope| N/A

## Implemented Syscall List (Offline Phase, Eventually should reach parity with SPADE's syscall list)
**Syscall**| **Syscall Code**|**Relevant Value**|**SPADE Edge Type**
:-----:|:-----:|:-----:|:-----:
open| 2| Returned Value| Used or WasGeneratedBy
read| 0| Argument 0| Used
write| 1| Argument 0| WasGeneratedBy and WasDerivedFrom
close| 3| Argument 0| Used or WasGeneratedBy



# Getting ready to use the repo

## Install Dependencies

- Install JsonCpp:  ``` sudo apt install libjsoncpp-dev ```
- Install Boost Library: ``` sudo apt install libboost-all-dev ```
- Install Clang and LLVM 14: ``` sudo apt install clang lldb lld ```
  - Please note that the current system is tested on Ubuntu 22.04 with clang and llvm version 14.0.x
  - After installing clang 14 using the above command, you should try: ```clang --version``` and it should produce the following output (Match your system accordingly): 
    ``` 
        Ubuntu clang version 14.0.0-1ubuntu1  
        Target:x86_64-pc-linux-gnu  
        Thread model: posix  
        InstalledDir: /usr/bin
    ```
  - Running ```llvm-config --version``` should return ```14.0.0```
- Install graphviz (To add it to system path): ``` sudo apt insall graphviz```
- Install pygraphviz (To visualize the output graphs in python): ```pip install graphviz```


## Building the pass: Use the build.sh script
- Alternatively, follow the steps below.
- Clone the repository using: ```git clone <repo-url>```
- Navigate to the repository root directory
- Execute the following commands. (Please note that your system should have Cmake Version 3.1 or higher to build the project.)

``` 
    mkdir build
    cd build
    cmake ..
    make
```

# Running the Analysis

### Option 1: 
- Navigate to the Test Directory
- Modify Existing Test Files or create your own test file.
- Return to the root directory and run: ```./run.sh <filename>```

### Option 2 (Outdated) :

- Navigate to the directory of the source code you want to analyze. Lets call it ```test.c```
- Run this command: ```clang -S -emit-llvm -o test.ll test.c``` 
- This will create a ```test.ll``` file in the same directory.
- Next, run the following command: ```opt -load <project root-dir>/build/musl-llvm-pass/libBlockExtractPass.so -basic-block-extract <your-test-source-dir>/test.ll -enable-new-pm=0 > /dev/null```
- This would generate the PDDG and ACFG of the graph as well as write it in two separate files named: ```ddgedges.txt``` and ```prov_edges.txt```. The first contains the edges of the Data Dependency Graph while the second one contains the provenance edges derived from analyzing the augmented control flow graph.
- Run this command ```python3 ddg_generator.py``` to visualize the ddg graph and the provenance graph. Remember to run this in the folder where you have your ```ddgedges.txt``` and ```prov_edges.txt```. For convenience this python script is currently in the test folder.


# Building the Relevant Function Table from musl-libc source
- [TODO]


