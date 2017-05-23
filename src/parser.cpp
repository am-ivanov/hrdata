#include "hrdata/parser.h"

namespace hrdata {

void Parser::getTok()
{
	Token tok = m_lexer.getToken();
	
	if (tok.getType() != TT_END) {
		m_tokens.push_back(tok);
	} else {
		m_end = true;
	}
}

bool Parser::reserve(size_t num) {
	if (num <= m_tokens.size()) return true;
	num -= m_tokens.size();
	while (true) {
		if (num == 0) return true;
		getTok();
		if (m_end) return false;
		--num;
	}
}

void Parser::drop(size_t num) {
	if (!reserve(num)) {
		throw Exception("hrdata::Parser:: Unexpected end of file");
	}
	for (size_t i = 0; i < num; ++i) {
		m_prev_tok_line = m_tokens.front().getLine();
		m_tokens.pop_front();
	}
}

Token Parser::get(size_t num)
{
	if (reserve(num+1)) {
		std::list<Token>::iterator it = m_tokens.begin();	
		assert(it != m_tokens.end());
		for (size_t i = 0; i < num; ++i) {
			++it;
			assert(it != m_tokens.end());
		}
		return *it;
	} else {
		throw Exception("hrdata::Parser:: Unexpected end of file");
	}
}
	
Node Parser::getValue() {
	Token tt = get(0);
	switch (tt.getType()) {
		case TT_EMPTY:
			drop(1);
			return Node(EMPTY_NODE);
		case TT_BRACE_LEFT:
			return getMap();
		case TT_BRACK_LEFT:
			return getList();
		case TT_ID:
			drop(1);
			return Node(tt.getId());
		default: {
			std::stringstream ss;
			ss << "hrdata::Parser:: Unexpected token <" << toString(tt.getType()) << "> ";
			ss << "(" << tt.getLine() << "," << tt.getPos() << ")"; 
			throw Exception(ss.str().c_str());
		}
	}
}

Node Parser::getMap() {
	get(0).require(TT_BRACE_LEFT);
	drop(1);
	
	Node map = MAP_NODE;
	
	if (get(0).getType() == TT_BRACE_RIGHT) {
		drop(1);
		return map;
	}
	
	while (true) {
		get(0).require(TT_ID);
		std::string key = get(0).getId();
		if (map.exist(key)) {
			std::stringstream ss;
			ss << "hrdata::Parser:: Identical keys in map are detected ";
			ss << "(" << get(0).getLine() << "," << get(0).getPos() << ")";
			throw Exception(ss.str().c_str());
		}
		drop(1);
		
		get(0).require(TT_COLON);
		drop(1);
		
		Node value = getValue();
		
		map(key) = value;
		
		// is delimiter present ?
		if (get(0).getType() == TT_COMMA) {
			drop(1);
		} else if (get(0).getType() == TT_BRACE_RIGHT) {
			drop(1);
			return map;
		} else if (get(0).getLine() != m_prev_tok_line) {
		} else {
			std::stringstream ss;
			ss << "hrdata::Parser:: Comma or end of line expected ";
			ss << "(" << get(0).getLine() << "," << get(0).getPos() << ")";
			throw Exception(ss.str().c_str());
		}
	}
}

Node Parser::getList() {
	get(0).require(TT_BRACK_LEFT);
	drop(1);
	
	Node list(LIST_NODE);
	
	if (get(0).getType() == TT_BRACK_RIGHT) {
		drop(1);
		return list;
	}
	
	size_t key = 0;
	while (true) {
		list(key++) = getValue();
		
		// is delimiter present ?
		if (get(0).getType() == TT_COMMA) {
			drop(1);
		} else if (get(0).getType() == TT_BRACK_RIGHT) {
			drop(1);
			return list;
		} else if (get(0).getLine() != m_prev_tok_line) {
		} else {
			std::stringstream ss;
			ss << "hrdata::Parser:: Comma or end of line expected ";
			ss << "(" << get(1).getLine() << "," << get(1).getPos() << ")";
			throw Exception(ss.str().c_str());
		}
	}
}

} // namespace
