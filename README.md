# MIPS-Simulator
COL216 - Computer Architecture

## A1
This folder has a software simulator in C++ for the MIPS processor and some valid testcases.

- The input to the program is a MIPS program (limited to the instructions like ```add```, ```sub```,```beq```,```j```,```jal```, etc.). Read this input into an array of instructions.
- Simulation starts with the Program Counter (PC) pointing to the first instruction.
- Execute the instruction. Display the values of the Register File and Memory contents as a result of executing the instruction.
- Proceed to the next instruction and continue the execution.
- Simulation stops when there is no further instruction to execute.

## A2 
This folder has a software simulator in C++ for the MIPS processor, Makefile and some valid testcases. \
This is built over the previous version of simulator and I have introduced a 5 stage pipelining structure for the MIPS Processor. \
The stages in the pipeline are: 
- ```IF``` - Instruction Fetch
- ```ID``` - Instruction Decode
- ```EX``` - Arthmetic Calculations and memory address calculation
- ```MEM``` - Memory Access
- ```WB``` - Write Back
The processor is stalled whenever any type of hazard is detected. \

## A3
This folder has a software simulator in C++ for the MIPS processor, Makefile and some valid testcases. \
This is built over the previous version of simulator and the data hazards in the pipeline are handled by introducing pipeline registers. 

## A4
This folder has a software simulator in C++ for the MIPS processor, Makefile and some valid testcases. \
This is built over the previous version of simulator.\
Sometimes, operations such as floating point arithmetic or memory accesses in a pipelined processor require variable delays. A floating point computation may depend on the data operands. A memory access may exhibit variable delays due to the cache hierarchy.\
In this I have modelled the memory architecture by introducing a data memory that works as follows:
- The operation completes in one cycle with a probability x (this is considered a HIT).
- The operation requires N cycles with a probability 1-x (this is considered a MISS).
- Upon a memory request, at end of one cycle, the memory indicates whether the operation was a HIT or MISS. If HIT, the operation is complete. If not, the operation completes N-1 cycles later.
