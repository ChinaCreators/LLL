#include "LML.h"
#include <cassert>

using namespace LML;

LML::MemoryManager::MemoryManager(uint64_t mem_size, uint64_t stack_size)
	: m_MaxMemorySize(mem_size), m_MaxStackSize(stack_size), m_HeapBaseAddress(0)
{
	m_StackBaseAddress = Allocate(m_MaxStackSize);
}

uint64_t LML::MemoryManager::Allocate(uint64_t size)
{
	uint64_t re = m_HeapBaseAddress;
	m_HeapBaseAddress += size;
	assert(m_HeapBaseAddress < m_MaxMemorySize);
	return re;
}

uint64_t LML::MemoryManager::GetStackBaseAddress() const
{
	return m_StackBaseAddress;
}

uint64_t LML::MemoryManager::GetHeapBaseAddress() const
{
	return m_HeapBaseAddress;
}

LML::Variable::Variable(const Type& type, uint64_t addr, bool is_tmp)
	: m_Address(addr), m_pType(&type), m_IsTemporary(is_tmp)
{
}

LML::Type::Type()
	: m_Id(0), m_SelfSize(0), m_RealType(RealType::UnknownType)
{
}

uint64_t LML::Type::GetSize() const
{
	uint64_t re = m_SelfSize;
	for (auto& i : m_MemberVariables)
		re += i.m_pType->GetSize();
	return re;
}

LML::Function::Function(const Type& return_type, const std::vector<const Type*>& parameters)
	: m_Return(&return_type), m_Parameters(parameters), m_pType(nullptr)
{
}

LML::Function::~Function()
{
	for (auto i : m_StaticVariables)
		delete i;
	for (auto i : m_TemporaryVariables)
		delete i;
}

Variable* LML::Function::NewStaticVariable(const Type& type, uint64_t addr)
{
	auto re = new Variable(type, addr, false);
	m_StaticVariables.emplace_back(re);
	return re;
}

Variable* LML::Function::NewTemporaryVariable(const Type& type, uint64_t addr)
{
	auto re = new Variable(type, addr, true);
	m_TemporaryVariables.emplace_back(re);
	return re;
}

uint64_t LML::Function::GetStaticVariableTotalSize() const
{
	uint64_t re = 0;
	for (auto& i : m_StaticVariables)
		re += i->m_pType->GetSize();
	return re;
}

uint64_t LML::Function::RearrangeStaticVariable(uint64_t base)
{
	for (auto i : m_StaticVariables)
	{
		i->m_Address = base;
		base += i->m_pType->GetSize();
	}
	return base;
}

std::string LML::GetBaseTypeNameById(uint64_t id)
{
	if (id == 0)
		return "i8";
	else if (id == 1)
		return "ui8";
	else if (id == 2)
		return "i32";
	else if (id == 3)
		return "ui32";
	else if (id == 4)
		return "i64";
	else if (id == 5)
		return "ui64";
	else if (id == 6)
		return "f";
	else if (id == 7)
		return "d";
	else
		abort();
}

LML::CompileUnit::CompileUnit()
	: m_TypeId(0), m_MainFunctionId(0)
{
	Type* i8 = NewType();	 // 0
	i8->m_RealType = RealType::BaseType;
	i8->m_SelfSize = 1;

	Type* ui8 = NewType();	  // 1
	ui8->m_RealType = RealType::BaseType;
	ui8->m_SelfSize = 1;

	Type* i32 = NewType();	  // 2
	i32->m_RealType = RealType::BaseType;
	i32->m_SelfSize = 4;

	Type* ui32 = NewType();	   // 3
	ui32->m_RealType = RealType::BaseType;
	ui32->m_SelfSize = 4;

	Type* i64 = NewType();	  // 4
	i64->m_RealType = RealType::BaseType;
	i64->m_SelfSize = 8;

	Type* ui64 = NewType();	   // 5
	ui64->m_RealType = RealType::BaseType;
	ui64->m_SelfSize = 8;

	Type* f = NewType();	// 6
	f->m_RealType = RealType::BaseType;
	f->m_SelfSize = 4;

	Type* d = NewType();	// 7
	d->m_RealType = RealType::BaseType;
	d->m_SelfSize = 8;
}

LML::CompileUnit::~CompileUnit()
{
	for (auto i : m_Types)
		delete i;
	for (auto i : m_StaticVariables)
		delete i;
	for (auto i : m_Functions)
		delete i;
}

uint64_t LML::CompileUnit::GenerateTypeId()
{
	return m_TypeId++;
}

uint64_t LML::CompileUnit::GetStaticVariableTotalSize() const
{
	uint64_t re = 0;
	for (auto i : m_StaticVariables)
		re += i->m_pType->GetSize();
	for (auto i : m_Functions)
		re += i->GetStaticVariableTotalSize();
	return re;
}

Type* LML::CompileUnit::NewType()
{
	auto re = new Type();
	re->m_Id = GenerateTypeId();
	m_Types.emplace_back(re);
	return re;
}

Variable* LML::CompileUnit::NewStaticVariable(const Type& type, uint64_t addr)
{
	auto re = new Variable(type, addr, false);
	m_StaticVariables.emplace_back(re);
	return re;
}

Function* LML::CompileUnit::NewFunction(const Type& return_type, const std::vector<const Type*>& parameters)
{
	auto re = new Function(return_type, parameters);
	m_Functions.emplace_back(re);
	return re;
}

uint64_t LML::CompileUnit::RearrangeStaticVariable(uint64_t base)
{
	for (auto i : m_StaticVariables)
	{
		i->m_Address = base;
		base += i->m_pType->GetSize();
	}
	for (auto i : m_Functions)
		base = i->RearrangeStaticVariable(base);
	return base;
}

Type* LML::CompileUnit::GetType(uint64_t type_id)
{
	assert(type_id < m_Types.size());
	return m_Types[type_id];
}

Variable* LML::CompileUnit::GetStaticVariable(uint64_t var_id)
{
	assert(var_id < m_StaticVariables.size());
	return m_StaticVariables[var_id];
}

Function* LML::CompileUnit::GetFunction(uint64_t func_id)
{
	assert(func_id < m_Functions.size());
	return m_Functions[func_id];
}

void LML::CompileUnit::SetMainFunctionId(uint64_t func_id)
{
	assert(func_id < m_Functions.size());
	m_MainFunctionId = func_id;
}

std::string LML::LASMGenerator::CallExternal(const std::string& func)
{
	return "call_ext " + func + "\n";
}

std::string LML::LASMGenerator::Set0A(uint64_t arg)
{
	return "set0a " + std::to_string(arg) + "\n";
}

std::string LML::LASMGenerator::Set1A(uint64_t arg)
{
	return "set1a " + std::to_string(arg) + "\n";
}

std::string LML::LASMGenerator::Set2A(uint64_t arg)
{
	return "set2a " + std::to_string(arg) + "\n";
}

std::string LML::LASMGenerator::Set0R(uint64_t arg)
{
	return "set0r " + std::to_string(arg) + "\n";
}

std::string LML::LASMGenerator::Set1R(uint64_t arg)
{
	return "set1r " + std::to_string(arg) + "\n";
}

std::string LML::LASMGenerator::Set2R(uint64_t arg)
{
	return "set2r " + std::to_string(arg) + "\n";
}

std::string LML::LASMGenerator::Ref0()
{
	return "ref0\n";
}

std::string LML::LASMGenerator::Ref1()
{
	return "ref1\n";
}

std::string LML::LASMGenerator::Ref2()
{
	return "ref2\n";
}

std::string LML::LASMGenerator::Goto(const std::string& label)
{
	return "goto " + label + "\n";
}

std::string LML::LASMGenerator::Goto0()
{
	return "goto0\n";
}

std::string LML::LASMGenerator::If()
{
	return "if\n";
}

std::string LML::LASMGenerator::Comment(const std::string& str)
{
	return ";" + str + "\n";
}

std::string LML::LASMGenerator::Label(const std::string& label)
{
	return ":" + label + "\n";
}

uint64_t LML::LASMGenerator::GetSystemStaticVariableAddres() const
{
	return m_SystemStaticVariableAddress;
}

uint64_t LML::LASMGenerator::GetUserStaticVariableAddress() const
{
	return m_UserStaticVariableAddress;
}

std::string LML::LASMGenerator::Generate(CompileUnit& cu)
{
	std::string re;
	MemoryManager mm;
	// system static variable
	m_SystemStaticVariableAddress = mm.Allocate(24);
	// system constant variable

	// user static variable
	m_UserStaticVariableAddress = mm.Allocate(cu.GetStaticVariableTotalSize());

	// generate code
	re += LASMGenerator::Set0A(m_SystemStaticVariableAddress);
	re += LASMGenerator::Set1A(0);
	re += LASMGenerator::CallExternal("CoreModule:store_ui64");
	re += LASMGenerator::Set0A(m_SystemStaticVariableAddress + 8);
	re += LASMGenerator::Set1A(0);
	re += LASMGenerator::CallExternal("CoreModule:store_ui64");
	re += LASMGenerator::Goto("function_" + std::to_string(cu.m_MainFunctionId));

	uint64_t func_cnt = 0;
	for (auto i : cu.m_Functions)
	{
		re += LASMGenerator::Label("function_" + std::to_string(func_cnt++));
		for (auto j : i->m_ActionGenerators)
			re += j.m_LASMGenerator();
	}
	return re;
}
