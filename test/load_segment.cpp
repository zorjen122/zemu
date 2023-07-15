#include <iostream>
#include "../src/srvemu.h"

int main(int argc, char* argv[]) {
    assert(argc > 1);
    Machine m;
    machine_t file_v{};
    file_v.state.exit_reason = none;

    std::cout << "argv[1] file path is " << argv[1] << std::endl;
    std::cout << "pc is " << file_v.state.pc << std::endl;

    m.machine_load_program(&file_v, argv[1]);

    std::cout << "mmu entry is 0x" << std::hex << file_v.mmu.entry << std::endl;
    std::cout << "mmu host entry is 0x" << std::hex << TO_HOST(file_v.mmu.entry) << "\n\n";

    std::cout << "page max is 0x" << std::hex << getpagesize() << std::endl;
    std::cout << "mmu host_alloc is 0x" << std::hex << file_v.mmu.host_alloc << std::endl;
    std::cout << "mmu alloc is 0x" << std::hex << file_v.mmu.alloc << std::endl;
    std::cout << "mmu base is 0x" << std::hex << file_v.mmu.base << std::endl;

    std::cout << "hello,world" << std::endl;

    return 0;
}
