#ifndef HRDATA_H
#define HRDATA_H

#include "hrdata/exception.h"
#include "hrdata/lexer.h"
#include "hrdata/node.h"
#include "hrdata/parser.h"
#include "hrdata/token.h"

namespace hrdata {

Node parse(std::istream& stream);

Node parse(const std::string& string);

Node parseFile(const std::string& file);

} // namespace

#endif