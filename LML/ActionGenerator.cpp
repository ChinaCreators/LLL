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
		str_re += LASMGenerator::CallExternal("CoreModule:mov_" + GetBaseTypeNameById(var.m_pType->m_Id));
		str_re += LASMGenerator::Set0A(add_buf);
		str_re += LASMGenerator::Set1A(var.m_pType->GetSize());
		str_re += LASMGenerator::CallExternal("CoreModule:store_" + GetBaseTypeNameById(var.m_pType->m_Id));
		str_re += LASMGenerator::Set0A(esp);
		str_re += LASMGenerator::Set1A(add_buf);
		str_re += LASMGenerator::Set2A(esp);
		str_re += LASMGenerator::CallExternal("CoreModule:add_" + GetBaseTypeNameById(var.m_pType->m_Id));
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