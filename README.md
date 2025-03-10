# CPU Design in C++

### Assembly.txt
This is the input file written in RISC-V format and supports the following instructions from the RV32I Base Instruction Set:-

$$ add,\space addi,\space sub,\space and,\space andi,\space or,\space ori,\space xor,\space xori,\space sll,\space slli,\space lui,\space lw,\space sw,\space beq,\space bne,\space blt,\space bge,\space jal,\space jalr, \space \space \text{Labels and Comments.}$$

The given Assembly.txt file is an example format as an example input file.

**Note:** It is necessary that the Assembly.txt file is written in Windows(notepad), such that all lines are terminating with '\r\n'. Any other form of line endings will result in wrong outputs.

### Assembly to Machine Language
Assembly to Machine Language.cpp converts the Assembly.txt into Binary (with spacings as per instruction formatting in RISC-V) and Hexadecimal formats along with corresponding Line numbers from the input file. In case of any syntax errors in input file it outputs the cause of error along with the line numbers.

Output is then redirected to the file Machine_language.txt. Hexadecimal is given as an output because many online converters use Hexadecimal output formats, thus it is easier to compare the accuracy of the given code.

### Converter
Converter.cpp filters the Machine_language.txt file to contain only the binary part and remove line numbers, spaces, hexadecimals, headers and errors in input format(if any). The output is then redirected to Binary.txt.

### CPU Design
CPU_Design.cpp finally converts the Binary.txt (32-bit binary instructions) into their corresponding instructions. Each time a register value is updated or a memory location is being accessed, the final changed values are printed onto the terminal.

5-stage pipelinining is used in this case, along with stall and operator forwarding for handling hazards.

### output.sh
This is a bash script which sequentially runs the .cpp files in order and prints the final output onto the screen.
