#pragma once
#include <functional>
#include <map>
#include <string>
#include "../LLL/LLL.h"

namespace LASM
{
	std::map<std::string, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)> GetFunctionsFromModule(const std::string& module_name);
}	 // namespace LASM

#define LASM_MODULE extern "C" void RegisterModule(std::map<const char*, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)>& re)
#define LASM_MODULE_FUNCTION(name, func) re.insert(std::make_pair(name, func));