#include <iostream>
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

	Variable& sv1 = *cu.NewStaticVariable(t_int64, 0);
	Function& func = *cu.NewFunction(t_int64, {&t_int32, &t_int32});
	Variable& sv2 = *func.NewStaticVariable(t_int32, 0);
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

	Type& t_int64 = *cu.GetType(4);
	Type& t_int32 = *cu.GetType(2);
	Variable& sv1 = *cu.NewStaticVariable(t_int64, 0);
	Function& func = *cu.NewFunction(t_int64, {&t_int32, &t_int32});
	Variable& sv2 = *func.NewStaticVariable(t_int32, 0);
	Variable& tv1 = *func.NewTemporaryVariable(t_int64, 0);

	func.m_ActionGenerators.emplace_back(GeneratePushBaseTypeAction(std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen), tv1));

	auto result_str = gen.Generate(cu);
	std::cout << result_str << std::endl;
}

int main()
{
	TestCompileUnit();
	TestLASMGenerator();
	return 0;
}