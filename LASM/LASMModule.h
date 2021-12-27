#pragma once
#include <functional>
#include <map>
#include <string>
#include "../LLL/LLL.h"

#ifdef _WIN64
#define LLL_WINDOWS
#define LLL_X64
#elif defined(_WIN32)
#define LLL_WINDOWS
#define LLL_X86
#endif

#if defined(__linux__) || defined(__linux)
#define LLL_LINUX
#define LLL_UNIX
#endif

#if defined(__APPLE__) || defined(__MACOSX__)
#define LLL_MACOS
#define LLL_UNIX
#endif

#ifdef LLL_UNIX
#ifdef __x86_64__
#define LLL_X64
#elif defined(__i386__)
#define LLL_X86
#endif
#endif

#ifdef _MSC_VER
#define LLL_MSVC 1
#define LLL_COMPILER LLL_MSVC
#elif defined(__clang__)
#define LLL_CLANG 2
#define LLL_COMPILER LLL_CLANG
#elif defined(__GNUC__)
#define LLL_GCC 3
#define LLL_COMPILER LLL_GCC
#endif

#ifdef LLL_MSVC
#if _MSVC_LANG > 201703L
#define LLL_CPP20
#endif
#else
#if __cplusplus > 201703L
#define LLL_CPP20
#endif
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define LLL_DEBUG
#endif

#if defined(LLL_UNIX) || defined(LLL_LINUX) || defined(LLL_MACOS)
#define LLL_POSIX
#endif

#if defined(LLL_WINDOWS) && defined(LLL_MSVC)
#define LLL_DLL_EXPORT __declspec(dllexport)
#else
#define LLL_DLL_EXPORT __attribute__((visibility("default")))
#endif

namespace LASM
{
	std::map<std::string, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)> GetFunctionsFromModule(const std::string& module_name);
}	 // namespace LASM

#define LASM_MODULE extern "C" LLL_DLL_EXPORT void RegisterModule(std::map<const char*, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)>& re)
#define LASM_MODULE_FUNCTION(name, func) re.insert(std::make_pair(name, func));