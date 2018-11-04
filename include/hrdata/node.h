#ifndef HRDATA_NODE_H
#define HRDATA_NODE_H

#include "hrdata/exception.h"
#include "hrdata/types.h"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>

namespace hrdata {


struct Node {
	Node(NodeType type = EMPTY_NODE);
	
	template <typename T>
	Node(const T& value);
	
	Node(const Node& rhs);
	
	NodeType type() const;
	
	void reset(NodeType type = EMPTY_NODE);
	
	void reparent(Node* parent);

	Node& operator=(NodeType type);
	
	template <typename T>
	Node& operator=(const T& value);
	
	Node& operator=(const Node& rhs);
	
	// list part
	
	size_t elems() const;
	
	bool exist(size_t index) const;
	
	const Node& operator[](size_t index) const;
	Node& operator[](size_t index);
	
	Node& operator()(size_t index);
	
	// map part
	
	size_t keys() const;
	const std::string& key(size_t index) const;
	
	bool exist(const std::string& key) const;
	
	const Node& operator[](const std::string& key) const;
	Node& operator[](const std::string& key);
	
	Node& operator()(const std::string& key);
	
	// string part
	
	// check is path to string exists and return its value
	template <typename T>
	operator T() const;
	
	template <typename T>
	T as() const;
	
	// go back to parent 
	
	bool isRoot() const;
	
	const Node& up() const; 
	
	Node& up(); 
	
	std::string path() const;
	
	// emitter
	void emit(std::ostream& os, size_t depth = 0) const;
	
public:
	
	bool test() const;
	
	Node* m_parent;
	
	NodeType m_type;
	
	typedef std::vector<std::string> MapKeys;
	typedef std::map<std::string, Node> Map;
	
	MapKeys m_map_keys;
	Map m_map;
	
	typedef std::vector<Node> List;
	List m_list;
	
	std::string m_string;
};

template <typename T>
Node::Node(const T& value) :
	m_parent(NULL),
	m_type(STRING_NODE)
{
	assert(test());
	std::ostringstream ss;
	ss << value;
	m_string = ss.str();
	assert(test());
}

template <typename T>
Node& Node::operator=(const T& rhs)
{
	assert(test());
	reset(STRING_NODE);
	m_string = toString(rhs);
	assert(test());
	return *this;
}

template <typename T>
T Node::as() const 
{
	assert(test());
	requireNodeType(STRING_NODE, m_type, path());
	std::istringstream ss(m_string);
	T value;
	ss >> value;
	if (ss.fail() && !ss.eof()) {
		throw ConversionError(m_string, toString(value)); 
	}
	return value;
}

template <>
inline std::string Node::as<std::string>() const
{
	assert(test());
	requireNodeType(STRING_NODE, m_type, path());
	return m_string;
}

template <typename T>
Node::operator T() const
{
	return as<T>();
}

} // namespace

#endif
