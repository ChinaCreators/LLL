#include <iostream>
#include <fstream>
#include <cassert>
#include "LML.h"
#include "ActionGenerator.h"

using namespace std;
using namespace LML;

void TestCompileUnit()
{
	CompileUnit cu;
	const Type& t_int64 = *cu.GetI64();
	const Type& t_int32 = *cu.GetI32();

	Variable& sv1 = *cu.NewStaticVariable(t_int64);
	Function& func = *cu.NewFunction(t_int64, {&t_int32, &t_int32},"test_func");
	Variable& sv2 = *func.NewStaticVariable(t_int32);
	Variable& tv1 = *func.NewTemporaryVariable(t_int64, 0);

	uint64_t sts = cu.GetStaticVariableTotalSize();
	std::cout << sts << std::endl;
	assert(sts == 12);

	cu.RearrangeStaticVariable(0);
	assert(sv1.m_Address == 0);
	assert(sv2.m_Address == 8);
	assert(tv1.m_Address == 0);
}

void TestLASMGenerator()
{
	CompileUnit cu;
	LASMGenerator gen;

	const Type& t_uint8 = *cu.GetUI8();
	const Type& t_int64 = *cu.GetI64();
	const Type& t_int32 = *cu.GetI32();
	Type& t_struct1 = *cu.NewType();
	t_struct1.m_RealType = RealType::CompositeType;
	t_struct1.m_MemberVariables.emplace_back(t_int64, 0, true);
	t_struct1.m_MemberVariables.emplace_back(t_int32, 8, true);

	Variable& sv1 = *cu.NewStaticVariable(t_int64);
	Function& func = *cu.NewFunction(t_int64, {&t_int32, &t_int32},"test_func");
	cu.SetMainFunction(&func);
	Variable& sv2 = *func.NewStaticVariable(t_int32);
	Variable& tv1 = *func.NewTemporaryVariable(t_int64, 0);
	Variable& sv3 = *cu.NewStaticVariable(t_struct1);
	Variable& sv4 = *cu.NewStaticVariable(t_struct1);

	Variable& c_endl = *cu.NewConstantVariable(t_uint8);
	Variable& c_spl = *cu.NewConstantVariable(t_uint8);
	Variable& c_space = *cu.NewConstantVariable(t_uint8);

	auto gen_split_line = [&c_spl, &c_endl]() -> std::string {
		std::string re;
		re += LASMGenerator::Set0A(c_spl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");
		re += LASMGenerator::Set0A(c_spl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");
		re += LASMGenerator::Set0A(c_spl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");
		re += LASMGenerator::Set0A(c_spl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");
		re += LASMGenerator::Set0A(c_spl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");
		re += LASMGenerator::Set0A(c_endl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");

		return re;
	};
	auto gen_print_struct1 = [&c_endl, &c_space](uint64_t ssvar_addr, Variable var) -> std::string {
		std::string re;
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::LoadVariableAddressToArg(var, ebp, add_buf, 0);
		re += LASMGenerator::CallExternal("CoreModule:print_i64");
		re += LASMGenerator::Set1R(0);
		re += LASMGenerator::Set0A(c_space.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::Set1A(8);
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(add_buf);
		re += LASMGenerator::Set2A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:add_ui64");
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Ref0();
		re += LASMGenerator::CallExternal("CoreModule:print_i32");
		re += LASMGenerator::Set0A(c_space.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");
		re += LASMGenerator::Set0A(c_endl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");

		return re;
	};
	auto gen_set_struct1 = [](uint64_t ssvar_addr, Variable var, int64_t v1, int32_t v2) -> std::string {
		std::string re;
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::LoadVariableAddressToArg(var, ebp, add_buf, 0);
		re += LASMGenerator::Set1A(v1);	   // val
		re += LASMGenerator::CallExternal("CoreModule:store_i64");
		re += LASMGenerator::Set1R(0);
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::Set1A(8);
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(add_buf);
		re += LASMGenerator::Set2A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:add_ui64");
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Ref0();
		re += LASMGenerator::Set1A(v2);	   // val
		re += LASMGenerator::CallExternal("CoreModule:store_i32");

		return re;
	};
	// func.m_ActionGenerators.emplace_back(GeneratePushAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&tv1]() { return tv1; }));
	// func.m_ActionGenerators.emplace_back(GeneratePushAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&sv3]() { return sv3; }));
	// func.m_ActionGenerators.emplace_back(GeneratePopAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&tv1]() { return tv1; }));
	// func.m_ActionGenerators.emplace_back(GeneratePopAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&sv3]() { return sv3; }));

	func.m_ActionGenerators.emplace_back(ActionGenerator{[&sv1, &c_endl, &c_spl, &c_space]() -> std::string {
		std::string re;
		re += LASMGenerator::Set0A(c_endl.m_Address);
		re += LASMGenerator::Set1A(10);	   //\n
		re += LASMGenerator::CallExternal("CoreModule:store_ui8");
		re += LASMGenerator::Set0A(c_spl.m_Address);
		re += LASMGenerator::Set1A(45);	   //-
		re += LASMGenerator::CallExternal("CoreModule:store_ui8");
		re += LASMGenerator::Set0A(c_space.m_Address);
		re += LASMGenerator::Set1A(32);	   //' '
		re += LASMGenerator::CallExternal("CoreModule:store_ui8");
		re += LASMGenerator::Set0A(sv1.m_Address);
		re += LASMGenerator::Set1A(123);
		re += LASMGenerator::CallExternal("CoreModule:store_i64");
		return re;
	}});
	func.m_ActionGenerators.emplace_back(GeneratePushAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&sv1]() { return sv1; }));
	func.m_ActionGenerators.emplace_back(ActionGenerator{[&sv1, &c_endl]() -> std::string {
		std::string re;
		re += LASMGenerator::Set0A(sv1.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_i64");
		re += LASMGenerator::Set0A(c_endl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");

		re += LASMGenerator::Set0A(sv1.m_Address);
		re += LASMGenerator::Set1A(321);
		re += LASMGenerator::CallExternal("CoreModule:store_i64");

		re += LASMGenerator::Set0A(sv1.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_i64");
		re += LASMGenerator::Set0A(c_endl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");

		return re;
	}});
	func.m_ActionGenerators.emplace_back(GeneratePopAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&sv1]() { return sv1; }));
	func.m_ActionGenerators.emplace_back(ActionGenerator{[&sv1, &c_endl, &c_spl, &gen_split_line, &sv3, &sv4, ssvr = std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), &gen_print_struct1, &gen_set_struct1]() -> std::string {
		std::string re;
		re += LASMGenerator::Set0A(sv1.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_i64");
		re += LASMGenerator::Set0A(c_endl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");
		re += gen_split_line();
		uint64_t ssvar_addr = ssvr();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;

		re += gen_set_struct1(ssvar_addr, sv3, 111, 222);
		re += gen_set_struct1(ssvar_addr, sv4, 333, 444);

		re += gen_print_struct1(ssvar_addr, sv3);
		re += gen_print_struct1(ssvar_addr, sv4);

		re += GenerateCopyVariableAction(
				  ssvr, [&sv4] { return sv4; }, [&sv3] { return sv3; })
				  .m_LASMGenerator();
		re += gen_print_struct1(ssvar_addr, sv3);
		re += gen_print_struct1(ssvar_addr, sv4);
		re += gen_split_line();
		return re;
	}});

	auto result_str = gen.Generate(cu);
	std::cout << result_str << std::endl;
	std::ofstream file("test1_out.lll");
	file << result_str;
}

int main()
{
	TestCompileUnit();
	TestLASMGenerator();
	return 0;
}