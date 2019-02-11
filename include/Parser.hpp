#pragma once

#include "AST.hpp"
#include "Lexer.hpp"
#include "data/Fuse_Object.hpp"

namespace Fuse {
	
	enum Parser_State { PARSER_SUCCESS , PARSER_WARNING , PARSER_ERROR , PARSER_EOF };
	
	struct Parser {
		Parser(Fuse::Lexer* _l, VariableScopes* _scopes): lex(_l), Scopes(_scopes) { ; }
		
		Parser_State Parse();
	private:
		using VariableScopes = std::vector< std::map<std::string, std::shared_ptr<Fuse::Object>> >;
		VariableScopes * Scopes;
	
		int GetNextToken();
		int GetCurrentToken();
		int CurrTok;
	
		std::unique_ptr<StatAST> StatLogError(const std::string& );
		std::unique_ptr<ExprAST> ExprLogError(const std::string& );
		void LogWarning(const std::string&);
	
		std::unique_ptr<StatAST> ParseStatement();
		
		std::unique_ptr<StatAST> ParseFuncDef();
		std::unique_ptr<ExprAST> ParseLambdaDef();
		
		std::unique_ptr<StatAST> ParseBlock();
		std::unique_ptr<StatAST> ParseReturn();
		
		
		std::unique_ptr<ExprAST> ParseExpression();
		
		std::unique_ptr<ExprAST> ParseNumber();
		std::unique_ptr<ExprAST> ParseString();
		std::unique_ptr<ExprAST> ParseBoolean();
			
		Fuse::Lexer* lex;
	};

};