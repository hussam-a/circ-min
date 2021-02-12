/*
Part of circ-min (github.com/hussam-a/circ-min)
Authors:
Hussam A. El-Araby (github.com/hussam-a)
Islam Faisal (github.com/decltypeme)

Project code and configurations are under the GNU GPL license (circ-min/LICENSE) 
*/

#ifndef HEADER_H
#define	HEADER_H

#include <boost/range/irange.hpp>
#include <boost/dynamic_bitset.hpp>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <cmath>
#include <map>
#include <iomanip>

using namespace std;

typedef boost::dynamic_bitset<> bitset;
typedef uint16_t _t_minterm;

enum BIT {
    ZERO,
    ONE,
    DASH
};

BIT bit_type(bool a, bool b);

char toChar(const BIT& a);

int bit_string_length(const _t_minterm& num);

void readExpression(istream& is, bitset* minterms, bitset* donotcare, uint8_t* varsCount);

string mintermsCovering(const bitset& inSet);
#endif	/* HEADER_H */

