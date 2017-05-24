#ifndef HRDATA_EXCEPTION_H
#define HRDATA_EXCEPTION_H

#include <stdexcept>

#include "hrdata/types.h"

namespace hrdata {

class Exception : public std::runtime_error {
public:
	
	explicit Exception(const std::string& message) :
		std::runtime_error("hrdata::Exception::" + message)
	{
	}
};

class NodeNotFound : public Exception {
public:
	
	explicit NodeNotFound(const std::string key, const std::string& path) :
		Exception("NodeNotFound::Map doesn't have node with key: " + key + ". " + "Path: " + path)
	{
	}
	
};

class OutOfRange : public Exception {
public:
	explicit OutOfRange(size_t accessed, size_t available, const std::string& path) :
		Exception("OutOfRange::Indices must be less than " + toString(available) +
			toString(", but accessed index is ") + toString(accessed) + ". " + 
			"Path: " + path)
	{
	}
	
};

class NodeMismatch : public Exception {
public:
	
	NodeMismatch(/*const Path& path, */NodeType accessed, NodeType available, const std::string& path) :
		Exception(
			toString("NodeMismatch::") + /*Path: " + toString(path) + ", "*/
			"Access to " + toString(available) + " as if it is " + toString(accessed) + ". " + 
			"Path: " + path)
	{
		
	}
};

class ConversionError : public Exception
{
public:
	ConversionError(const std::string before, const std::string after) :
		Exception(
			"ConversionError::Value before conversion: <" + before + ">, " +
			"value after conversion: <" + after + ">") 
	{
	}
};

} // namespace

#endif