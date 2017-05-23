#include "hrdata/node.h"
#include "hrdata/lexer.h"

namespace hrdata {

Node::Node(const Node& rhs) :
	m_parent(NULL),
	m_type(rhs.m_type),
	m_map_keys(rhs.m_map_keys),
	m_map(rhs.m_map),
	m_list(rhs.m_list),
	m_string(rhs.m_string)
{
	assert(test());
}

Node::Node(NodeType type) :
	m_parent(NULL),
	m_type(type)
{
}

void Node::reset(NodeType type)
{
	assert(test());
	m_type = type;
	m_map_keys.clear();
	m_map.clear();
	m_list.clear();
	m_string.clear();
	assert(test());
}

NodeType Node::type() const
{
	return m_type;
}

Node& Node::operator=(const Node& rhs)
{
	assert(test());
	m_type = rhs.m_type;
	m_map_keys = rhs.m_map_keys;
	m_map = rhs.m_map;
	m_list = rhs.m_list;
	m_string = rhs.m_string;
	assert(test());
	return *this;
}

Node& Node::operator=(NodeType type)
{
	assert(test());
	reset(type);
	assert(test());
	return *this;
}

// list part

size_t Node::elems() const
{
	assert(test());
	requireNodeType(LIST_NODE, m_type, path());
	return m_list.size();
}

bool Node::exist(size_t index) const
{
	assert(test());
	requireNodeType(LIST_NODE, m_type, path());
	return index < m_list.size();
}

const Node& Node::operator[](size_t index) const {
	return (*const_cast<Node*>(this))[index];
}

Node& Node::operator[](size_t index) {
	assert(test());
	requireNodeType(LIST_NODE, m_type, path());
	requireIndex(index, m_list.size(), path());
	return m_list.at(index);
}

Node& Node::operator()(size_t index)
{
	assert(test());
	if (m_type != LIST_NODE) reset(LIST_NODE);
	if (index >= m_list.size()) {
		m_list.resize(index+1);
		for (size_t i = 0; i != m_list.size(); ++i) {
			m_list.at(i).m_parent = this;
		}
	}
	assert(test());
	return m_list.at(index);
}

size_t Node::keys() const
{
	assert(test());
	return m_map_keys.size();
}

const std::string& Node::key(size_t index) const
{
	assert(test());
	requireIndex(index, m_map_keys.size(), path());
	return m_map_keys.at(index);
}

bool Node::exist(const std::string& key) const
{
	assert(test());
	requireNodeType(MAP_NODE, m_type, path());
	Map::const_iterator it = m_map.find(key);
	return it != m_map.end();

}

const Node& Node::operator[](const std::string& key) const {
	return (*const_cast<Node*>(this))[key];
}

Node& Node::operator[](const std::string& key)
{
	assert(test());
	requireNodeType(MAP_NODE, m_type, path());
	Map::iterator it = m_map.find(key);
	if (it == m_map.end()) throw NodeNotFound(key, path());
	return it->second;
}

Node& Node::operator()(const std::string& key)
{
	assert(test());
	if (m_type != MAP_NODE) reset(MAP_NODE);
	// if key already present, the isertion won't take place and it.pair->second will be false
	std::pair<Map::iterator, bool> pair = m_map.insert(std::make_pair(key, Node()));
	if (pair.second) m_map_keys.push_back(key); // if insertion occured, add key to map
	Node& n = pair.first->second;
	n.m_parent = this;
	assert(test());
	return n;
}

bool Node::isRoot() const
{
	return m_parent == NULL;
}

Node& Node::up()
{
	if (isRoot()) {
		throw Exception("Can't take parent node of root");
	}
	return *m_parent;
}

const Node& Node::up() const
{
	return const_cast<Node*>(this)->up();
}

std::string Node::path() const
{
	if (isRoot()) return "";
	const Node& u = up();
	switch (u.type()) {
		case (MAP_NODE):
			for (size_t i = 0; i < u.keys(); ++i) {
				const Node* temp = &u[u.key(i)];
				if (temp == this) {
					return u.path() + toString(TT_BRACE_LEFT) + 
						escapeString(u.key(i)) + toString(TT_BRACE_RIGHT);
				}
			}
		case (LIST_NODE):
			for (size_t i = 0; i < u.elems(); ++i) {
				const Node* temp = &u[i];
				if (temp == this) {
					return u.path() + toString(TT_BRACK_LEFT) + 
						toString(i) + toString(TT_BRACK_RIGHT);
				}
			}
			
		default:
			assert(0);
	}
}

void Node::emit(std::ostream& os, size_t depth) const {
	assert(test());
	
	switch (m_type) {
		case EMPTY_NODE:
			os << toString(TT_EMPTY) << std::endl;
			break;
		case STRING_NODE:
			escapeString(m_string, os);
			os << std::endl;
			break;
		case MAP_NODE:
			os << toString(TT_BRACE_LEFT) << std::endl;
			for (size_t k = 0; k != keys(); ++k) {
				os << std::string(depth + 1, '\t');
				escapeString(key(k), os); 
				os << ' ' << toString(TT_COLON) << ' ';
				(*this)[key(k)].emit(os, depth + 1);
			}
			os << std::string(depth, '\t') << toString(TT_BRACE_RIGHT) << std::endl;
			break;
		case LIST_NODE:
			os << toString(TT_BRACK_LEFT) << std::endl;
			for (List::const_iterator it = m_list.begin(); it != m_list.end(); ++it) {
				os << std::string(depth + 1, '\t');
				it->emit(os, depth + 1);
			}
			os << std::string(depth, '\t') << toString(TT_BRACK_RIGHT) << std::endl;
			break;
		default:
			throw Exception("hrdata::emit:: Unknown node type");
	}
}

bool Node::test() const
{
	switch (m_type) {
		case EMPTY_NODE:
			return m_list.empty() && m_string.empty() && m_map.empty() && m_map_keys.empty();
		case LIST_NODE:
			return m_string.empty() && m_map.empty() && m_map_keys.empty();
		case MAP_NODE:
			return m_list.empty() && m_string.empty() && (m_map.size() == m_map_keys.size());
		case STRING_NODE:
			return m_list.empty() && m_map.empty() && m_map_keys.empty();
		default:
			return false;
	}
}

} // namespace
