#include "hrdata/token.h"

#include <sstream>

namespace hrdata {

void Token::require(TokenType tt) const {
	if (tt == m_type) return;
	std::stringstream ss;
	ss << "hrdata::Token:: Token <" << toString(tt) << "> is expected, ";
	ss << "however <" << toString(m_type) << "> is present ";
	ss << "(" << m_line << "," << m_pos << ")";
	throw Exception(ss.str().c_str());
}



} // namespace