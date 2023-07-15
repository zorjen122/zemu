#include <iostream>
#include "../src/srvemu.h"
#include "../src/syscall.h"

int main(int argc, char* argv[]) {
    assert(argc > 1);
    
    Machine call;
    machine_t machine{};
    
    std::cout << "argv[1] file path is " << argv[1] << std::endl;
    std::cout << "pc is " << machine.state.pc << std::endl;

    call.machine_load_program(&machine,argv[1]);

    std::cout << "mmu entry is 0x" << std::hex << machine.mmu.entry << std::endl;
    std::cout << "mmu host entry is 0x" << std::hex << TO_HOST(machine.mmu.entry) << "\n\n";

    std::cout << "page max is 0x" << std::hex << getpagesize() << std::endl;
    std::cout << "mmu host_alloc is 0x" << std::hex << machine.mmu.host_alloc << std::endl;
    std::cout << "mmu alloc is 0x" << std::hex << machine.mmu.alloc << std::endl;
    std::cout << "mmu base is 0x" << std::hex << machine.mmu.base << std::endl;

    std::cout << "\nhello,world\n";   
    
    int count = 0;
    
    call.machine_setup(&machine, argc, argv);

    while (1) {
        
        exit_reason_t reason = call.machine_step_program(&machine);
        
        ++count;
        std::cout <<"count("<<count<<")\n\tpc is"<< std::hex 
            << machine.state.pc << std::endl;

        assert(reason == ecall);
        u64 syscall = machine_get_gp_reg(&machine, a7);
        std::cout << "\ta7 addr ret of func called is 0x" << std::hex
            <<syscall << "\n";
        u64 ret = do_syscall(&machine, syscall);        // a7 calling return value.
        std::cout<<"\ta7 addr ret value of func called is 0x"<<std::hex
            <<ret<<"\n\n";
        machine_set_gp_reg(&machine, a0, ret);

    }


    return 0;
}
