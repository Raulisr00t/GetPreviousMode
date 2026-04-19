#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    EXEC_USER_MODE   = 0,  // ARM64: EL0 | x86/x64: Ring 3
    EXEC_KERNEL_MODE = 1   // ARM64: EL1+ | x86/x64: Ring 0
} Execution_Mode;

Execution_Mode GetPreviousMode(void) {

#if defined(_M_ARM64) || defined(__aarch64__) || defined(__arm64__)
    // ARM64: Read CurrentEL (Exception Level) register
    // Bits [3:2] contain the EL value:
    //   0b00 = EL0 (User), 0b01 = EL1 (Kernel), 0b10 = EL2 (Hyp), 0b11 = EL3 (Secure)
    uint64_t current_el;

#if defined(_MSC_VER)
    // MSVC ARM64: No inline assembly support.
    // Use external assembly (see arm64_mode.asm) or fallback.
    // For safety in user-mode apps, default to USER_MODE.
    return EXEC_USER_MODE;
    
#else
    // GCC / Clang ARM64: Inline assembly supported
    __asm__ volatile("mrs %0, CurrentEL" : "=r"(current_el));
    uint64_t el = (current_el >> 2) & 0x3;
    return (el == 0) ? EXEC_USER_MODE : EXEC_KERNEL_MODE;
#endif

#elif defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
    uint64_t cs;

#if defined(_MSC_VER)
    // MSVC x64: No inline assembly → safe fallback for user-mode
    return EXEC_USER_MODE;
    
#else
    // GCC / Clang x64
    __asm__ volatile("mov %%cs, %0" : "=r"(cs));
    return ((cs & 3) == 0) ? EXEC_KERNEL_MODE : EXEC_USER_MODE;
#endif

#elif defined(_M_IX86) || defined(__i386__)
    uint32_t cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(cs));
    return ((cs & 3) == 0) ? EXEC_KERNEL_MODE : EXEC_USER_MODE;

#else
    // Fallback for unknown architectures
    return EXEC_USER_MODE;
#endif
}

int main(void) {
    Execution_Mode mode = GetPreviousMode();
    
    printf("Current Execution Mode: %s\n",
           mode == EXEC_KERNEL_MODE ? "Kernel Mode" : "User Mode");
    
    return 0;
}
