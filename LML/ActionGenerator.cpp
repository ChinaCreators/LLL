#include "ActionGenerator.h"
#include <cassert>

using namespace LML;

ActionGenerator LML::GeneratePushBaseTypeAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar)
{
	assert((uint8_t)gvar().m_pType->m_RealType & (uint8_t)RealType::BaseType);
	ActionGenerator re;
	re.m_LASMGenerator = [ssvar, gvar]() -> std::string {
		std::string str_re;
		auto ssvar_addr = ssvar();
		Variable var = gvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		str_re += LASMGenerator::LoadVariableAddressToArg(var, ebp, add_buf, 1);
		str_re += LASMGenerator::Set0A(esp);
		str_re += LASMGenerator::Ref0();
		str_re += LASMGenerator::CallExternal("CoreModule:mov_" + std::string(var.m_pType->m_BaseTypeSuffix));
		str_re += LASMGenerator::Set0A(add_buf);
		str_re += LASMGenerator::Set1A(var.m_pType->GetSize());
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		str_re += LASMGenerator::Set0A(esp);
		str_re += LASMGenerator::Set1A(add_buf);
		str_re += LASMGenerator::Set2A(esp);
		str_re += LASMGenerator::CallExternal("CoreModule:add_ui64");
		return str_re;
	};
	return re;
}

ActionGenerator LML::GeneratePushAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar)
{
	if ((uint8_t)gvar().m_pType->m_RealType & (uint8_t)RealType::BaseType)
		return GeneratePushBaseTypeAction(ssvar, gvar);

	ActionGenerator re;
	re.m_LASMGenerator = [ssvar, gvar]() -> std::string {
		std::string re_str;
		Variable var = gvar();
		for (auto i : var.m_pType->m_MemberVariables)
		{
			i.m_Address += var.m_Address;
			i.m_IsTemporary = var.m_IsTemporary;
			re_str += GeneratePushAction(
						  ssvar, [i]() { return i; })
						  .m_LASMGenerator();
		}
		return re_str;
	};
	return re;
}

ActionGenerator LML::GeneratePopBaseTypeAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar)
{
	assert((uint8_t)gvar().m_pType->m_RealType & (uint8_t)RealType::BaseType);
	ActionGenerator re;
	re.m_LASMGenerator = [ssvar, gvar]() -> std::string {
		std::string str_re;
		auto ssvar_addr = ssvar();
		Variable var = gvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		str_re += LASMGenerator::Set0A(add_buf);
		str_re += LASMGenerator::Set1A(var.m_pType->GetSize());
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		str_re += LASMGenerator::Set0A(esp);
		str_re += LASMGenerator::Set1A(add_buf);
		str_re += LASMGenerator::Set2A(esp);
		str_re += LASMGenerator::CallExternal("CoreModule:sub_ui64");
		str_re += LASMGenerator::LoadVariableAddressToArg(var, ebp, add_buf, 0);
		str_re += LASMGenerator::Set1A(esp);
		str_re += LASMGenerator::Ref1();
		str_re += LASMGenerator::CallExternal("CoreModule:mov_" + std::string(var.m_pType->m_BaseTypeSuffix));
		return str_re;
	};
	return re;
}

ActionGenerator LML::GeneratePopAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar)
{
	if ((uint8_t)gvar().m_pType->m_RealType & (uint8_t)RealType::BaseType)
		return GeneratePopBaseTypeAction(ssvar, gvar);

	ActionGenerator re;
	re.m_LASMGenerator = [ssvar, gvar]() -> std::string {
		std::string re_str;
		Variable var = gvar();
		for (auto i : var.m_pType->m_MemberVariables)
		{
			i.m_Address += var.m_Address;
			i.m_IsTemporary = var.m_IsTemporary;
			re_str += GeneratePopAction(
						  ssvar, [i]() { return i; })
						  .m_LASMGenerator();
		}
		return re_str;
	};
	return re;
}

ActionGenerator LML::GenerateCopyVariableBaseTypeAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) dst, LML_LAZY(Variable) src)
{
	assert(dst().m_pType == src().m_pType);
	assert((uint8_t)dst().m_pType->m_RealType & (uint8_t)RealType::BaseType);
	ActionGenerator re;
	re.m_LASMGenerator = [ssvar, dst, src]() -> std::string
	{
		std::string str_re;
		auto ssvar_addr = ssvar();
		Variable dst_v = dst();
		Variable src_v = src();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf = ssvar_addr + 24;
		str_re += LASMGenerator::LoadVariableAddressToArg(dst_v, ebp, add_buf, 1);
		str_re += LASMGenerator::Set0A(store_buf);
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		str_re += LASMGenerator::LoadVariableAddressToArg(src_v, ebp, add_buf, 1);
		str_re += LASMGenerator::Set0A(store_buf);
		str_re += LASMGenerator::Ref0();
		str_re += LASMGenerator::CallExternal("CoreModule:mov_" + std::string(dst_v.m_pType->m_BaseTypeSuffix));
		return str_re;
	};
	return re;
}

ActionGenerator LML::GenerateCopyVariableAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) dst, LML_LAZY(Variable) src)
{
	assert(dst().m_pType == src().m_pType);
	if ((uint8_t)dst().m_pType->m_RealType & (uint8_t)RealType::BaseType)
		return GenerateCopyVariableBaseTypeAction(ssvar, dst, src);

	ActionGenerator re;
	re.m_LASMGenerator = [ssvar, dst, src]() -> std::string
	{
		std::string str_re;
		Variable dst_v = dst();
		Variable src_v = src();
		for (auto i : dst_v.m_pType->m_MemberVariables)
		{
			Variable m_dv(i);
			Variable m_sv(i);
			m_dv.m_Address += dst_v.m_Address;
			m_dv.m_IsTemporary = dst_v.m_IsTemporary;
			m_sv.m_Address += src_v.m_Address;
			m_sv.m_IsTemporary = src_v.m_IsTemporary;
			str_re += GenerateCopyVariableAction(
						  ssvar, 
						  [m_dv]()
						  { return m_dv; },
						  [m_sv]()
						  { return m_sv; })
						  .m_LASMGenerator();
		}
		return str_re;
	};
	return re;
}

ActionGenerator LML::GenerateCallAction(LML_LAZY(uint64_t) ssvar,LML_LAZY(Function&) tarfunc,LML_LAZY(std::vector<ActionGenerator>) pushvars,LML_LAZY(ActionGenerator) popretval){
	assert(tarfunc().m_Name.size());
	ActionGenerator re;
	re.m_LASMGenerator=[ssvar,tarfunc,pushvars,popretval]() -> std::string{
		std::string str_re;
		auto ssvar_addr = ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf =ssvar_addr +24;
		Function& targetfunc=tarfunc();
		//ebp->*esp
		str_re += LASMGenerator::Set0A(esp);
		str_re += LASMGenerator::Ref0();
		str_re += LASMGenerator::Set1A(ebp);
		str_re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		//esp+16  store ebp and set space for pc
		str_re += LASMGenerator::Set0A(add_buf);
		str_re += LASMGenerator::Set1A(16);
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		str_re += LASMGenerator::Set0A(esp);
		str_re += LASMGenerator::Set1A(add_buf);
		str_re += LASMGenerator::Set2A(esp);
		str_re += LASMGenerator::CallExternal("CoreModule:add_ui64");


		//do construction about all param and push them
		if(pushvars){
			std::vector<ActionGenerator> pushvarsActionGenerator=pushvars();
			for(auto ag:pushvarsActionGenerator){
				str_re+=ag.m_LASMGenerator();
			}
		}
		//esp->ebp
		str_re += LASMGenerator::Set0A(ebp);
		str_re += LASMGenerator::Set1A(esp);
		str_re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		//ebp-(paramTotalSize)
		str_re += LASMGenerator::Set0A(add_buf);
		uint64_t paramTotalSize=0;
		for(auto t:targetfunc.m_Parameters){
			paramTotalSize+=t->GetSize();
		}
		str_re += LASMGenerator::Set1A(paramTotalSize);
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		str_re += LASMGenerator::Set0A(ebp);
		str_re += LASMGenerator::Set1A(add_buf);
		str_re += LASMGenerator::Set2A(ebp);
		str_re += LASMGenerator::CallExternal("CoreModule:sub_ui64");

		//save pc(pc->*esp   *esp+12->*esp)
		str_re += LASMGenerator::Set0A(store_buf);
		str_re += LASMGenerator::Set1A(ebp);
		str_re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		str_re += LASMGenerator::Set0A(add_buf);
		str_re += LASMGenerator::Set1A(8);
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		str_re += LASMGenerator::Set0A(store_buf);
		str_re += LASMGenerator::Set1A(add_buf);
		str_re += LASMGenerator::Set2A(store_buf);
		str_re += LASMGenerator::CallExternal("CoreModule:sub_ui64");
		str_re += LASMGenerator::Ref0();
		str_re += LASMGenerator::Set1R(3);
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		str_re += LASMGenerator::Set0A(add_buf);
		str_re += LASMGenerator::Set1A(12);
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");	
		str_re += LASMGenerator::Set0A(store_buf);
		str_re += LASMGenerator::Ref0();
		str_re += LASMGenerator::Set1A(add_buf);
		str_re += LASMGenerator::Set2A(store_buf);
		str_re += LASMGenerator::Ref2();
		str_re += LASMGenerator::CallExternal("CoreModule:add_ui64");



		//跳转
		str_re += LASMGenerator::Goto("function_"+targetfunc.m_Name);

		//pop ret_val_addr and destroy retval
		if(popretval){
			ActionGenerator popretvarActionGenerator=popretval();
			str_re +=popretvarActionGenerator.m_LASMGenerator();
		}

		//ebp-16->esp
		str_re += LASMGenerator::Set0A(add_buf);
		str_re += LASMGenerator::Set1A(16);
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		str_re += LASMGenerator::Set0A(ebp);
		str_re += LASMGenerator::Set1A(add_buf);
		str_re += LASMGenerator::Set2A(esp);
		str_re += LASMGenerator::CallExternal("CoreModule:sub_ui64");
		//*esp->ebp
		str_re += LASMGenerator::Set1A(esp);
		str_re += LASMGenerator::Ref1();
		str_re += LASMGenerator::Set0A(ebp);
		str_re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		return str_re;
	};
	return re;
}

ActionGenerator LML::GenerateReturnAction(LML_LAZY(uint64_t) ssvar,LML_LAZY(Variable)retvar,LML_LAZY(std::vector<ActionGenerator>) dtyvars){
	ActionGenerator re;
		re.m_LASMGenerator=[ssvar,retvar,dtyvars]() -> std::string{
		std::string str_re;
		auto ssvar_addr = ssvar();
		uint64_t ebp = ssvar_addr;
		uint64_t esp = ssvar_addr + 8;
		uint64_t add_buf = ssvar_addr + 16;
		uint64_t store_buf =ssvar_addr +24;
		if(dtyvars){
			//do destruction about all temporary variables except ret_var
			std::vector<ActionGenerator> destroyvars=dtyvars();
			for(auto dv:destroyvars)
				str_re+=dv.m_LASMGenerator();
		}
		if(retvar){
			Variable ret_var=retvar();
			str_re += LASMGenerator::LoadVariableAddressToArg(ret_var, ebp, add_buf, 1);
			str_re += LASMGenerator::Set0A(esp);
			str_re += LASMGenerator::Ref0();
			str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
			str_re += LASMGenerator::Set0A(add_buf);
			str_re += LASMGenerator::Set1A(8);
			str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
			str_re += LASMGenerator::Set0A(esp);
			str_re += LASMGenerator::Set1A(add_buf);
			str_re += LASMGenerator::Set2A(esp);
			str_re += LASMGenerator::CallExternal("CoreModule:add_ui64");

		}
		//get old_pc(epb-8)
		str_re += LASMGenerator::Set0A(store_buf);
		str_re += LASMGenerator::Set1A(ebp);
		str_re += LASMGenerator::CallExternal("CoreModule:mov_ui64");
		str_re += LASMGenerator::Set0A(add_buf);
		str_re += LASMGenerator::Set1A(8);
		str_re += LASMGenerator::CallExternal("CoreModule:store_ui64");
		str_re += LASMGenerator::Set0A(store_buf);
		str_re += LASMGenerator::Set1A(add_buf);
		str_re += LASMGenerator::Set2A(store_buf);
		str_re += LASMGenerator::CallExternal("CoreModule:sub_ui64");
		str_re += LASMGenerator::Ref0();
		str_re += LASMGenerator::Ref0();

		//goto pc
		str_re += LASMGenerator::Goto0();
		return str_re;
	};
	return re;
}