#pragma once

#include <vector>
#include <array>

#include "ast/Base.hpp"

namespace Fuse {

	class BlockAST : public ExprAST {
	public:
		BlockAST(std::vector<std::unique_ptr<ExprAST> >& _stats);
	
		// Eval returns something if there is a return statement
		// inside the block, otherwise it returns nullptr
		std::shared_ptr<Fuse::Object> Eval();
		TypeAST GetType();
	private:
		std::vector<std::unique_ptr<ExprAST> > Statements;
	};

	class ReturnAST : public ExprAST {
	public:
		// _expr can be nullptr, if it is then it
		// is an empty return statement that implicitly
		// returns a Fuse::Null object.
		ReturnAST(std::unique_ptr<ExprAST>& _expr);
		
		std::shared_ptr<Fuse::Object> Eval();
		TypeAST GetType();
	private:
		std::unique_ptr<ExprAST> Expr;
	};
	
};