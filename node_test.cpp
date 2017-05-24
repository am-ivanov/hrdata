#include "hrdata/hrdata.h"

int main() {
	hrdata::Node n;
	
	n = hrdata::LIST_NODE;
	n[0];
	
	n = hrdata::MAP_NODE;
	n = "test";
	n = 10.0;
	
	n(10) = hrdata::LIST_NODE;
	n[10] = "test";
	n(10)(20)(30)("bc") = 10.0;
	
	hrdata::Node n1 = n(10)(20)(30);
	
	hrdata::Node& m = n(10);
	
	n(15) = 777;
	n(20) = n;
	
	std::string x = n[20][15];
	double y = n[20][15];
	
	hrdata::Node* n2 = &n[10];
	(*n2)[15] = n[10];
	
	int v = n[20][10][20][30]("bc");
	
	std::cout << v << std::endl;
	
}