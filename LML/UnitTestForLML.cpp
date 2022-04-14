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
	const Type& t_uint32 = *cu.GetUI32();
	Function& mainfun = *cu.NewFunction(t_uint32, {&t_uint32, &t_uint32},"main");
	cu.SetMainFunction(&mainfun);
	Variable& tv1=*mainfun.NewTemporaryVariable(t_uint32,0);
	Variable& tv2=*mainfun.NewTemporaryVariable(t_uint32,4);
	Variable& tv3=*mainfun.NewTemporaryVariable(t_uint32,8);
	Function& test_addfunc = *cu.NewFunction(t_uint32, {&t_uint32, &t_uint32},"test_addfunc");
	Variable& tv4=*test_addfunc.NewTemporaryVariable(t_uint32,0);
	Variable& tv5=*test_addfunc.NewTemporaryVariable(t_uint32,4);
	Variable& tv6=*test_addfunc.NewTemporaryVariable(t_uint32,8);
	std::vector<ActionGenerator> pushAg={ActionGenerator{[ssvar=std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv1]()->std::string{
		std::string re;
		re += LML::GeneratePushAction(ssvar,[&tv1](){return tv1;}).m_LASMGenerator();
		return re;
	}
	},ActionGenerator{[ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv2]()->std::string{
		std::string re;
		re += LML::GeneratePushAction(ssvar,[&tv2](){return tv2	;}).m_LASMGenerator();
		return re;
	}}};
	ActionGenerator popAg{ActionGenerator{[ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv3]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(esp);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		re += LML::GenerateUpdateSsvarAction(ssvar,[store_buf](){return store_buf;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		//if acceptVar is temporaryVar,need get old ebp
		//re += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,0);----error
		re += LASMGenerator::Set0A(8);
		re += LASMGenerator::Set1A(store_buf);
		re += LASMGenerator::Ref1();
		re += LASMGenerator::Ref1();
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LML::GenerateUpdateSsvarAction(ssvar,[esp](){return esp;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		return re;
	}}};
	mainfun.m_ActionGenerators.push_back(ActionGenerator{[&tv1,&tv2,&tv3,&pushAg,&popAg,&test_addfunc,ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen)]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::LoadVariableAddressToArg(tv1,ebp,add_buf,0);
		re += LASMGenerator::CallExternal("CoreModule:read_ui32");
		re += LASMGenerator::LoadVariableAddressToArg(tv2,ebp,add_buf,0);
		re += LASMGenerator::CallExternal("CoreModule:read_ui32");
		re += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,0);
		re += LASMGenerator::Set1A(0);
		re += LASMGenerator::CallExternal("CoreModule:store_ui32");
		re += LML::GenerateUpdateSsvarAction(ssvar,[esp](){return esp;},
		[](){return 12;},[](){return "add";}).m_LASMGenerator();
		re+=LML::GenerateCallAction(ssvar,[&test_addfunc]()->Function&{return test_addfunc;},
		[&pushAg](){return pushAg;},[&popAg](){return popAg;}).m_LASMGenerator();
		re += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,0);
		re+=LASMGenerator::CallExternal("CoreModule:print_ui64");
		return re;
	}});
	test_addfunc.m_ActionGenerators.push_back(ActionGenerator{[&tv4,&tv5,&tv6,ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen)]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re+=LML::GeneratePushAction(ssvar,[&tv6](){return tv6;}).m_LASMGenerator();
		re +=LASMGenerator::Set0A(28);
		re +=LASMGenerator::Set1A(32);
		re +=LASMGenerator::Set2A(36);
		re += LASMGenerator::CallExternal("CoreModule:add_ui32");
		re+=LML::GenerateReturnAction(ssvar,[&tv6](){return tv6;}).m_LASMGenerator();
		return re;
	}});
	auto result_str = gen.Generate(cu);
	std::cout << result_str << std::endl;
	std::ofstream file("call_return.lasm");
	file << result_str;
	
}

void TestCallAndReturnRecursion(){
	CompileUnit cu;
	LASMGenerator gen;
	const Type& t_uint32 = *cu.GetUI32();
	Function& mainfun = *cu.NewFunction(t_uint32, {&t_uint32, &t_uint32},"main");
	cu.SetMainFunction(&mainfun);
	Variable& tv1=*mainfun.NewTemporaryVariable(t_uint32,0);
	Variable& tv2=*mainfun.NewTemporaryVariable(t_uint32,4);
	Variable& tv3=*mainfun.NewTemporaryVariable(t_uint32,8);
	Function& test_addfunc = *cu.NewFunction(t_uint32, {&t_uint32, &t_uint32},"test_addfunc");
	Variable& tv4=*test_addfunc.NewTemporaryVariable(t_uint32,0);
	Variable& tv5=*test_addfunc.NewTemporaryVariable(t_uint32,4);
	Variable& tv6=*test_addfunc.NewTemporaryVariable(t_uint32,8);
	Function& test_squrfunc = *cu.NewFunction(t_uint32, {&t_uint32},"test_squrfunc");
	Variable& tv7=*test_squrfunc.NewTemporaryVariable(t_uint32,0);
	//默认popAg
	auto popAgs=[&gen](Variable& accptVar,uint64_t accptVarAddr){
		auto popAg=ActionGenerator{[ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&accptVar,accptVarAddr]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(esp);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		re += LML::GenerateUpdateSsvarAction(ssvar,[store_buf](){return store_buf;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		//if acceptVar is temporaryVar,need get old ebp
		//re += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,0);----error
		re += LASMGenerator::Set0A(accptVarAddr);
		re += LASMGenerator::Set1A(store_buf);
		re += LASMGenerator::Ref1();
		re += LASMGenerator::Ref1();
		re += LASMGenerator::CallExternal("CoreModule:mov_"+std::string(accptVar.m_pType->m_BaseTypeSuffix));
		re += LML::GenerateUpdateSsvarAction(ssvar,[esp](){return esp;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		return re;
	}};
		return popAg;
	};
	ActionGenerator popAgs1=popAgs(tv3,8);
	ActionGenerator popAgs2=popAgs(tv1,0);
	ActionGenerator popAgs3=popAgs(tv2,4);
	auto pushAgs=[&gen](std::vector<Variable> &params){
		std::vector<ActionGenerator> pushAg;
		for(Variable &p:params){
			pushAg.push_back(ActionGenerator{[ssvar=std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&p]()->std::string{
				std::string re;
				re += LML::GeneratePushAction(ssvar,[&p](){return p;}).m_LASMGenerator();
				return re;
				}
			});
		}
		return pushAg;
	};
	std::vector<Variable> p1={tv1,tv2};
	std::vector<Variable> p2={tv1};
	std::vector<Variable> p3={tv2};
	std::vector<ActionGenerator> pushAgs1=pushAgs(p1);
	std::vector<ActionGenerator> pushAgs2=pushAgs(p2);
	std::vector<ActionGenerator> pushAgs3=pushAgs(p3);
	std::vector<LML::ActionGenerator> pushAg={ActionGenerator{[ssvar=std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv1,&pushAgs2,&popAgs2,&test_squrfunc]()->std::string{
		std::string re;
		re += LML::GenerateCallAction(ssvar,[&test_squrfunc]()->Function&{return test_squrfunc;},
		[&pushAgs2](){return pushAgs2;},[&popAgs2](){return popAgs2;}).m_LASMGenerator();
		re += LML::GeneratePushAction(ssvar,[&tv1](){return tv1;}).m_LASMGenerator();
		return re;
	}
	},ActionGenerator{[ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv2,&pushAgs3,&popAgs3,&test_squrfunc]()->std::string{
		std::string re;
		re += LML::GenerateCallAction(ssvar,[&test_squrfunc]()->Function&{return test_squrfunc;},
		[&pushAgs3](){return pushAgs3;},[popAgs3](){return popAgs3;}).m_LASMGenerator();
		re += LML::GeneratePushAction(ssvar,[&tv2](){return tv2;}).m_LASMGenerator();
		return re;
	}}};
	mainfun.m_ActionGenerators.push_back(ActionGenerator{[&tv1,&tv2,&tv3,&pushAg,&popAgs1,&test_addfunc,ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen)]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::LoadVariableAddressToArg(tv1,ebp,add_buf,0);
		re += LASMGenerator::CallExternal("CoreModule:read_ui32");
		re += LASMGenerator::LoadVariableAddressToArg(tv2,ebp,add_buf,0);
		re += LASMGenerator::CallExternal("CoreModule:read_ui32");
		re += LML::GenerateUpdateSsvarAction(ssvar,[esp](){return esp;},
		[](){return 8;},[](){return "add";}).m_LASMGenerator();
		re+=LML::GeneratePushAction(ssvar,[&tv3](){return tv3;}).m_LASMGenerator();
		//print a*a+b*b
		re+=LML::GenerateCallAction(ssvar,[&test_addfunc]()->Function&{return test_addfunc;},
		[&pushAg](){return pushAg;},[&popAgs1](){return popAgs1;}).m_LASMGenerator();
		re += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,0);
		re+=LASMGenerator::CallExternal("CoreModule:print_ui64");
		return re;
	}});
	test_addfunc.m_ActionGenerators.push_back(ActionGenerator{[&tv4,&tv5,&tv6,ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen)]()->std::string{
		//return x1+x2
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re+=LML::GeneratePushAction(ssvar,[&tv6](){return tv6;}).m_LASMGenerator();
		re +=LASMGenerator::Set0A(28);
		re +=LASMGenerator::Set1A(32);
		re +=LASMGenerator::Set2A(36);
		re += LASMGenerator::CallExternal("CoreModule:add_ui32");
		re+=LML::GenerateReturnAction(ssvar,[&tv6](){return tv6;}).m_LASMGenerator();
		return re;
	}});
		test_squrfunc.m_ActionGenerators.push_back(ActionGenerator{[&tv7,ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen)]()->std::string{
		//return x*x
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::LoadVariableAddressToArg(tv7,ebp,add_buf,1);
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LASMGenerator::Set2R(1);
		re += LASMGenerator::CallExternal("CoreModule:mul_ui32");
		re+=LML::GenerateReturnAction(ssvar,[&tv7](){return tv7;}).m_LASMGenerator();
		return re;
	}});
	auto result_str = gen.Generate(cu);
	std::cout << result_str << std::endl;
	std::ofstream file("call_return_recursion.lasm");
	file << result_str;
}

void TestFibonacci(){
	CompileUnit cu;
	LASMGenerator gen;
	const Type& t_uint32 = *cu.GetUI32();
	const Type& t_uint8 = *cu.GetUI8();
	Function& mainfun = *cu.NewFunction(t_uint32, {&t_uint32, &t_uint32},"main");
	cu.SetMainFunction(&mainfun);
	Variable& tv1=*mainfun.NewTemporaryVariable(t_uint32,0);
	Variable& tv2=*mainfun.NewTemporaryVariable(t_uint32,4);
	Function& fibonacci = *cu.NewFunction(t_uint32, {&t_uint32},"fibonacci");
	Variable& tv3=*fibonacci.NewTemporaryVariable(t_uint32,0);//x
	Variable& tv4=*fibonacci.NewTemporaryVariable(t_uint32,4);//fibonacci（x-1)
	Variable& tv5=*fibonacci.NewTemporaryVariable(t_uint32,8);//fibonacci(x-2)
	Variable& tv6=*fibonacci.NewTemporaryVariable(t_uint32,12);//add
	Variable& tv7=*fibonacci.NewTemporaryVariable(t_uint8,16);//if_res
	Variable& tv8=*cu.NewStaticVariable(t_uint32);//1

	std::vector<ActionGenerator> pushAg1={ActionGenerator{[ssvar=std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv1]()->std::string{
		std::string re;
		re += LML::GeneratePushAction(ssvar,[&tv1](){return tv1;}).m_LASMGenerator();
		return re;
	}
	}};
	std::vector<ActionGenerator> pushAg2={ActionGenerator{[ssvar=std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv3,&tv4]()->std::string{
		//x-1
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,1);
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::Set1A(1);
		re += LASMGenerator::CallExternal("CoreModule:store_ui32");
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(add_buf);
		re += LASMGenerator::Set2A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:sub_ui32");
		re += LASMGenerator::LoadVariableAddressToArg(tv4,ebp,add_buf,0);
		re += LASMGenerator::Set1A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LML::GeneratePushAction(ssvar,[&tv4](){return tv4;}).m_LASMGenerator();
		return re;
	}
	}};
	std::vector<ActionGenerator> pushAg3={ActionGenerator{[ssvar=std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv3,&tv5]()->std::string{
		//x-2
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,1);
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::Set1A(2);
		re += LASMGenerator::CallExternal("CoreModule:store_ui32");
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(add_buf);
		re += LASMGenerator::Set2A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:sub_ui32");
		re += LASMGenerator::LoadVariableAddressToArg(tv5,ebp,add_buf,0);
		re += LASMGenerator::Set1A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LML::GeneratePushAction(ssvar,[&tv5](){return tv5;}).m_LASMGenerator();
		return re;
	}
	}};
	ActionGenerator popAg1{ActionGenerator{[ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv2]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(esp);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		re += LML::GenerateUpdateSsvarAction(ssvar,[store_buf](){return store_buf;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		//if acceptVar is temporaryVar,need get old ebp
		//re += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,0);----error
		re += LASMGenerator::Set0A(4);
		re += LASMGenerator::Set1A(store_buf);
		re += LASMGenerator::Ref1();
		re += LASMGenerator::Ref1();
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LML::GenerateUpdateSsvarAction(ssvar,[esp](){return esp;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		return re;
	}}};
	ActionGenerator popAg2{ActionGenerator{[ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv4]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		uint64_t temp_buf = ssvar_addr + 32;
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(esp);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		re += LML::GenerateUpdateSsvarAction(ssvar,[store_buf](){return store_buf;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		//if acceptVar is temporaryVar,need get old ebp
		re += LASMGenerator::Set0A(temp_buf);
		re += LASMGenerator::Set1A(33);//old ebp and now ebp diff_size
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::LoadVariableAddressToArg(tv4,ebp,add_buf,1);
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::Set1A(temp_buf);
		re += LASMGenerator::Set2A(add_buf);
		re += LASMGenerator::CallExternal("CoreModule:sub_ui64");
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::Ref0();
		re += LASMGenerator::Set1A(store_buf);
		re += LASMGenerator::Ref1();
		re += LASMGenerator::Ref1();
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LML::GenerateUpdateSsvarAction(ssvar,[esp](){return esp;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		return re;
	}}};
	ActionGenerator popAg3{ActionGenerator{[ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen),&tv5]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		uint64_t temp_buf = ssvar_addr + 32;
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(esp);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		re += LML::GenerateUpdateSsvarAction(ssvar,[store_buf](){return store_buf;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		//if acceptVar is temporaryVar,need get old ebp
		re += LASMGenerator::Set0A(temp_buf);
		re += LASMGenerator::Set1A(33);//old ebp and now ebp diff_size
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::LoadVariableAddressToArg(tv5,ebp,add_buf,1);
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::Set1A(temp_buf);
		re += LASMGenerator::Set2A(add_buf);
		re += LASMGenerator::CallExternal("CoreModule:sub_ui64");
		re += LASMGenerator::Set0A(add_buf);
		re += LASMGenerator::Ref0();
		re += LASMGenerator::Set1A(store_buf);
		re += LASMGenerator::Ref1();
		re += LASMGenerator::Ref1();
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LML::GenerateUpdateSsvarAction(ssvar,[esp](){return esp;},
		[](){return 8;},[](){return "sub";}).m_LASMGenerator();
		return re;
	}}};
	mainfun.m_ActionGenerators.push_back(ActionGenerator{[&tv1,&tv2,&tv8,&pushAg1,&popAg1,&fibonacci,ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen)]()->std::string{
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		re += LASMGenerator::LoadVariableAddressToArg(tv8,ebp,add_buf,0);
		re += LASMGenerator::Set1A(1);
		re += LASMGenerator::CallExternal("CoreModule:store_ui32");
		re += LASMGenerator::LoadVariableAddressToArg(tv1,ebp,add_buf,0);
		re += LASMGenerator::CallExternal("CoreModule:read_ui32");
		re += LML::GenerateUpdateSsvarAction(ssvar,[esp](){return esp;},
		[&tv1](){return tv1.m_pType->GetSize();},[](){return "add";}).m_LASMGenerator();
		re += LML::GeneratePushAction(ssvar,[&tv2](){return tv2;}).m_LASMGenerator();
		re +=LML::GenerateCallAction(ssvar,[&fibonacci]()->Function&{return fibonacci;},
		[&pushAg1](){return pushAg1;},[&popAg1](){return popAg1;}).m_LASMGenerator();
		re += LASMGenerator::LoadVariableAddressToArg(tv2,ebp,add_buf,0);
		re+=LASMGenerator::CallExternal("CoreModule:print_ui32");
		return re;
	}});
	int if_count=0;
	fibonacci.m_ActionGenerators.push_back(ActionGenerator{[&if_count,&fibonacci,&pushAg2,&pushAg3,&popAg2,&popAg3,&tv3,&tv4,&tv5,&tv6,&tv7,&tv8,ssvar= std::bind(&LASMGenerator::GetSystemStaticVariableAddres, &gen)]()->std::string{
		if_count++;
		std::string re;
		uint64_t ssvar_addr=ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		uint64_t temp_buf = ssvar_addr + 32;
		re += LML::GeneratePushAction(ssvar,[&tv4](){return tv4;}).m_LASMGenerator();
		re += LML::GeneratePushAction(ssvar,[&tv5](){return tv5;}).m_LASMGenerator();
		re += LML::GeneratePushAction(ssvar,[&tv6](){return tv6;}).m_LASMGenerator();
		re += LASMGenerator::LoadVariableAddressToArg(tv7,ebp,add_buf,0);
		re += LASMGenerator::Set1A(0);
		re += LASMGenerator::CallExternal("CoreModule:store_ui8");
		re += LML::GenerateUpdateSsvarAction(ssvar,[esp](){return esp;},
		[](){return 1;},[](){return "add";}).m_LASMGenerator();
		//n<=2
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Set1A(2);
		re += LASMGenerator::CallExternal("CoreModule:store_ui32");
		re += LASMGenerator::LoadVariableAddressToArg(tv3,ebp,add_buf,1);
		re += LASMGenerator::Set0A(temp_buf);
		re += LASMGenerator::CallExternal("CoreModule:mov_ui32");
		re += LASMGenerator::LoadVariableAddressToArg(tv7,ebp,add_buf,2);
		re += LASMGenerator::Set0A(temp_buf);
		re += LASMGenerator::Set1A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:ifle_ui32");
		re += LASMGenerator::LoadVariableAddressToArg(tv7,ebp,add_buf,0);
		re += LASMGenerator::Ref0();
		re += LASMGenerator::If();
		re += LASMGenerator::Goto("if_true_"+std::to_string(if_count));
		re += LASMGenerator::Goto("if_false_"+std::to_string(if_count));
		//return x------1
		re += LASMGenerator::Label("if_true_"+std::to_string(if_count));
		re += LML::GenerateReturnAction(ssvar,[&tv8](){return tv8;}).m_LASMGenerator();
		re += LASMGenerator::Goto("if_return_"+std::to_string(if_count));
		//return fibonacci(x-1)+fibonacci(x-2)
		re += LASMGenerator::Label("if_false_"+std::to_string(if_count));
		re += LML::GenerateCallAction(ssvar,[&fibonacci]()->Function&{return fibonacci;},
		[&pushAg2](){return pushAg2;},[&popAg2](){return popAg2;}).m_LASMGenerator();
		re += LML::GenerateCallAction(ssvar,[&fibonacci]()->Function&{return fibonacci;},
		[&pushAg3](){return pushAg3;},[&popAg3](){return popAg3;}).m_LASMGenerator();
		//c=a+b
		re += LASMGenerator::LoadVariableAddressToArg(tv4,ebp,add_buf,1);
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::LoadVariableAddressToArg(tv5,ebp,add_buf,1);
		re += LASMGenerator::Set0A(temp_buf);
		re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		re += LASMGenerator::LoadVariableAddressToArg(tv6,ebp,add_buf,2);
		re += LASMGenerator::Set0A(store_buf);
		re += LASMGenerator::Ref0();
		re += LASMGenerator::Set1A(temp_buf);
		re += LASMGenerator::Ref1();
		re += LASMGenerator::CallExternal("CoreModule:add_"+std::string(tv6.m_pType->m_BaseTypeSuffix));
		re += LML::GenerateReturnAction(ssvar,[&tv6](){return tv6;}).m_LASMGenerator();
		re += LASMGenerator::Goto("if_return_"+std::to_string(if_count));
		re += LASMGenerator::Label("if_return_"+std::to_string(if_count));
		return re;
	}});
	auto result_str = gen.Generate(cu);
	std::cout << result_str << std::endl;
	std::ofstream file("fibonacci.lasm");
	file << result_str;
	
}

int main()
{
	//TestCompileUnit();
	//TestLASMGenerator();
	//TestCallAndReturn();
	//TestCallAndReturnRecursion();
	TestFibonacci();
	return 0;
}