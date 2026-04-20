# GetPreviousMode - Execution Mode Detector for Windows
A lightweight C utility that detects the current CPU privilege level (Ring 0 vs Ring 3) by reading the Code Segment (CS) register's Current Privilege Level (CPL) bits.

    ⚠️ Note: This detects the current execution mode via CPU state. It does not replicate Windows' internal ExGetPreviousMode() kernel API, which reads thread state.

## 🔍 How It Works
The x86/x64 CS register contains 2 bits (bits 0–1) called the Current Privilege Level (CPL):

    0b00 (0) → Kernel Mode (Ring 0)
    0b11 (3) → User Mode (Ring 3)

This tool extracts those bits to determine execution context.

## 📋 Features

    ✅ Cross-compiler support: GCC, Clang, MSVC
    ✅ x86 and x64 architecture support
    ✅ Zero external dependencies
    ✅ Fallback to EXEC_USER_MODE for unsupported platforms (ARM64, MSVC x64)

### 🛠️ Build Instructions
GCC / MinGW

```powershell
gcc -o mode.exe mode.c
```

Clang (Windows)

```powershell
clang mode.c -o mode.exe
```

### ▶️ Usage
```c
Execution_Mode mode = GetPreviousMode();

if (mode == EXEC_KERNEL_MODE) {
    // Running in Ring 0 (kernel driver)
} else {
    // Running in Ring 3 (standard application)
}

```
## 🔐 Security Note
```
This code is read-only and does not modify system state.
Attempting to access kernel memory structures from user mode will cause an Access Violation.
Do not use this for privilege escalation or anti-cheat bypassing.
⚠️ MSVC x64 does not support inline assembly. The code safely returns EXEC_USER_MODE for user-mode apps
```
