#include <iostream>
#include "srvemu.h"
#include "syscall.h"

int main(int argc, char* argv[]) {
    assert(argc > 1);

    machine_t machine{};
    Machine call;

    call.machine_load_program(&machine, argv[1]);
    call.machine_setup(&machine, argc, argv);

    while (1) {

        exit_reason_t reason = call.machine_step_program(&machine);

        assert(reason == ecall);
        u64 syscall = machine_get_gp_reg(&machine, a7);
        u64 ret = do_syscall(&machine, syscall);        // a7 calling return value.
        machine_set_gp_reg(&machine, a0, ret);
    }


    return 0;
}
