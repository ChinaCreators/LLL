#include "LML.h"
#include <cassert>
#include <iostream>

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
	: m_SelfSize(0), m_RealType(RealType::UnknownType)
{
	memset(m_BaseTypeSuffix,0,sizeof(m_BaseTypeSuffix));
}

uint64_t LML::Type::GetSize() const
{
	uint64_t re = m_SelfSize;
	for (auto& i : m_MemberVariables)
		re += i.m_pType->GetSize();
	return re;
}

LML::Function::Function(const Type& return_type, const std::vector<const Type*>& parameters, const std::string& name)
	: m_Return(&return_type), m_Parameters(parameters), m_pType(nullptr), m_Name(name)
{
}

LML::Function::~Function()
{
	for (auto i : m_StaticVariables)
		delete i;
	for (auto i : m_TemporaryVariables)
		delete i;
}

Variable* LML::Function::NewStaticVariable(const Type& type)
{
	auto re = new Variable(type, 0, false);
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

LML::CompileUnit::CompileUnit()
	: m_pMainFunction(nullptr)
{
	Type* i8 = NewType();	 // 0
	i8->m_RealType = RealType::BaseType;
	i8->m_SelfSize = 1;
	strcpy(i8->m_BaseTypeSuffix,"i8");

	Type* ui8 = NewType();	  // 1
	ui8->m_RealType = RealType::BaseType;
	ui8->m_SelfSize = 1;
	strcpy(ui8->m_BaseTypeSuffix,"ui8");

	Type* i32 = NewType();	  // 2
	i32->m_RealType = RealType::BaseType;
	i32->m_SelfSize = 4;
	strcpy(i32->m_BaseTypeSuffix,"i32");

	Type* ui32 = NewType();	   // 3
	ui32->m_RealType = RealType::BaseType;
	ui32->m_SelfSize = 4;
	strcpy(ui32->m_BaseTypeSuffix,"ui32");

	Type* i64 = NewType();	  // 4
	i64->m_RealType = RealType::BaseType;
	i64->m_SelfSize = 8;
	strcpy(i64->m_BaseTypeSuffix,"i64");

	Type* ui64 = NewType();	   // 5
	ui64->m_RealType = RealType::BaseType;
	ui64->m_SelfSize = 8;
	strcpy(ui64->m_BaseTypeSuffix,"ui64");

	Type* f = NewType();	// 6
	f->m_RealType = RealType::BaseType;
	f->m_SelfSize = 4;
	strcpy(f->m_BaseTypeSuffix,"f");

	Type* d = NewType();	// 7
	d->m_RealType = RealType::BaseType;
	d->m_SelfSize = 8;
	strcpy(d->m_BaseTypeSuffix,"d");
}

LML::CompileUnit::~CompileUnit()
{
	for (auto i : m_Types)
		delete i;
	for (auto i : m_StaticVariables)
		delete i;
	for (auto i : m_ConstantVariables)
		delete i;
	for (auto i : m_Functions)
		delete i;
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

uint64_t LML::CompileUnit::GetConstantVariableTotalSize() const
{
	uint64_t re = 0;
	for (auto i : m_ConstantVariables)
		re += i->m_pType->GetSize();
	return re;
}

Type* LML::CompileUnit::NewType()
{
	auto re = new Type();
	m_Types.emplace_back(re);
	return re;
}

Variable* LML::CompileUnit::NewStaticVariable(const Type& type)
{
	auto re = new Variable(type, 0, false);
	m_StaticVariables.emplace_back(re);
	return re;
}

Variable* LML::CompileUnit::NewConstantVariable(const Type& type)
{
	auto re = new Variable(type, 0, false);
	m_ConstantVariables.emplace_back(re);
	return re;
}

Function* LML::CompileUnit::NewFunction(const Type& return_type, const std::vector<const Type*>& parameters,const std::string& name)
{
	auto re = new Function(return_type, parameters, name);
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

uint64_t LML::CompileUnit::RearrangeConstantVariable(uint64_t base)
{
	for (auto i : m_ConstantVariables)
	{
		i->m_Address = base;
		base += i->m_pType->GetSize();
	}
	return base;
}

void LML::CompileUnit::SetMainFunction(const Function* pfunc)
{
	assert(pfunc);
	m_pMainFunction = pfunc;
}

const Type* LML::CompileUnit::GetI8() const
{
	return m_Types[0];
}

const Type* LML::CompileUnit::GetUI8() const
{
	return m_Types[1];
}

const Type* LML::CompileUnit::GetI32() const
{
	return m_Types[2];
}

const Type* LML::CompileUnit::GetUI32() const
{
	return m_Types[3];
}

const Type* LML::CompileUnit::GetI64() const
{
	return m_Types[4];
}

const Type* LML::CompileUnit::GetUI64() const
{
	return m_Types[5];
}

const Type* LML::CompileUnit::GetF() const
{
	return m_Types[6];
}

const Type* LML::CompileUnit::GetD() const
{
	return m_Types[7];
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

std::string LASMGenerator::LoadVariableAddressToArg(const Variable& var, uint64_t ebp, uint64_t add_buf, uint64_t arg_idx)
{
	std::string re;
	assert(arg_idx < 3);
	if (!var.m_IsTemporary)
	{
		re += "set" + std::to_string(arg_idx) + "a " + std::to_string(var.m_Address) + "\n";
	}
	else
	{
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::Set1A(var.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::Set0A(ebp);
		re += LASMGenerator::Set1A(add_buf);
		re += LASMGenerator::Set2A(add_buf);
		re += LASMGenerator::CallExternal("CoreModule:add_ui64");
		re += "set" + std::to_string(arg_idx) + "a " + std::to_string(add_buf) + "\n";
		re += "ref" + std::to_string(arg_idx) + "\n";
	}
	return re;
}

uint64_t LML::LASMGenerator::GetSystemStaticVariableAddres() const
{
	return m_SystemStaticVariableAddress;
}

uint64_t LML::LASMGenerator::GetConstantVariableAddress() const
{
	return m_ConstantVariableAddress;
}

uint64_t LML::LASMGenerator::GetUserStaticVariableAddress() const
{
	return m_UserStaticVariableAddress;
}

std::string LML::LASMGenerator::Generate(CompileUnit& cu)
{
	assert(cu.m_pMainFunction);
	std::string re;
	MemoryManager mm;
	// system static variable
	m_SystemStaticVariableAddress = mm.Allocate(32);
	// constant variable
	m_ConstantVariableAddress = mm.Allocate(cu.GetConstantVariableTotalSize());
	cu.RearrangeConstantVariable(m_ConstantVariableAddress);
	// user static variable
	m_UserStaticVariableAddress = mm.Allocate(cu.GetStaticVariableTotalSize());
	cu.RearrangeStaticVariable(m_UserStaticVariableAddress);

	// generate code
	re += LASMGenerator::Set0A(m_SystemStaticVariableAddress);
	re += LASMGenerator::Set1A(0);
	re += LASMGenerator::CallExternal("CoreModule:store_ui64");
	re += LASMGenerator::Set0A(m_SystemStaticVariableAddress + 8);
	re += LASMGenerator::Set1A(0);
	re += LASMGenerator::CallExternal("CoreModule:store_ui64");
	re += LASMGenerator::Goto("function_" + cu.m_pMainFunction->m_Name);

	for (const auto& i : cu.m_Functions)
	{
		re += LASMGenerator::Label("function_" + i->m_Name);
		for (const auto& j : i->m_ActionGenerators)
			re += j.m_LASMGenerator();
	}
	return re;
}
