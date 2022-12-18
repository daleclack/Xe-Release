// Defining the OS
enum class OS_Type{
    Linux,
    macOS,
    Windows
};

static inline OS_Type os_detector(){
#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    // #define COMPILED_IN_LINUX
    return OS_Type::Linux;
#elif defined(__APPLE__) && defined(__MACH__)
    // #define COMPILED_IN_MACOS
    return OS_Type::macOS;
#elif defined(_WIN32) || defined(_WIN64)
    // #define COMPILED_IN_WINDOWS
    return OS_Type::Windows;
#endif
}