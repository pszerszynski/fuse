#include "data/Fuse_Function.hpp"

using namespace Fuse;

Type Fuse::Function::GetType() {
	return TYPE_FUNCTION;
}

std::shared_ptr<Object> Fuse::Function::Clone() {
	return std::make_shared<Function>(Func);
}

std::string Fuse::Function::ToString() {
	return "";
}

const std::vector<std::string>& Fuse::Function::GetArgs() {
	return Func->GetArgs();
}

std::shared_ptr<Fuse::Object> Fuse::Function::Call(std::vector< std::shared_ptr<Object> >& call_args) {
	return Func->Call(call_args);
}

bool Fuse::Function::IsTrue() {
	return Func != nullptr;
}