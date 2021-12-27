#include "../LASMModule.h"
#include <iostream>

LASM_MODULE
{
	LASM_MODULE_FUNCTION("test", [](LLL::Byte* pcontent, uint64_t a0, uint64_t a1, uint64_t a2) -> uint64_t {
		std::cout << "test" << std::endl;
		return 0;
	});
	LASM_MODULE_FUNCTION("print_a0", [](LLL::Byte* pcontent, uint64_t a0, uint64_t a1, uint64_t a2) -> uint64_t {
		std::cout << a0 << std::endl;
		return 0;
	});
	LASM_MODULE_FUNCTION("print_a1", [](LLL::Byte* pcontent, uint64_t a0, uint64_t a1, uint64_t a2) -> uint64_t {
		std::cout << a1 << std::endl;
		return 0;
	});
	LASM_MODULE_FUNCTION("print_a2", [](LLL::Byte* pcontent, uint64_t a0, uint64_t a1, uint64_t a2) -> uint64_t {
		std::cout << a2 << std::endl;
		return 0;
	});
	LASM_MODULE_FUNCTION("print_ui64", [](LLL::Byte* pcontent, uint64_t a0, uint64_t a1, uint64_t a2) -> uint64_t {
		std::cout << *(uint64_t*)(pcontent + a0) << std::endl;
		return 0;
	});
	LASM_MODULE_FUNCTION("a2to_ui64", [](LLL::Byte* pcontent, uint64_t a0, uint64_t a1, uint64_t a2) -> uint64_t {
		*(uint64_t*)(pcontent + a0) = a2;
		return 0;
	});
	LASM_MODULE_FUNCTION("add_ui64", [](LLL::Byte* pcontent, uint64_t a0, uint64_t a1, uint64_t a2) -> uint64_t {
		uint64_t& r0 = *(uint64_t*)(pcontent + a0);
		uint64_t& r1 = *(uint64_t*)(pcontent + a1);
		uint64_t& r2 = *(uint64_t*)(pcontent + a2);

		r2 = r0 + r1;
		return 0;
	});
	LASM_MODULE_FUNCTION("ifeq_ui64", [](LLL::Byte* pcontent, uint64_t a0, uint64_t a1, uint64_t a2) -> uint64_t {
		uint64_t& r0 = *(uint64_t*)(pcontent + a0);
		uint64_t& r1 = *(uint64_t*)(pcontent + a1);
		uint64_t& r2 = *(uint64_t*)(pcontent + a2);

		r2 = (r0 == r1 ? 1 : 0);
		return 0;
	});
}