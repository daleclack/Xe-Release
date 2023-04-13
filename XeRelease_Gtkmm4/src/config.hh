// This header file is to get the platform and features enable/disable
#pragma once

// Defining the OS
enum class OS_Type{
    Linux,
    Darwin,
    Windows
};

static inline OS_Type get_os_type(){
#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    // #define COMPILED_IN_LINUX
    return OS_Type::Linux;
#elif defined(__APPLE__) && defined(__MACH__)
    // #define COMPILED_IN_Darwin
    return OS_Type::Darwin;
#elif defined(_WIN32) || defined(_WIN64)
    // #define COMPILED_IN_WINDOWS
    return OS_Type::Windows;
#endif
}

// Enable/Disable longterm build
// #define DISABLE_LONGTERM_BUILD
