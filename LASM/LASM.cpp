#include "LASM.h"
#include "LASMModule.h"
#include <functional>
#include <cassert>

using namespace LASM;

void LASM::Compiler::CompileFromString(const std::string& str)
{
	if (m_Commands.size())
		m_Commands.clear();
	if (m_ExternalFunctionNames.size())
		m_ExternalFunctionNames.clear();

	std::vector<std::vector<std::string>> split_str;
	//parse token
	std::vector<std::string> line_buf;
	std::string buf;
	bool is_comment = false;
	for (const auto& i : str)
	{
		if (i == '\n' || i == '\r')
		{
			if (buf.size())
			{
				line_buf.emplace_back(buf);
				buf.clear();
			}
			if (line_buf.size())
			{
				split_str.emplace_back(line_buf);
				line_buf.clear();
			}
			is_comment = false;
			continue;
		}
		if (is_comment)
			continue;
		else
		{
			if (i == ';')
			{
				is_comment = true;
				if (buf.size())
				{
					line_buf.emplace_back(buf);
					buf.clear();
				}
			}
			else if (i == ' ' || i == '\t')
			{
				if (buf.size())
				{
					line_buf.emplace_back(buf);
					buf.clear();
				}
			}
			else
				buf += i;
		}
	}
	if (buf.size())
	{
		line_buf.emplace_back(buf);
		buf.clear();
	}
	if (line_buf.size())
	{
		split_str.emplace_back(line_buf);
		line_buf.clear();
	}
	//parse
	std::map<std::string, std::uint64_t> flags;
	std::uint64_t line_cnt = 0;
	std::vector<std::function<void()>> todo;
	for (std::uint64_t i = 0; i < split_str.size(); i++, line_cnt += 1)
	{
		const auto& lstrs = split_str[i];
		if (lstrs.size() == 1 && lstrs[0].size() > 1 && lstrs[0][0] == ':')
		{
			flags[lstrs[0].substr(1, lstrs[0].size() - 1)] = line_cnt;
			line_cnt -= 1;
		}
		else if (lstrs.size() == 2 && lstrs[0] == "call_ext")
		{
			auto fiter = m_ExternalFunctionNames.find(lstrs[1]);
			if (fiter != m_ExternalFunctionNames.end())
			{
				m_Commands.emplace_back(LLL::Command(0, fiter->second));
			}
			else
			{
				auto idx = m_ExternalFunctionNames.size();
				m_ExternalFunctionNames.insert(std::make_pair(lstrs[1], idx));
				m_Commands.emplace_back(LLL::Command(0, idx));
			}
		}
		else if (lstrs.size() == 2 && lstrs[0] == "set0a")
		{
			m_Commands.emplace_back(LLL::Command(1, std::stoull(lstrs[1])));
		}
		else if (lstrs.size() == 2 && lstrs[0] == "set1a")
		{
			m_Commands.emplace_back(LLL::Command(2, std::stoull(lstrs[1])));
		}
		else if (lstrs.size() == 2 && lstrs[0] == "set2a")
		{
			m_Commands.emplace_back(LLL::Command(3, std::stoull(lstrs[1])));
		}
		else if (lstrs.size() == 2 && lstrs[0] == "set0r")
		{
			m_Commands.emplace_back(LLL::Command(4, std::stoull(lstrs[1])));
		}
		else if (lstrs.size() == 2 && lstrs[0] == "set1r")
		{
			m_Commands.emplace_back(LLL::Command(5, std::stoull(lstrs[1])));
		}
		else if (lstrs.size() == 2 && lstrs[0] == "set2r")
		{
			m_Commands.emplace_back(LLL::Command(6, std::stoull(lstrs[1])));
		}
		else if (lstrs.size() == 1 && lstrs[0] == "ref0")
		{
			m_Commands.emplace_back(LLL::Command(7, 0));
		}
		else if (lstrs.size() == 1 && lstrs[0] == "ref1")
		{
			m_Commands.emplace_back(LLL::Command(8, 0));
		}
		else if (lstrs.size() == 1 && lstrs[0] == "ref2")
		{
			m_Commands.emplace_back(LLL::Command(9, 0));
		}
		else if (lstrs.size() == 2 && lstrs[0] == "goto")
		{
			auto fiter = flags.find(lstrs[1]);
			if (fiter == flags.end())
			{
				auto idx = m_Commands.size();
				auto fname = lstrs[1];
				todo.emplace_back([this, idx, fname, &flags]() {
					auto fiter2 = flags.find(fname);
					assert(fiter2 != flags.end());
					m_Commands[idx].m_Argument = fiter2->second;
				});
				m_Commands.emplace_back(LLL::Command(10, 0));
			}
			else
				m_Commands.emplace_back(LLL::Command(10, fiter->second));
		}
		else if (lstrs.size() == 1 && lstrs[0] == "goto0")
		{
			m_Commands.emplace_back(LLL::Command(11, 0));
		}
		else if (lstrs.size() == 1 && lstrs[0] == "if")
		{
			m_Commands.emplace_back(LLL::Command(12, 0));
		}
	}
	for (auto& i : todo)
	{
		i();
	}
}
const std::vector<LLL::Command>& LASM::Compiler::GetCommands() const
{
	return m_Commands;
}
std::vector<std::string> LASM::Compiler::GetExternalFunctionNames() const
{
	std::vector<std::string> re(m_ExternalFunctionNames.size());
	for (const auto& i : m_ExternalFunctionNames)
	{
		re[i.second] = i.first;
	}
	return re;
}

void LASM::Runner::RegisterExternalFunction(const std::string& name,uint64_t (*pfunc)(LLL::Byte*, uint64_t&, uint64_t&, uint64_t&))
{
	m_ExternalFunctionCache.emplace(std::make_pair(name,pfunc));
}

uint64_t LASM::Runner::RunCommands(const Compiler& c)
{
	LLL::VM vm;
	auto ext_funcs = c.GetExternalFunctionNames();
	for (auto& m : ext_funcs)
	{
		auto fiter = m_ExternalFunctionCache.find(m);
		if (fiter != m_ExternalFunctionCache.end())
		{
			vm.AddExternalFunction(fiter->second);
		}
		else
		{
			auto sp = m.find_first_of(':');
			assert(sp != m.npos);
			assert(sp != 0);
			auto mod_name = m.substr(0, sp);
			auto fs = GetFunctionsFromModule(mod_name);
			bool is_find = false;
			for (auto& i : fs)
			{
				if (i.first == m)
				{
					vm.AddExternalFunction(i.second);
					is_find = true;
				}
				m_ExternalFunctionCache.emplace(std::move(i));
			}
			if (!is_find)
			{
				abort();
			}
		}
	}
	return vm.Run(c.GetCommands());
}