#include "hrdata/types.h"
#include "hrdata/exception.h"
#include "hrdata/lexer.h"

#include <string>

namespace hrdata {

const std::string g_spec_syms("{}[],:#'\" \f\n\r\t\v");

template <>
std::string toString(const NodeType& nt) {
	switch (nt) {
		case INVALID_NODE:
			return "invalid";
		case EMPTY_NODE:
			return "empty";
		case STRING_NODE:
			return "string";
		case MAP_NODE:
			return "map";
		case LIST_NODE:
			return "list";
		default:
			return "unknown";
	}
} 

template <>
std::string toString(const TokenType& tt) {
	switch (tt) {
		case TT_INVALID:
			return "invalid";
		case TT_END:
			return "end";
		case TT_BRACE_LEFT:
			return "{";
		case TT_BRACE_RIGHT:
			return "}";
		case TT_BRACK_LEFT:
			return "[";
		case TT_BRACK_RIGHT:
			return "]";
		case TT_COMMA:
			return ",";
		case TT_COLON:
			return ":";
		case TT_EMPTY:
			return "#";
		case TT_ID:
			return "id";
		default:
			return "undefined";
	}
}
	
void requireNodeType(NodeType accessed, NodeType available, const std::string& path) {
	if (accessed != available) throw NodeMismatch(accessed, available, path);
}

void requireIndex(size_t accessed, size_t present, const std::string& path)
{
	if (accessed >= present) throw OutOfRange(accessed, present, path);
}

std::string escapeString(const std::string& str)
{
	std::ostringstream ss;
	escapeString(str, ss);
	return ss.str();
}

void escapeString(const std::string& str, std::ostream& os)
{
	bool not_found = str.find_first_of(g_spec_syms) == std::string::npos;
	bool not_found1 = str.find_first_of('\'') == std::string::npos;
	bool not_found2 = str.find_first_of('"') == std::string::npos;
	
	const char c1 = '\'';
	const char c2 = '"';
	
	if (not_found && !str.empty()) {
		os << str;
	} else if (not_found1) {
		os << c1 << str << c1;
	} else if (not_found2) {
		os << c2 << str << c2;
	} else {
		os << c1;
		for (size_t i = 0; i != str.size(); ++i) {
			if (str[i] == c1) os << c1;
			os << str[i];
		}
		os << c1;
	}
}

//std::string toString(const Path& path) {
//	std::stringstream ss;
//	for (Path::iterator it = path.begin(); it != path.end(); ++it) {
//		if (it->m_is_list) {
//			ss << "#" << it->m_index;
//		} else {
//			ss << "/";
//			if (it->m_name.find('/') == std::string::npos) {
//				ss << it->m_name;
//			} else {
//				for (size_t i = 0; i != it->m_name.size(); ++i) {
//					if (it->m_name.at(i) == '/') {
//						ss << '/';
//					}
//					ss << it->m_name.at(i);
//				}
//			}
//		}
//	}
//	return ss.str();
//}

} // namespace