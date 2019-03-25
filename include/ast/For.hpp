#pragma once

#include "ast/Base.hpp"

namespace Fuse {

	class ForAST : public ExprAST {
	public:
		ForAST(std::unique_ptr<ExprAST> _assign, std::unique_ptr<ExprAST> _cond, std::unique_ptr<ExprAST> _step,
		  std::unique_ptr<ExprAST> _body):
		  assign(std::move(_assign)), cond(std::move(_cond)), step(std::move(_step)), body(std::move(_body)) { ; }
		
		std::shared_ptr<Fuse::Object> Eval();
		TypeAST GetType();
	private:
		std::unique_ptr<ExprAST> assign, cond, step, body;
	};
	
};