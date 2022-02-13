#pragma once
#include "../LASM/LASM.h"
#include <functional>

namespace LML
{

	class MemoryManager
	{
	public:
		inline static constexpr uint64_t sm_DefaultMemorySize = LLL::VM::sm_DefaultMemorySize;
		inline static constexpr uint64_t sm_DefaultStackSize = 0xffff + 1;

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
		Variable(const Type& type, uint64_t addr, bool is_tmp);
		uint64_t m_Address;
		const Type* m_pType;
		bool m_IsTemporary;
	};

	enum class RealType : uint8_t
	{
		UnknownType = 0,
		BaseType = 1,
		PointerType = 3,
		FunctionType = 7,
		CompositeType = 8
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
		Function(const Type& return_type, const std::vector<const Type*>& parameters, uint64_t func_id);
		~Function();

		uint64_t GetStaticVariableTotalSize() const;

		uint64_t m_FunctionId;
		const Type* m_Return;
		std::vector<const Type*> m_Parameters;
		const Type* m_pType;
		std::vector<Variable*> m_StaticVariables;
		std::vector<Variable*> m_TemporaryVariables;
		std::vector<ActionGenerator> m_ActionGenerators;

		Variable* NewStaticVariable(const Type& type);
		Variable* NewTemporaryVariable(const Type& type, uint64_t addr);

		uint64_t RearrangeStaticVariable(uint64_t base);
	};

	class LASMGenerator;

	std::string GetBaseTypeNameById(uint64_t id);

	class CompileUnit
	{
	public:
		friend class LASMGenerator;

		CompileUnit();
		~CompileUnit();

		uint64_t GetStaticVariableTotalSize() const;
		uint64_t GetConstantVariableTotalSize() const;

		Type* NewType();
		Variable* NewStaticVariable(const Type& type);
		Variable* NewConstantVariable(const Type& type);
		Function* NewFunction(const Type& return_type, const std::vector<const Type*>& parameters);

		uint64_t RearrangeStaticVariable(uint64_t base);
		uint64_t RearrangeConstantVariable(uint64_t base);

		Type* GetType(uint64_t type_id);
		Function* GetFunction(uint64_t func_id);

		void SetMainFunctionId(uint64_t func_id);

	private:
		std::vector<Type*> m_Types;
		std::vector<Variable*> m_StaticVariables;
		std::vector<Variable*> m_ConstantVariables;
		std::vector<Function*> m_Functions;
		uint64_t m_MainFunctionId;
	};

	/*
	system static variable: 24
	ebp +0
	esp +8
	add_buffer +16
	store_buffer +24

	system constant variable:
	*/
	class LASMGenerator
	{
	public:
		static std::string CallExternal(const std::string& func);
		static std::string Set0A(uint64_t arg);
		static std::string Set1A(uint64_t arg);
		static std::string Set2A(uint64_t arg);
		static std::string Set0R(uint64_t arg);
		static std::string Set1R(uint64_t arg);
		static std::string Set2R(uint64_t arg);
		static std::string Ref0();
		static std::string Ref1();
		static std::string Ref2();
		static std::string Goto(const std::string& label);
		static std::string Goto0();
		static std::string If();
		static std::string Comment(const std::string& str);
		static std::string Label(const std::string& label);
		static std::string LoadVariableAddressToArg(const Variable& var, uint64_t ebp, uint64_t add_buf, uint64_t arg_idx);

		uint64_t GetSystemStaticVariableAddres() const;
		uint64_t GetConstantVariableAddress() const;
		uint64_t GetUserStaticVariableAddress() const;

	public:
		std::string Generate(CompileUnit& cu);

	private:
		uint64_t m_SystemStaticVariableAddress;
		uint64_t m_ConstantVariableAddress;
		uint64_t m_UserStaticVariableAddress;
	};

}