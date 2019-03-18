#pragma once

#include "Lexer.hpp"
#include "data/Fuse_Object.hpp"
#include "AST.hpp"
#include "Parser.hpp"
#include "Operations.hpp"

#include <vector>
#include <memory>
#include <map>

using VariableScopes = std::vector< std::map<std::string, std::shared_ptr<Fuse::Object>> >;
using Scope = std::map<std::string, std::shared_ptr<Fuse::Object> >;

namespace Fuse {
	
	enum VAR_SET_STATE { SUCCESS , ERROR };
	
	extern struct Core {
	public:
		Core();
		
		// Returns shared_ptr pointer to variable 'var_name's object, if variable doesn't exist it returns nullptr
		std::shared_ptr<Fuse::Object>* GetVariable(const std::string& var_name);
		std::shared_ptr<Fuse::Object>  CallFunction(const std::string& func_name, std::vector< std::shared_ptr<Object> >& call_args);
		// Sets variable 'var_name' to an object, if variable doesn't exist returns enum ERROR otherwise SUCCESS
		VAR_SET_STATE SetVariable(const std::string& var_name, std::shared_ptr<Fuse::Object> obj);
		std::shared_ptr<Fuse::Object>* CreateVariable(const std::string& var_name, std::shared_ptr<Fuse::Object> obj);
		
		std::unique_ptr<ExprAST> Parse();
		int Load(void (*handle)(std::shared_ptr<Object>) = nullptr);
		
		Parser _Parser;
		Lexer _Lexer;
		
		std::vector<Operation> Operations[OP_COUNT];
	private:
	
		// Returns top scope
		Scope& TopScope();
		// Enter a new scope
		void EnterScope();
		// Leave current scope
		void LeaveScope();
		Scope GlobalScope;
		VariableScopes Scopes;
	} Core;
	
};