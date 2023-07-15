## Zero-Dependency RISC-V 64  Simulator

This is a concise, zero-dependency RISC-V 64 simulator that supports c++11 and above.

### Preparation

To use this simulator, make sure that the RISC-V64-related compilers support program input parameters, such as `riscv64-unknown-elf-g++`.

### Build Instructions

1. Clone the repository:

   ```
   git clone https://github.com/zorjen122/zemu.git
   ```

2. Navigate to the `zemu` directory:

   ```
   cd zemu
   ```

3. Build the simulator:

   ```
   make
   ```

### Usage

To run the simulator, pass the compiled RISC-V64 executable file as an argument, for example:

```
./rvemu a.out
```

Alternatively, you can use CMake for compilation. For example, you can compile `./src/rvemu.cpp` with clang++ using `-std=c++11`, and save it as `zvemu` with `-Wall -Werror` options:

```
clang++ --std=c++11 ./src/rvemu.cpp -o zvemu -Wall -Werror
clang++ --std=c++11 ./test/emuproc.cpp -o --std=c++20
```

Example: Compile the demo code using the RISC-V64 compiler:

```
riscv64-unknown-elf-g++ --std=c++11 ./demo.cpp -o demo -Wall -Werror
```

Then run the simulator with the compiled executable:

```
./zvemu demo
```

The expected output:

```
hello zemu
```

**Check**:

Static analysis  ( xanalyzer ):

```
clang++ --analyze --verbose -Xanalyzer -analyzer-output=text ./src/rvemu.cpp
```

Dynic analysis (  address sanitizer ):

```
clang++ --std=c++11 -O3 -Wall -Werror -fsanitize=address
```



### TODO

- Implement JIT optimization by compiling instructions that support optimization conditions after the C++ code generates assembly code.
- Add support for RISC-V32 to allow for porting the simulator to multiple systems.

