#pragma once
#include "../LLL/LLL.h"
#include <vector>
#include <string>
#include <map>
#include <cstdint>

namespace LASM
{
	class Compiler
	{
	public:
		void CompileFromString(const std::string& str);
		const std::vector<LLL::Command>& GetCommands() const;
		std::vector<std::string> GetExternalFunctionNames() const;

	private:
		std::vector<LLL::Command> m_Commands;
		std::map<std::string, uint64_t> m_ExternalFunctionNames;
	};

	class Runner
	{
	public:
		uint64_t RunCommands(const Compiler& c);

	private:
		std::map<std::string, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)> m_ExternalFunctionCache;
		std::map<std::string, std::map<std::string, uint64_t (*)(LLL::Byte*, uint64_t, uint64_t, uint64_t)>> m_ExternalModuleCache;
	};

}