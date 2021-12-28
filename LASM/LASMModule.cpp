#include "LASMModule.h"
#ifdef LLL_UNIX
#include <dlfcn.h>
#elif defined(LLL_WINDOWS)
#include <Windows.h>
#endif

#ifdef LLL_UNIX
#elif defined(LLL_WINDOWS)
#endif

class DllCloser
{
public:
	~DllCloser()
	{
		for (auto i : m_Content)
#ifdef LLL_UNIX
			dlclose(i);
#elif defined(LLL_WINDOWS)
			FreeLibrary(i);
#endif
	}

#ifdef LLL_UNIX
	void AddDll(void* dl)
#elif defined(LLL_WINDOWS)
	void AddDll(HMODULE dl)
#endif
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
#ifdef LLL_UNIX
	std::vector<void*> m_Content;
#elif defined(LLL_WINDOWS)
	std::vector<HMODULE> m_Content;
#endif
};

std::string GetDllName(const std::string& module_name)
{
#ifdef LLL_MACOS
	return "lib" + module_name + ".dylib";
#elif defined(LLL_LINUX)
	return "lib" + module_name + ".so";
#elif defined(LLL_WINDOWS)
	return module_name + ".dll";
#endif
}

std::map<std::string, uint64_t (*)(LLL::Byte*, uint64_t&, uint64_t&, uint64_t&)> LASM::GetFunctionsFromModule(const std::string& module_name)
{
#ifdef LLL_UNIX
	auto dl = dlopen(GetDllName(module_name).c_str(), RTLD_NOW | RTLD_GLOBAL);
#elif defined(LLL_WINDOWS)
	auto dl = LoadLibraryA(GetDllName(module_name).c_str());
#endif
	std::map<const char*, uint64_t (*)(LLL::Byte*, uint64_t&, uint64_t&, uint64_t&)> re;
	void (*func)(std::map<const char*, uint64_t (*)(LLL::Byte*, uint64_t&, uint64_t&, uint64_t&)>&);
#ifdef LLL_UNIX
	func = ((decltype(func))dlsym(dl, "RegisterModule"));
#elif defined(LLL_WINDOWS)
	func = ((decltype(func))GetProcAddress(dl, "RegisterModule"));
#endif
	func(re);
	std::map<std::string, uint64_t (*)(LLL::Byte*, uint64_t&, uint64_t&, uint64_t&)> re2;
	for (auto& i : re)
	{
		re2.emplace(std::make_pair(module_name + ":" + std::string(i.first), i.second));	//convert to module_name:function_name
	}
	return re2;
}