#ifndef HRDATA_TOKEN_H
#define HRDATA_TOKEN_H

#include <string>

#include "hrdata/exception.h"

namespace hrdata {

struct Token {
	Token() :
		m_type(TT_INVALID),
		m_line(0),
		m_pos(0)
	{}
		
	Token(
		TokenType tt,
		size_t line,
		size_t pos,
		std::string str = std::string()
	) :
		m_type(tt),
		m_line(line),
		m_pos(pos),
		m_id(str)
	{}
	
	TokenType getType() const {
		return m_type;
	}
	
	~Token() {}
	
	std::string getId() const {
		if (m_type != TT_ID) {
			throw Exception("hrdata::Token:: Trying to get ID from token without ID type");
		}
		return m_id;
	}
	
	size_t getLine() const {
		return m_line;
	}
	
	size_t getPos() const {
		return m_pos;
	}
	
	void require(TokenType c) const;
	
private:
	TokenType m_type;
	size_t m_line;
	size_t m_pos;
	std::string m_id;
};

} // namespace

#endif