#include <iostream>
#include "LLL.h"

using namespace std;
using namespace LLL;

int print_args(Byte* pmem, uint64_t a1, uint64_t a2, uint64_t a3)
{
	std::cout << "arg1: " << a1 << "\targ2: " << a2 << "\targ3: " << a3 << endl;
	return 0;
}

int test(Byte* pmem, uint64_t a1, uint64_t a2, uint64_t a3)
{
	std::cout << "test" << endl;
	return 0;
}

int set_addr_int64(Byte* pmem, uint64_t a1, uint64_t a2, uint64_t a3)
{
	*(uint64_t*)(pmem + a1) = a2;
	return 0;
}

void TestArg012()
{
	VM v;
	auto e_pai = v.AddExternalFunction(print_args);
	auto e_ti = v.AddExternalFunction(test);
	auto e_saii = v.AddExternalFunction(set_addr_int64);
	vector<Command> cmds = {
		Command(1, 0),
		Command(2, 1),
		Command(3, 2),
		Command(10, 5),
		Command(0, e_pai),
		Command(1, 8),
		Command(11, 0),
		Command(0, e_pai),
		Command(1, 0),
		Command(2, 0),
		Command(0, e_saii),
		Command(0, e_ti),
		Command(1, 0),
		Command(7, 0),
		Command(11, 0),
		Command(0, e_pai)};
	auto re = v.Run(cmds);
	std::cout << "result : " << re << endl;
}

int main()
{
	TestArg012();
	return 0;
}