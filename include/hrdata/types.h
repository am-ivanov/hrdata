#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <sstream>
#include <string>
#include <list>

namespace hrdata {

extern const std::string g_spec_syms;

template <typename T>
std::string toString(const T& value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

enum NodeType {
	INVALID_NODE,
	EMPTY_NODE,
	STRING_NODE,
	MAP_NODE,
	LIST_NODE
};

template <>
std::string toString(const NodeType& nt);

enum TokenType {
	TT_INVALID,
	TT_END,
	TT_BRACE_LEFT,
	TT_BRACE_RIGHT,
	TT_BRACK_LEFT,
	TT_BRACK_RIGHT,
	TT_COMMA,
	TT_EMPTY,
	TT_COLON,
	TT_ID
};

template <>
std::string toString(const TokenType& tt);

void requireNodeType(NodeType accessed, NodeType available, const std::string& path);
void requireIndex(size_t accessed, size_t available, const std::string& path);


std::string escapeString(const std::string& str);
void escapeString(const std::string& str, std::ostream& os);

//struct PathEntry {
//	PathEntry(const std::string& str) : m_is_list(false), m_name(str), m_index(0) {}
//	PathEntry(size_t ind) : m_is_list(true), m_index(ind) {}
//	
//	bool m_is_list; // true - list, false - map
//	std::string m_name;
//	size_t m_index;
//};
//
//typedef std::list<PathEntry> Path;

} // namespace

#endif