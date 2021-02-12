/*
Part of circ-min (github.com/hussam-a/circ-min)
Authors:
Hussam A. El-Araby (github.com/hussam-a)
Islam Faisal (github.com/decltypeme)

Project code and configurations are under the GNU GPL license (circ-min/LICENSE) 
*/

#include "Quin.h"

void readExpression(istream& is, bitset* minterms, bitset* donotcare, uint8_t* varsCount) {
    char temp = '0';
    long long number;
    bitset* currentSet = minterms;
    is >> number;
    *varsCount = number;
    cout << *varsCount << endl;
    if (is.fail() || int(*varsCount) > int(16))
        throw logic_error("Variables count could not be read or invalid");
    is >> temp;
    if (is.fail() || is.eof() || temp != ';')
        throw logic_error("Syntax error: expected ';'");
    _t_minterm maxValue = (pow(2, *varsCount) - 1);
    minterms->resize(maxValue + 1);
    donotcare->resize(maxValue + 1);
    is >> number;
    if (!is.eof()) {
        while (true) {
            if (is.fail())
                throw logic_error("Syntax error");
            if (number > maxValue)
                throw out_of_range("Invalid min-term value: given value is greater than maximum");
            currentSet->set(number);
            temp = '0';
            is >> temp;
            if (is.fail() || is.eof() || (temp != ';' && temp != ','))
                throw logic_error("Syntax error: expected ';'");
            if (temp == ',') {
                is >> number;
            } else if (temp == ';') {
                if (currentSet == donotcare)
                    break;
                else {
                    currentSet = donotcare;
                    is >> number;
                    if (is.eof())
                        break;
                }
            } else {
                throw logic_error("Unhandled weird exception");
            }
        }
    }
}

namespace std {

    template<>
    bool operator<(const vector<Implicant>& a, const vector<Implicant>& b) {
        bool azero = (a.size() == 0), bzero = (b.size() == 0);
        if (azero && !bzero)
            return true;
        else if (!azero && !bzero) {
            return (a[0] < b[0]);
        } else
            return false;
    }
}

int bit_string_length(const _t_minterm& num) {
    return ceil(log2(num + 1));
}

char toChar(const BIT& a) {
    switch (a) {
        case BIT::ZERO:
            return '0';
        case BIT::ONE:
            return '1';
        case BIT::DASH:
            return '-';
    }
}

BIT bit_type(bool a, bool b) {
    if (!a)
        return BIT::ZERO;
    else if (b)
        return BIT::DASH;
    else
        return BIT::ONE;
}

string mintermsCovering(const bitset& inSet) {
    string toReturn = "(";
    for (int i = 0; i < inSet.size(); i++) {
        if (inSet[i]) {
            if (toReturn[toReturn.length() - 1] != '(')
                toReturn += ',';
            toReturn += to_string(i);
        }
    }
    toReturn += ')';
    return toReturn;
}
