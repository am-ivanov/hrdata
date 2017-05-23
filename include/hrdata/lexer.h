#ifndef HRDATA_LEXER_H
#define HRDATA_LEXER_H

#include "hrdata/exception.h"
#include "hrdata/token.h"

#include <string>
#include <list>
#include <set>

namespace hrdata {



struct Symbol {
	Symbol() :
		m_line(0),
		m_pos(0),
		m_char(0)
	{}
	
	void require(char c);
	
	size_t m_line;
	size_t m_pos;
	char m_char;
};

class Lexer {
public:
	Lexer() : 
		m_expr(NULL)
	{}
	
	explicit Lexer(std::istream& expr) :
		m_expr(NULL)
	{
		setInput(expr);
	}
	
	void setInput(std::istream& expr);
	
	Token getToken();
	
private:
	// functions
	
	void getSym();
	
	bool reserve(size_t num);
	
	void drop(size_t num);
	
	Symbol get(size_t num);
	
	void dropShortComment();
	
	void dropLongComment();
	
	void dropSpaces();
	
	// single word
	std::string getId0();
	
	// line inside ' ', delete comments
	std::string getId1();
	
	// line inside " ", with comments
	std::string getId2();
	
	// variables
	
	
	bool m_end;
	
	size_t m_next_sym_line;
	size_t m_next_sym_pos;
	std::list<Symbol> m_symbols;
	
	std::istream* m_expr;
};

} // namespace

#endif