#include <iostream>
#include <cassert>
#include "LML.h"

using namespace std;
using namespace LML;

void TestStaticVariableTotalSize()
{
	CompileUnit cu;
	Type& t_int64 = *cu.NewType();
	t_int64.m_SelfSize = 8;
	assert(t_int64.m_Id == 0);

	Type& t_int32 = *cu.NewType();
	t_int32.m_SelfSize = 4;
	assert(t_int32.m_Id == 1);

	Variable& sv1 = *cu.NewStaticVariable(t_int64, 0);
	Function& func = *cu.NewFunction(t_int64, {&t_int32, &t_int32});
	Variable& sv2 = *func.NewStaticVariable(t_int32, 0);
	Variable& tv1 = *func.NewTemporaryVariable(t_int64, 0);

	uint64_t sts = cu.GetStaticVariableTotalSize();
	std::cout << sts << std::endl;
	assert(sts == 12);
}

int main()
{
	TestStaticVariableTotalSize();
	return 0;
}