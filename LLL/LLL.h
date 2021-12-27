#pragma once
#include <iostream>
#include <cstring>
#include <climits>
#include <cstdint>
#include <vector>
#include <functional>
#include <map>

namespace LLL
{
	using Byte = uint8_t;

	struct Command
	{
		uint8_t m_Id;
		uint64_t m_Argument;

		Command(uint8_t id, uint64_t arg);
	};

	class VM
	{
	public:
		inline static constexpr const uint64_t sm_Arg0 = 0;
		inline static constexpr const uint64_t sm_Arg1 = 1;
		inline static constexpr const uint64_t sm_Arg2 = 2;
		inline static constexpr const uint64_t sm_Pc = 3;

		uint64_t Run(const std::vector<Command>& cmds);

		VM(uint64_t mem_size = 0xffffff);
		~VM();

		template<typename Callable>
		inline uint64_t AddExternalFunction(Callable&& f)
		{
			m_ExternalFunctions.emplace_back(std::forward<Callable>(f));
			return m_ExternalFunctions.size() - 1;
		}

	private:
		inline static constexpr const uint64_t sm_RegisterSize = 16;
		uint64_t m_Registers[sm_RegisterSize];
		std::vector<std::function<uint64_t(Byte*, uint64_t, uint64_t, uint64_t)>> m_ExternalFunctions;
		Byte* m_pMemory;
		uint64_t m_MemorySize;
	};
}	 // namespace LLL
