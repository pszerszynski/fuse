#include "Lexer.hpp"

using namespace Fuse;

std::string Lexer::TokenToString(int tok) {
	switch (tok) {
	case TOK_EOF: 		return "TOK_EOF"; 
		
	case TOK_IF: 	return "TOK_IF";
	case TOK_ELSE: 	return "TOK_ELSE";
	case TOK_WHILE: return "TOK_WHILE";
	case TOK_DO: 	return "TOK_DO";
		
	case TOK_FOR: 	return "TOK_FOR";
	case TOK_IN:	return "TOK_IN";
	case TOK_NEW:	return "TOK_NEW";
		
	case TOK_SWITCH:	return "TOK_SWITCH";
	case TOK_CASE:		return "TOK_CASE";
		
	case TOK_FUNCTION:	return "TOK_FUNCTION";
	case TOK_RETURN:	return "TOK_RETURN";
		
	case TOK_USING:	return "TOK_USING";
	
	case TOK_IDENTIFIER:
		return "{TOK_IDENTIFIER: " + IdName + "}";
	case TOK_NUMBER:
		return "{TOK_NUMBER:" + (IsInt ? std::to_string(IntVal) : std::to_string(DoubleVal)) + "}";
	case TOK_OPERATOR:
		return "{TOK_OPERATOR:" + op_str[Operator] + "}";
	case TOK_STRING:
		return "{TOK_STRING:" + String + "}";
	default:
		return std::string("(") + (char)tok  + ")";
	}
}

int Lexer::SetReader(std::istream* _stream) {
	if (!_stream) return -1;
	
	if (stream) {
		stream->clear();
	}
	stream = _stream;
	stream_set = true;
	line = 1;
	char_count = 0;
	
	NextChar();
	
	// if stream is empty return error
	if (stream->eof()) {
		stream_set = false;
		return -1;
	}
	
	return 0;
}

void Lexer::SetOut(std::ostream* _ostream, std::string str) {
	text_prefix = str;
	ostream = _ostream;
	
	PrintTextPrefix();
}

void Lexer::PrintTextPrefix() {
	if (ostream == nullptr) return;
	*ostream << text_prefix << std::flush;
}

bool Lexer::IsStreamSet() {
	return stream_set;
}

bool Lexer::IsStreamEOF() {
	if (stream == nullptr)
		return true;
	return LastToken == TOK_EOF;
}

bool Lexer::IsPosEOF() {
	if (IsStreamEOF()) return true;
	if (stream->peek() == EOF) return true;
	return false;
}

int Lexer::GetLineCount() {
	return line;
}

int Lexer::GetCharCount() {
	return char_count;
}

inline void Lexer::NextChar() {
	C = stream->get();
	++char_count;
}

// Checks if a-z or A-Z or '_'
inline bool IdentifierChar(char c) {
	return (std::isalpha(c) || (c == '_'));
}
// Checks if its a character not part of any operators
inline bool EmittableChar(char  c) {
	return emit_chars.find(c) != std::string::npos;
}
// Checks if its a character part of an operator
inline bool OperatorChar(char  c) {
	return op_chars.find(c) != std::string::npos;
}

int Lexer::GetNextToken() {
	if (HeldToken != NO_TOK_HELD) {
		int held = HeldToken;
		HeldToken = NO_TOK_HELD;
		return held;
	}
	LastToken = Next();
	return LastToken;
}

int Lexer::PutBackToken() {
	HeldToken = LastToken;
}

PREUNARY_OPERATORS Fuse::GetPreunaryOp(OPERATORS op) {
	if (op < 14 || op >= OP_COUNT) return (PREUNARY_OPERATORS)-1;
	return (PREUNARY_OPERATORS)(op-14);
}

POSTUNARY_OPERATORS Fuse::GetPostunaryOp(OPERATORS op) {
	if (op < 14 || op >= 16) return (POSTUNARY_OPERATORS)-1;
	return (POSTUNARY_OPERATORS)(op-14);
}

int Lexer::Next() {
	if (stream->eof()) return TOK_EOF;
	
	// Ignore whitespaces, if new line increase line counter
	while (std::isspace(C)) {
		if (C == '\n') {
			PrintTextPrefix();
			line++;
			char_count = 0;
		}

		NextChar();
	}
	
	// Keywords  Identifiers -- Must start with a-z or A-Z or '_'
	if (IdentifierChar(C)) {
		return TokenizeKeywordId();
	}
	
	// Numbers
	// Check if '-' is a minus sign (therefore part of a number)
	// '-' is guaranteed to be part of a number if the next char is a number (or a .)
	// and the previous token is not a number/identifier/close bracket/postfix unary operator (++ and --)
	if (C == '-') {
		if ( (std::isdigit(stream->peek()) || (stream->peek() == '.') ) &&
		     ((LastToken != TOK_NUMBER) && (LastToken != TOK_IDENTIFIER) && (LastToken != ')')
		       && (LastToken != ']') && (LastToken != OP_INC) && (LastToken != OP_DEC)) )
		{
			return TokenizeNumber();
		}
	} // If 0-9 digit must be a number
	// ".9" also counts as a number so '.' is allowed
	// but only when the previous token isn't an identifier
	// because '.' can be used in "table.member" and
	// the next character has to be a digit
	else if (std::isdigit(C) || (C == '.' && LastToken != TOK_IDENTIFIER && std::isdigit(stream->peek()) )) {
		return TokenizeNumber();
	}
	
	// Special characters not part of any operators ;().
	if (EmittableChar(C)) {
		char last_char = C;
		NextChar();
		return last_char;
	}
	
	// Operators
	if (OperatorChar(C)) {
		return TokenizeOperator();
	}
	
	// Strings
	if (C == '\"') {
		return TokenizeString();
	}
	
	char last_char = C;
	NextChar();
	return last_char;
}

int Lexer::TokenizeKeywordId() {
	IdName = "";
		
	while (IdentifierChar(C) || std::isdigit(C)) {
		IdName += C;
		NextChar();
	}
		
	// Check if keyword
	if      (IdName == "if") 	return TOK_IF;
	else if (IdName == "else") 	return TOK_ELSE;
	else if (IdName == "true")	return TOK_TRUE;
	else if (IdName == "false")	return TOK_FALSE;
	else if (IdName == "function") 	return TOK_FUNCTION;
	else if (IdName == "return") 	return TOK_RETURN;
	else if (IdName == "for") 	return TOK_FOR;
	else if (IdName == "in") 	return TOK_IN;
	else if (IdName == "while") 	return TOK_WHILE;
	else if (IdName == "do") 	return TOK_DO;
	else if (IdName == "break")	return TOK_BREAK;
	else if (IdName == "continue")	return TOK_CONTINUE;
	else if (IdName == "new") 	return TOK_NEW;
	else if (IdName == "switch") 	return TOK_SWITCH;
	else if (IdName == "case") 	return TOK_CASE;
	else if (IdName == "using") 	return TOK_USING;
		
	// If not keyword it is an identifier
	else return TOK_IDENTIFIER;
}

int Lexer::TokenizeNumber() {
	std::string temp = "";
	IsInt = true;

	while (std::isdigit(C) // is 0-9
	       || (C == '.')   // is decimal dot
	       // checks whether it is first character and is a minus sign
	       || (C == '-' && temp == ""))
	{
		if (C == '.') IsInt = false;
		temp += C;
		
		NextChar();
	}
	
	if (IsInt) {
		IntVal = std::stoll(temp);
	} else {
			DoubleVal = std::stod(temp);
	}
	
	return TOK_NUMBER;
}

int Lexer::TokenizeOperator() {
	std::string temp = "";
	
	while (OperatorChar(C)) {
		temp += C;
		NextChar();
	}
	
	if (temp == "-") {
		if ((LastToken == TOK_NUMBER) || (LastToken == TOK_IDENTIFIER) || (LastToken == ')')
		       || (LastToken == ']') || (LastToken == OP_INC) || (LastToken == OP_DEC))
		{
			Operator = OP_SUB;
		} else {
			Operator = OP_NEGATE;    
		}
		return TOK_OPERATOR;
	}
	
	for (int i = 0; i < OP_COUNT; ++i) {
		if (temp == op_str[i]) {
			Operator = (OPERATORS)i;
			return TOK_OPERATOR;
		}
	}
	
	return TOK_ERROR;
}

int Lexer::TokenizeString() {
	String = "";
	NextChar(); // get rid of " char
	
	while (C != '\"') {
		String += C;
		NextChar();
	}
	
	NextChar(); // get rid of closing " char
	
	return TOK_STRING;
}