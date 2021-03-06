#include "ast/Variable.hpp"

#include "Fuse_Core.hpp"

using namespace Fuse;

TypeAST Fuse::VariableAST::GetType() {
	return NODE_VARIABLE;
}

std::shared_ptr<Fuse::Object> VariableAST::Eval() {
	auto v = Fuse::Core.GetVariable(Identifier);
	if (v == nullptr) {
		Core.SetErrorMessage("unknown variable \"" + Identifier + "\" referenced");
		return nullptr;
	}

	return v;
}

std::shared_ptr<Fuse::Object> VariableAST::Assign(std::shared_ptr<Object> obj) {
	Fuse::Core.CreateVariable(Identifier, obj);
	return obj;
}

const std::string& VariableAST::GetIdentifier() {
	return Identifier;
}
