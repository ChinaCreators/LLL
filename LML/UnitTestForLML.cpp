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
	Type& t_int64 = *cu.NewType();
	t_int64.m_SelfSize = 8;
	assert(t_int64.m_Id == 8);

	Type& t_int32 = *cu.NewType();
	t_int32.m_SelfSize = 4;
	assert(t_int32.m_Id == 9);

	Variable& sv1 = *cu.NewStaticVariable(t_int64);
	Function& func = *cu.NewFunction(t_int64, {&t_int32, &t_int32});
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

	Type& t_uint8 = *cu.GetType(1);
	Type& t_int64 = *cu.GetType(4);
	Type& t_int32 = *cu.GetType(2);
	Type& t_struct1 = *cu.NewType();
	t_struct1.m_RealType = RealType::CompositeType;
	t_struct1.m_MemberVariables.emplace_back(t_int64, 0, true);
	t_struct1.m_MemberVariables.emplace_back(t_int32, 8, true);

	Variable& sv1 = *cu.NewStaticVariable(t_int64);
	Function& func = *cu.NewFunction(t_int64, {&t_int32, &t_int32});
	Variable& sv2 = *func.NewStaticVariable(t_int32);
	Variable& tv1 = *func.NewTemporaryVariable(t_int64, 0);
	Variable& sv3 = *cu.NewStaticVariable(t_struct1);
	Variable& c_endl = *cu.NewConstantVariable(t_uint8);

	// func.m_ActionGenerators.emplace_back(GeneratePushAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&tv1]() { return tv1; }));
	// func.m_ActionGenerators.emplace_back(GeneratePushAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&sv3]() { return sv3; }));
	// func.m_ActionGenerators.emplace_back(GeneratePopAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&tv1]() { return tv1; }));
	// func.m_ActionGenerators.emplace_back(GeneratePopAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), [&sv3]() { return sv3; }));

	func.m_ActionGenerators.emplace_back(ActionGenerator{[&sv1, &c_endl]() -> std::string {
		std::string re;
		re += LASMGenerator::Set0A(c_endl.m_Address);
		re += LASMGenerator::Set1A(10);
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
	func.m_ActionGenerators.emplace_back(ActionGenerator{[&sv1, &c_endl]() -> std::string {
		std::string re;
		re += LASMGenerator::Set0A(sv1.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_i64");
		re += LASMGenerator::Set0A(c_endl.m_Address);
		re += LASMGenerator::CallExternal("CoreModule:print_c");
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