#include "LML.h"
#include <cassert>

using namespace LML;

LML::MemoryManager::MemoryManager(uint64_t mem_size, uint64_t stack_size)
	: m_MaxMemorySize(mem_size), m_MaxStackSize(stack_size)
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

LML::Variable::Variable(const Type& type, uint64_t addr)
	: m_Address(addr), m_pType(&type)
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
	auto re = new Variable(type, addr);
	m_StaticVariables.emplace_back(re);
	return re;
}

Variable* LML::Function::NewTemporaryVariable(const Type& type, uint64_t addr)
{
	auto re = new Variable(type, addr);
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
	: m_TypeId(0)
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
	auto re = new Variable(type, addr);
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
	for (auto i : cu.m_Functions)
	{
		for (auto j : i->m_ActionGenerators)
			re += j.m_LASMGenerator();
	}
	return re;
}
