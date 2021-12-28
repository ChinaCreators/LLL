#include "../LASMModule.h"
#include <iostream>

#define TYPE_OPERATOR(type, name, oper) LASM_MODULE_FUNCTION(name, [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t { \
	type& r0 = *(type*)(pcontent + a0);                                                                                                            \
	type& r1 = *(type*)(pcontent + a1);                                                                                                            \
	type& r2 = *(type*)(pcontent + a2);                                                                                                            \
	r2 = r0 oper r1;                                                                                                                               \
	return 0;                                                                                                                                      \
});

#define TYPE_IO(type, name)                                                                                             \
	LASM_MODULE_FUNCTION("print_" name, [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t { \
		std::cout << *(type*)(pcontent + a0);                                                                           \
		return 0;                                                                                                       \
	});                                                                                                                 \
	LASM_MODULE_FUNCTION("read_" name, [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t {  \
		std::cin >> *(type*)(pcontent + a0);                                                                            \
		return 0;                                                                                                       \
	});

#define TYPE_LOAD_STORE(type, name)                                                                                     \
	LASM_MODULE_FUNCTION("load_" name, [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t {  \
		a2 = static_cast<uint64_t>(*(type*)(pcontent + a0));                                                            \
		return 0;                                                                                                       \
	});                                                                                                                 \
	LASM_MODULE_FUNCTION("store_" name, [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t { \
		*(type*)(pcontent + a0) = static_cast<type>(a1);                                                                \
		return 0;                                                                                                       \
	});

#define TYPE_MOV(type, name)                                                                                          \
	LASM_MODULE_FUNCTION("mov_" name, [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t { \
		*(type*)(pcontent + a0) = *(type*)(pcontent + a1);                                                            \
		return 0;                                                                                                     \
	});

// uint8_t as result type
#define TYPE_JUDGE(type, name, judge)                                                                          \
	LASM_MODULE_FUNCTION(name, [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t { \
		const type& r0 = *(type*)(pcontent + a0);                                                              \
		const type& r1 = *(type*)(pcontent + a1);                                                              \
		uint8_t& r2 = *(uint8_t*)(pcontent + a2);                                                              \
		r2 = (r0 judge r1 ? 1 : 0);                                                                            \
		return 0;                                                                                              \
	});

#define TYPE_JUDGES(type, name)        \
	TYPE_JUDGE(type, "ifeq_" name, ==) \
	TYPE_JUDGE(type, "ifne_" name, !=) \
	TYPE_JUDGE(type, "iflt_" name, <)  \
	TYPE_JUDGE(type, "ifle_" name, <=) \
	TYPE_JUDGE(type, "ifbt_" name, >)  \
	TYPE_JUDGE(type, "ifbe_" name, >=)

#define INTEGER_TYPE_OPERATORS(type, name) \
	TYPE_OPERATOR(type, "add_" name, +);   \
	TYPE_OPERATOR(type, "sub_" name, -);   \
	TYPE_OPERATOR(type, "mul_" name, *);   \
	TYPE_OPERATOR(type, "div_" name, /);   \
	TYPE_OPERATOR(type, "mod_" name, %);   \
	TYPE_OPERATOR(type, "sal_" name, <<);  \
	TYPE_OPERATOR(type, "sar_" name, >>);  \
	TYPE_JUDGES(type, name)                \
	TYPE_IO(type, name)                    \
	TYPE_LOAD_STORE(type, name)            \
	TYPE_MOV(type, name)

#define DECIMAL_TYPE_OPERATORS(type, name) \
	TYPE_OPERATOR(type, "add_" name, +);   \
	TYPE_OPERATOR(type, "sub_" name, -);   \
	TYPE_OPERATOR(type, "mul_" name, *);   \
	TYPE_OPERATOR(type, "div_" name, /);   \
	TYPE_JUDGES(type, name)                \
	TYPE_IO(type, name)                    \
	TYPE_LOAD_STORE(type, name)            \
	TYPE_MOV(type, name)

LASM_MODULE
{
	LASM_MODULE_FUNCTION("test", [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t {
		std::cout << "test" << std::endl;
		return 0;
	});
	LASM_MODULE_FUNCTION("print_a0", [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t {
		std::cout << a0;
		return 0;
	});
	LASM_MODULE_FUNCTION("print_a1", [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t {
		std::cout << a1;
		return 0;
	});
	LASM_MODULE_FUNCTION("print_a2", [](LLL::Byte* pcontent, uint64_t& a0, uint64_t& a1, uint64_t& a2) -> uint64_t {
		std::cout << a2;
		return 0;
	});

	TYPE_IO(char, "c");

	INTEGER_TYPE_OPERATORS(uint64_t, "ui64");
	INTEGER_TYPE_OPERATORS(int64_t, "i64");
	INTEGER_TYPE_OPERATORS(uint32_t, "ui32");
	INTEGER_TYPE_OPERATORS(int32_t, "i32");
	INTEGER_TYPE_OPERATORS(uint8_t, "ui8");
	INTEGER_TYPE_OPERATORS(int8_t, "i8");
	DECIMAL_TYPE_OPERATORS(float, "f");
	DECIMAL_TYPE_OPERATORS(double, "d");
}