#include <iostream>
#include <cassert>
#include "LASM.h"

using namespace std;
using namespace LLL;
using namespace LASM;

void TestCompiler()
{
	Compiler c;
	c.CompileFromString(R"(
		:start
		call_ext test;test call_ext
		goto start
		goto end
		set0a 1
		:end
	)");
	auto res = c.GetCommands();
	auto ext = c.GetExternalFunctionNames();
	assert(res[0].m_Id == 0);
	assert(res[0].m_Argument == 0);
	assert(ext[0] == "test");
	assert(res[1].m_Id == 10);
	assert(res[1].m_Argument == 0);
	assert(res[2].m_Id == 10);
	assert(res[2].m_Argument == 4);
	assert(res[3].m_Id == 1);
	assert(res[3].m_Argument == 1);

	cout << "test compiler success" << endl;
}

void TestRunCommand()
{
	Compiler c;
	c.CompileFromString(R"(
		:start
		call_ext CoreModule:test
		:end
	)");
	Runner r;
	r.RunCommands(c);
}

int main()
{
	TestCompiler();
	TestRunCommand();
	return 0;
}