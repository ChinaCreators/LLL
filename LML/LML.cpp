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

uint64_t LML::GenerateTypeId()
{
	static uint64_t idx = 0;
	return idx++;
}

LML::Type::Type()
	: m_Id(0), m_SelfSize(0), m_RealType(RealType::NormalType)
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

LML::CompileUnit::~CompileUnit()
{
	for (auto i : m_Types)
		delete i;
	for (auto i : m_StaticVariables)
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

std::string LML::LASMGenerator::Generate(const CompileUnit& cu)
{
	std::string re;
	MemoryManager mm;
	// system static variable
	uint64_t ssvar_base = mm.Allocate(24);
	uint64_t ebp = ssvar_base;
	uint64_t esp = ssvar_base + 8;
	uint64_t add_buf = ssvar_base + 16;
	// system constant variable

	// user static variable
	uint64_t usvar_base = mm.Allocate(cu.GetStaticVariableTotalSize());
	return re;
}
