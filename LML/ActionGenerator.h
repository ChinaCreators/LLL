#pragma once
#include "LML.h"

#define LML_LAZY(type) const std::function<type()>&

namespace LML
{
	ActionGenerator GeneratePushBaseTypeAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar);
	ActionGenerator GeneratePushAction(LML_LAZY(uint64_t) ssvar, LML_LAZY(Variable) gvar);
}