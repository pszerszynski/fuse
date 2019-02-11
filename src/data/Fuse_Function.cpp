#include "data/Fuse_Function.hpp"

using namespace Fuse;

Function* Fuse::Function::Clone() {
	auto n = new Function();
	*n = *this;
	return n;
}

std::string Fuse::Function::ToString() {
	return "function";
}

const std::vector<std::string>& Fuse::Function::GetArgs() {
	return Func->GetArgs();
}