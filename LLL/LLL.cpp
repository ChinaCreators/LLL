#include "LLL.h"

using namespace LLL;

#define DF_CMD(i) else if (cmd.m_Id == i)

Command::Command(uint8_t id, uint64_t arg)
	: m_Id(id), m_Argument(arg)
{
}

VM::VM(uint64_t mem_size)
	: m_pMemory(new Byte[mem_size]), m_MemorySize(mem_size)
{
}

VM::~VM()
{
	if (m_MemorySize != 0)
		delete[] m_pMemory;
}

uint64_t VM::Run(const std::vector<Command>& cmds)
{
	for (m_Registers[sm_Pc] = 0; m_Registers[sm_Pc] < cmds.size(); ++m_Registers[sm_Pc])
	{
		const Command& cmd = cmds[m_Registers[sm_Pc]];

		if (cmd.m_Id == 0)	  //call_external
		{
			uint64_t re = m_ExternalFunctions[cmd.m_Argument](m_pMemory, m_Registers[sm_Arg0], m_Registers[sm_Arg1], m_Registers[sm_Arg2]);
			if (re != 0)
				return re;
		}
		DF_CMD(1)	 //set0 arg
		{
			m_Registers[sm_Arg0] = cmd.m_Argument;
		}
		DF_CMD(2)	 //set1 arg
		{
			m_Registers[sm_Arg1] = cmd.m_Argument;
		}
		DF_CMD(3)	 //set2 arg
		{
			m_Registers[sm_Arg2] = cmd.m_Argument;
		}
		DF_CMD(4)	 //set0 reg
		{
			m_Registers[sm_Arg0] = m_Registers[cmd.m_Argument];
		}
		DF_CMD(5)	 //set1 reg
		{
			m_Registers[sm_Arg1] = m_Registers[cmd.m_Argument];
		}
		DF_CMD(6)	 //set2 reg
		{
			m_Registers[sm_Arg2] = m_Registers[cmd.m_Argument];
		}
		DF_CMD(7)	 //ref0
		{
			m_Registers[sm_Arg0] = *(uint64_t*)(m_pMemory + m_Registers[sm_Arg0]);
		}
		DF_CMD(8)	 //ref1
		{
			m_Registers[sm_Arg1] = *(uint64_t*)(m_pMemory + m_Registers[sm_Arg1]);
		}
		DF_CMD(9)	 //ref2
		{
			m_Registers[sm_Arg2] = *(uint64_t*)(m_pMemory + m_Registers[sm_Arg2]);
		}
		DF_CMD(10)	  //goto arg
		{
			m_Registers[sm_Pc] = cmd.m_Argument - 1;
		}
		DF_CMD(11)	  //goto arg0
		{
			m_Registers[sm_Pc] = m_Registers[sm_Arg0] - 1;
		}
		DF_CMD(12)	  //if arg0
		{
			if (uint8_t(m_Registers[sm_Arg0]) == 0)
				m_Registers[sm_Pc] += 1;
		}
	}
	return 0;
}