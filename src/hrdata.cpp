#include "hrdata/hrdata.h"

#include <sstream>
#include <fstream>

namespace hrdata {
	
Node parse(std::istream& stream) {
	Parser p(stream);
	Node n = p.getValue();
	if (!p.isEnd()) {
		throw Exception("hrdata:: File parsed, however end of file is not reached");
	}
	return n;
}


Node parse(const std::string& string) {
	std::stringstream ss(string);
	return parse(ss);
}

Node parseFile(const std::string& file) {
	std::ifstream fs(file.c_str());
	if (!fs.is_open()) {
		std::stringstream ss;
		ss << "Can't open file '" << file << "' for reading";
		throw Exception(ss.str().c_str());
	}
	return parse(fs);
}

} // namespace
