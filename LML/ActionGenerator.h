#pragma once
#include "LML.h"

#define LML_LAZY(type) const std::function<type()>&

namespace LML
{
	ActionGenerator GeneratePushBaseTypeAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar);
	ActionGenerator GeneratePushAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar);
	ActionGenerator GeneratePopBaseTypeAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar);
	ActionGenerator GeneratePopAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar);
	ActionGenerator GenerateCopyVariableBaseTypeAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) dst, LML_LAZY(Variable) src);
	ActionGenerator GenerateCopyVariableAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) dst, LML_LAZY(Variable) src);
	ActionGenerator GenerateCallAction(LML_LAZY(uint64_t) ssvar,LML_LAZY(Function&) tarfunc,LML_LAZY(std::vector<ActionGenerator>) pushvars=nullptr,LML_LAZY(ActionGenerator) popretval=nullptr);
	ActionGenerator GenerateReturnAction(LML_LAZY(uint64_t) ssvar,LML_LAZY(Variable)retvar=nullptr,LML_LAZY(std::vector<ActionGenerator>) dtyvars=nullptr);
}