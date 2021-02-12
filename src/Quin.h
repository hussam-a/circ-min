/*
Part of circ-min (github.com/hussam-a/circ-min)
Authors:
Hussam A. El-Araby (github.com/hussam-a)
Islam Faisal (github.com/decltypeme)

Project code and configurations are under the GNU GPL license (circ-min/LICENSE)
*/

#ifndef QUIN_H
#define	QUIN_H

#include "Header.h"

class Implicant {
public:
    static bool canJoin(const Implicant& _impl_a, const Implicant& _impl_b);
    explicit Implicant(const int& _size);
    explicit Implicant(const int& _minterm, uint8_t _bitsAll);
    Implicant(const Implicant& orig);
    Implicant(const Implicant& a, const Implicant& b);
    bool operator<(const Implicant& foreign) const;
    void join(const Implicant& _foreign);
    string to_string(void) const;

    /**
     * Ones and dashes count
     * @return 
     */
    int getRank(void) const {
        return bit_string[0].count();
    }

    int cost(void) {
        return bitsCountLeading - bit_string[1].count();
    }

    operator string(void) const {
        return to_string();
    };
protected:
    bitset bit_string[2];
    uint8_t bitsCountLeading;
};


namespace std {
    template<>
    bool operator<(const vector<Implicant>& a, const vector<Implicant>& b);
}

class Quin {
public:
    explicit Quin(istream& input, ostream& output, ostream& detailedLog);
    void reset();
    void resetRunPrint(void);
protected:
    void generatePrimes(void);
    void findEssentialPrimes(void);
    void printResults(void) const;
    map<int, map<bitset, Implicant>> implicant_list;
    map<bitset, bool> taken;
    map<bitset, Implicant> primes;
    map<bitset, Implicant> essentialPrimes;
    bitset minterms, dontcare, all;
    uint8_t varsCount;
    ostream& output;
    ostream& log;
};

#endif
