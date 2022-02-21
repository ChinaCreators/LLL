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
	std::ofstream file("test1_out.lasm");
	file << result_str;
}
//int main(){
//	int a,b,c;
//	std::cin>>a;
//	std::cin>>b;
//	c=add(a,b);
//	std::cout<<c;
//	return 0;
//}
//
//int add(int a,int b){
//	int c=a+b;
//	return c;
//}
void TestCallAndReturn(){
	CompileUnit cu;
	LASMGenerator gen;
	const Type& t_uint8 = *cu.GetUI8();
	const Type& t_int64 = *cu.GetI64();
	const Type& t_int32 = *cu.GetI32();
	Function& mainfun = *cu.NewFunction(t_int32, {&t_int32, &t_int32},"main");
	cu.SetMainFunction(&mainfun);
	Variable& tv1=*mainfun.NewTemporaryVariable(t_int32,0);
	Variable& tv2=*mainfun.NewTemporaryVariable(t_int32,4);
	Variable& tv3=*mainfun.NewTemporaryVariable(t_int32,8);
	Function& test_addfunc = *cu.NewFunction(t_int32, {&t_int32, &t_int32},"test_addfunc");
	Variable& tv4=*test_addfunc.NewTemporaryVariable(t_int32,0);
	Variable& tv5=*test_addfunc.NewTemporaryVariable(t_int32,4);
	Variable& tv6=*test_addfunc.NewTemporaryVariable(t_int32,8);
	std::vector<ActionGenerator> pushAg={ActionGenerator{[ssvar=std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv1]()->std::string{
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		std::string re;
		re += LASMGenerator::LoadVariableAddressToArg(tv1, ebp, add_buf, 0);
		return re;
	}
	},ActionGenerator{[ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv2]()->std::string{
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		std::string re;
		re += LASMGenerator::LoadVariableAddressToArg(tv2, ebp, add_buf, 0);
		return re;
	}}};
	ActionGenerator popAg{ActionGenerator{[ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv3]()->std::string{
		std::string res;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		res += LASMGenerator::Set0A(add_buf);
		res += LASMGenerator::Set1A(8);
		res += LASMGenerator::CallExternal("CoreModule:store_ui64");
		res += LASMGenerator::Set0A(esp);
		res += LASMGenerator::Set1A(add_buf);
		res += LASMGenerator::Set2A(esp);
		res += LASMGenerator::CallExternal("CoreModule:sub_ui64");
		res += LASMGenerator::Set1A(esp);
		res += LASMGenerator::Ref1();
		res += LASMGenerator::Ref1();
		res += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,0);
		res += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		return res;
	}}};
	mainfun.m_ActionGenerators.push_back(ActionGenerator{[&tv1,&tv2,&pushAg,&popAg,&test_addfunc,ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen)]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re+=LASMGenerator::LoadVariableAddressToArg(tv1,ebp,add_buf,0);
		re += LASMGenerator::CallExternal("CoreModule:read_ui32");
		re+=LASMGenerator::LoadVariableAddressToArg(tv2,ebp,add_buf,0);
		re += LASMGenerator::CallExternal("CoreModule:read_ui32");
		
		re+=LML::GenerateCallAction(ssvar,[&test_addfunc]()->Function&{return test_addfunc;},
		[&pushAg](){return pushAg;},[&popAg](){return popAg;}).m_LASMGenerator();
		re+=LASMGenerator::Set0A(8);
		re+=LASMGenerator::CallExternal("CoreModule:print_a0");
		return re;
	}});
	test_addfunc.m_ActionGenerators.push_back(ActionGenerator{[tv4,tv5,tv6,ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen)]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re+=LASMGenerator::LoadVariableAddressToArg(tv4,ebp,add_buf,0);
		re+=LASMGenerator::LoadVariableAddressToArg(tv5,ebp,add_buf,1);
		re+=LASMGenerator::LoadVariableAddressToArg(tv6,ebp,add_buf,2);
		re+=LASMGenerator::CallExternal("CoreModule:add_ui32");
		re+=LML::GenerateReturnAction(ssvar,[&tv6](){return tv6;}).m_LASMGenerator();
		return re;
	}});
	auto result_str = gen.Generate(cu);
	std::cout << result_str << std::endl;
	std::ofstream file("call_return.lll");
	file << result_str;
	std::cout << test_addfunc.m_Name << std::endl;
	
}

int main()
{
	//TestCompileUnit();
	//TestLASMGenerator();
	TestCallAndReturn();
	return 0;
}