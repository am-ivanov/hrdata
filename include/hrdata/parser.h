#ifndef HRDATA_PARSER_H
#define HRDATA_PARSER_H

#include <list>

#include "hrdata/lexer.h"
#include "hrdata/node.h"

namespace hrdata {

class Parser {
public:
	Parser() :
		m_end(false),
		m_prev_tok_line(0)
	{
	}
	
	explicit Parser(std::istream& in) :
		m_end(false),
		m_prev_tok_line(0)
	{
		setInput(in);
	}
	
	void setInput(std::istream& in) {
		m_end = false;
		m_prev_tok_line = 0;
		m_lexer.setInput(in);
	}
	
	bool isEnd() { 
		return !reserve(1) && m_end; 
	}
	
	Node getValue();
	Node getMap();
	Node getList();
	
private:
	
	void getTok();
	
	bool reserve(size_t num);
	
	void drop(size_t num);
	
	Token get(size_t num);
	
	bool m_end;
	size_t m_prev_tok_line;
	
	std::list<Token> m_tokens;
	
	Lexer m_lexer;
	
};

} // namespace

#endif