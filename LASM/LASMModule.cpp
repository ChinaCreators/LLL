#include "LASMModule.h"
#include <dlfcn.h>

class DllCloser
{
public:
	~DllCloser()
	{
		for (auto i : m_Content)
			dlclose(i);
	}

	void AddDll(void* dl)
	{
		m_Content.emplace_back(dl);
	}

	static DllCloser& Get()
	{
		static DllCloser instance;
		return instance;
	}

private:
	DllCloser()
	{
	}

private:
	std::vector<void*> m_Content;
};

std::string GetDllName(const std::string& module_name)
{
	//todo : support more os
	return "lib" + module_name + ".dylib";
}

std::map<std::string, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)> LASM::GetFunctionsFromModule(const std::string& module_name)
{
	//todo add more os, now just unix
	auto dl = dlopen(GetDllName(module_name).c_str(), RTLD_NOW | RTLD_GLOBAL);
	std::map<const char*, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)> re;
	void (*func)(std::map<const char*, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)>&);
	func = ((decltype(func))dlsym(dl, "RegisterModule"));
	func(re);
	std::map<std::string, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)> re2;
	for (auto& i : re)
	{
		re2.emplace(std::make_pair(module_name + ":" + std::string(i.first), i.second));	//convert to module_name:function_name
	}
	return re2;
}