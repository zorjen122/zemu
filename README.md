# zemu

这是一个简约且无依赖的 RISC-V64-IFMC 模拟解释器，兼容 C++11 及更高版本

- Language-en [English](README.md)

### 设置

在使用该模拟器之前，请确保您拥有一个支持程序输入参数的 RISC-V64 编译器，例如 `riscv64-unknown-elf-g++`。

### 构建步骤

1. 克隆存储库：
   ```
   git clone https://github.com/zorjen122/zemu.git
   ```
2. 进入 zemu 目录：
   ```
   cd zemu
   ```
3. 构建模拟器：
   ```
   make
   ```

### 用法

要运行模拟器，只需将编译好的 RISC-V64 可执行文件作为参数传递，例如：

```
./rvemu a.out
```

或者，您可以使用 CMake 进行编译。例如，您可以使用 clang++ 编译文件 `./src/rvemu.cpp`，并使用 `-std=c++11` 选项保存为 `zvemu`。您还可以添加选项 `-Wall -Werror` 以获取更严格的警告：

```
clang++ --std=c++11 ./src/rvemu.cpp -o zvemu -Wall -Werror
```

示例：使用 RISC-V64 编译器编译演示代码：

```
riscv64-unknown-elf-g++ --std=c++11 ./demo.cpp -o demo -Wall -Werror
```

然后，使用您编译好的ELF文件作为参数运行该模拟器：

```
./zvemu demo
```

预期输出：

```
hello zemu
```

### 检查

- 静态分析（xanalyzer）：
  ```
  clang++ --analyze --verbose -Xanalyzer -analyzer-output=text ./src/rvemu.cpp
  ```
- 动态分析（地址检测）：
  ```
  clang++ --std=c++11 -O3 -Wall -Werror -fsanitize=address
  ```

### TODO

- 通过在生成的 C++ 汇编代码中编译支持优化条件的指令，实现即时优化（JIT）。
- 添加对 RISC-V32 的支持，以支持将模拟器移植到多个系统。
- 增加模拟器窗口并扩展功能。
