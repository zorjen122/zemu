# zemu

This is a minimalistic and **dependency-free** RISC-V64-IFMC simulator, compatible with c++11 and higher.

- zh_CN [简体中文](readme/README.zh_CN.md)

### Setup

------

Before using this simulator, please make sure you have a **RISC-V64** compiler that supports program input parameters, such as `riscv64-unknown-elf-g++`.

### Building Steps

------

1. Clone the repository:

   ```
   git clone https://github.com/zorjen122/zemu.git
   ```

2. Enter the `zemu` directory:

   ```
   cd zemu
   ```

3. Build the simulator:

   ```
   make
   ```

### Usage

------

To run the simulator, simply pass the compiled RISC-V64 executable file as an argument, for example:

```
./rvemu a.out
```

Alternatively, you can use CMake to compile it. For instance, you can compile the file `./src/rvemu.cpp` with `clang++`, using `-std=c++11` option and save it as `zvemu`. You can also add the options `-Wall -Werror` for stricter warnings:

```
clang++ --std=c++11 ./src/rvemu.cpp -o zvemu -Wall -Werror
```

Example: Compile the demo code using the RISC-V64 compiler:

```
riscv64-unknown-elf-g++ --std=c++11 ./demo.cpp -o demo -Wall -Werror
```

Then, run the simulator with your compiled executable:

```
./zvemu demo
```

Expected output:

```
hello zemu
```

### Checking

------

Static analysis (xanalyzer):

```
clang++ --analyze --verbose -Xanalyzer -analyzer-output=text ./src/rvemu.cpp
```

Dynamic analysis (address sanitizer):

```
clang++ --std=c++11 -O3 -Wall -Werror -fsanitize=address
```



### TODO

------

- Implement Just-in-Time (JIT) optimization by compiling instructions that support optimization conditions after generating assembly code in C++.
- Add support for RISC-V32 to enable porting the simulator to multiple systems.
- Incorporate a simulator window and extend functionalities.
