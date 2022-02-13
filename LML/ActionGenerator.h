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
}