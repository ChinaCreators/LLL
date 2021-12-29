#pragma once
#include "../LASM/LASM.h"
#include <functional>

namespace LML
{

	class MemoryManager
	{
	public:
		inline static constexpr uint64_t sm_DefaultMemorySize = LLL::VM::sm_DefaultMemorySize;
		inline static constexpr uint64_t sm_DefaultStackSize = 0xffff;

		MemoryManager(uint64_t mem_size = sm_DefaultMemorySize, uint64_t stack_size = sm_DefaultStackSize);

		uint64_t Allocate(uint64_t size);

		uint64_t GetStackBaseAddress() const;

		// call at last
		uint64_t GetHeapBaseAddress() const;

	private:
		uint64_t m_MaxMemorySize;
		uint64_t m_MaxStackSize;
		uint64_t m_StackBaseAddress;
		uint64_t m_HeapBaseAddress;
	};

	struct Type;

	struct Variable
	{
		Variable(const Type& type, uint64_t addr);
		uint64_t m_Address;
		const Type* m_pType;
	};

	uint64_t GenerateTypeId();

	enum class RealType : uint8_t
	{
		NormalType = 0,
		PointerType = 1,
		FunctionType = 2
	};

	struct Type
	{
		Type();
		uint64_t m_Id;
		uint64_t m_SelfSize;
		std::vector<Variable> m_MemberVariables;
		RealType m_RealType;

		uint64_t GetSize() const;
	};

	struct ActionGenerator
	{
		std::function<std::string()> m_LASMGenerator;
	};

	struct Function
	{
		Function(const Type& return_type, const std::vector<const Type*>& parameters);
		~Function();

		uint64_t GetStaticVariableTotalSize() const;

		const Type* m_Return;
		std::vector<const Type*> m_Parameters;
		const Type* m_pType;
		std::vector<Variable*> m_StaticVariables;
		std::vector<Variable*> m_TemporaryVariables;
		std::vector<ActionGenerator> m_ActionGenerators;

		Variable* NewStaticVariable(const Type& type, uint64_t addr);
		Variable* NewTemporaryVariable(const Type& type, uint64_t addr);
	};

	class CompileUnit
	{
	public:
		~CompileUnit();

		uint64_t GetStaticVariableTotalSize() const;

		Type* NewType();
		Variable* NewStaticVariable(const Type& type, uint64_t addr);
		Function* NewFunction(const Type& return_type, const std::vector<const Type*>& parameters);

	private:
		std::vector<Type*> m_Types;
		std::vector<Variable*> m_StaticVariables;
		std::vector<Function*> m_Functions;
	};

	/*
	system static variable: 24
	ebp +0
	esp +8
	add_buffer +16

	system constant variable:
	*/
	class LASMGenerator
	{
	public:
		std::string Generate(const CompileUnit& cu);
	};

}