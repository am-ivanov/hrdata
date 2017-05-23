#include "hrdata/lexer.h"
#include "hrdata/exception.h"
#include "hrdata/types.h"

#include <sstream>
#include <cassert>

namespace hrdata {
	

void Symbol::require(char c) {
	if (c == m_char) return;
	std::stringstream ss;
	ss << "hrdata::Symbol:: Symbol <" << c << "> is expected, ";
	ss << "however <" << m_char << "> is present ";
	ss << "(" << m_line << "," << m_pos << ")";
	throw Exception(ss.str().c_str());
}
	
void Lexer::setInput(std::istream& expr)
{
	m_expr = &expr;
	m_end = false;
	m_next_sym_line = 1;
	m_next_sym_pos = 1;
}

void Lexer::getSym()
{
	if (m_expr == NULL) {
		throw Exception("hrdata::Lexer:: Input stream is not set");
	}

	Symbol last_sym;
	last_sym.m_line = m_next_sym_line;
	last_sym.m_pos = m_next_sym_pos;
	
	if (!m_expr->get(last_sym.m_char)) {
		if (m_expr->eof()) {
			m_end = 1;
		} else {
			std::stringstream ss;
			ss << "hrdata::Lexer:: Error occured while getting character ";
			ss << "(" << last_sym.m_line << "," << last_sym.m_pos << ")";
			throw Exception(ss.str().c_str());
		}
	} else {
		if (last_sym.m_char == '\n'){
			m_next_sym_line += 1;
			m_next_sym_pos = 1;
		} else {
			++m_next_sym_pos;
		}
		m_symbols.push_back(last_sym);
	}
}

bool Lexer::reserve(size_t num) {
	if (num <= m_symbols.size()) return true;
	num -= m_symbols.size();
	while (true) {
		if (num == 0) return true;
		getSym();
		if (m_end) return false;
		--num;
	}
}

Symbol Lexer::get(size_t num)
{
	if (reserve(num+1)) {
		std::list<Symbol>::iterator it = m_symbols.begin();	
		assert(it != m_symbols.end());
		for (size_t i = 0; i < num; ++i) {
			++it;
			assert(it != m_symbols.end());
		}
		return *it;
	} else {
		throw Exception("hrdata::Lexer:: Unexpected end of file");
	}
}

void Lexer::drop(size_t num) {
	if (!reserve(num)) {
		throw Exception("hrdata::Lexer:: Unexpected end of file");
	}
	for (size_t i = 0; i < num; ++i) {
		m_symbols.pop_front();
	}
}

void Lexer::dropSpaces() {
	while (reserve(1)) {
		assert(!m_symbols.empty());
		if (std::isspace(get(0).m_char)) {
			drop(1);
		} else if (reserve(2) && get(0).m_char == '/' && get(1).m_char == '/') {
			dropShortComment();
		} else if (reserve(2) && get(0).m_char == '/' && get(1).m_char == '*') {
			dropLongComment();
		} else {
			break;
		}
	}
}

void Lexer::dropLongComment()
{
	get(0).require('/');
	get(1).require('*');
	drop(2);
	while (reserve(2)) {
		char c0 = get(0).m_char;
		char c1 = get(1).m_char;
		drop(1);
		if (c0 == '*' && c1 == '/') {
			drop(1);
			return;
		}
	}
	throw Exception("Unexpected end of file");
}

void Lexer::dropShortComment()
{
	get(0).require('/');
	get(1).require('/');
	drop(2);
	
	while (reserve(1)) {
		char c = get(0).m_char;
		if (c == '\n') break;
		drop(1);
	}
}

std::string Lexer::getId0() {
	std::string id;
	while (reserve(1)) {
		if (reserve(2) && get(0).m_char == '/' && get(1).m_char == '/') {
			dropShortComment();
		} else if (reserve(2) && get(0).m_char == '/' && get(1).m_char == '*') {
			dropLongComment();
		} else {
			char c = get(0).m_char;
			if (std::isspace(c) || g_spec_syms.find(c) != std::string::npos) {
				break;
			} else {
				id += c;
				m_symbols.pop_front();
			}
		}
	}
	return id;
}

std::string Lexer::getId1() {
	std::string id;
	
	get(0).require('\'');	
	drop(1);
		
	while (reserve(1)) {
		if (get(0).m_char == '\'')
		{
			if (reserve(2) && get(1).m_char == '\'') {
				// valid replacement '' -> '
				id += get(0).m_char;
				drop(2);
			} else {
				// this is last symbol
				drop(1);
				return id;
			}
		} else {
			// check is it comment
			if (reserve(2) && get(0).m_char == '/' && get(1).m_char == '/') {
				dropShortComment();
			} else if (reserve(2) && get(0).m_char == '/' && get(1).m_char == '*') {
				dropLongComment();
			} else {
				id += get(0).m_char;
				drop(1);
			}
		}
	}
	throw Exception("Unexpected end of file");
}

std::string Lexer::getId2() {
	std::string id;
	
	get(0).require('"');	
	drop(1);
		
	while (reserve(1)) {
		if (get(0).m_char == '"')
		{
			if (reserve(2) && get(1).m_char == '"') {
				// valid replacement "" -> "
				id += get(0).m_char;
				drop(2);
			} else {
				// this is last symbol
				drop(1);
				return id;
			}
		} else {
			id += get(0).m_char;
			drop(1);
		}
	}
	throw Exception("Unexpected end of file");
}

Token Lexer::getToken() {
	if (m_expr == NULL) {
		throw Exception("hrdata::Lexer:: Input stream is not set");
	}
	
	dropSpaces();
	
	if (!reserve(1)) {
		return Token(TT_END, m_next_sym_line, m_next_sym_pos);
	} else {
		char c = get(0).m_char; 
		size_t line = get(0).m_line; 
		size_t pos = get(0).m_pos; 
		switch (c) {
			case '{':
				drop(1);
				return Token(TT_BRACE_LEFT, line, pos);
			case '}':
				drop(1);
				return Token(TT_BRACE_RIGHT, line, pos);
			case '[':
				drop(1);
				return Token(TT_BRACK_LEFT, line, pos);
			case ']':
				drop(1);
				return Token(TT_BRACK_RIGHT, line, pos);
			case ',':
				drop(1);
				return Token(TT_COMMA, line, pos);
			case ':':
				drop(1);
				return Token(TT_COLON, line, pos);
			case '#':
				drop(1);
				return Token(TT_EMPTY, line, pos);
			case '\'':
				return Token(TT_ID, line, pos, getId1());
			case '"':
				return Token(TT_ID, line, pos, getId2());
			default:
				return Token(TT_ID, line, pos, getId0());
		}
	}
}
	
} // namespace
