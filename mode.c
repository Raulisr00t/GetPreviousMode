#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    EXEC_USER_MODE = 3,
    EXEC_KERNEL_MODE = 0
}Execution_Mode;

Execution_Mode GetPreviousMode(void){

#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
    uint64_t cs;

#if defined(_MSC_VER)
    // MSVC x64 does NOT support inline assembly.
    // In a standard user-mode app, this is always User Mode.
    return EXEC_USER_MODE;

#else
    // GCC and Clang compilers
    __asm__ volatile("mov %%cs, %0" : "=r"(cs));
    return (cs & 3) == 0 ? EXEC_KERNEL_MODE : EXEC_USER_MODE;

#endif

#elif defined(_M_IX86) || defined(__i386__)
    uint32_t cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(cs));
    return (cs & 3) == 0 ? EXEC_KERNEL_MODE : EXEC_USER_MODE;

#else
    // ARM64 / Other: Requires EL0/EL1 check (OS-dependent)
    return EXEC_USER_MODE;
#endif
}

int main(){
    Execution_Mode mode = GetPreviousMode();

    printf("Current Execution Mode: %s\n", 
           mode == EXEC_KERNEL_MODE ? "Kernel Mode (Ring 0)" 
                                    : "User Mode (Ring 3)");

    return 0;
}
