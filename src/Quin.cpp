/*
Part of circ-min (github.com/hussam-a/circ-min)
Authors:
Hussam A. El-Araby (github.com/hussam-a)
Islam Faisal (github.com/decltypeme)

Project code and configurations are under the GNU GPL license (circ-min/LICENSE) 
*/

#include "Quin.h"

bool Implicant::canJoin(const Implicant& _impl_a, const Implicant& _impl_b) {
    if ((_impl_a.bitsCountLeading) == (_impl_b.bitsCountLeading)) {
        auto a0 = _impl_a.bit_string[0];
        a0.resize((_impl_a.bitsCountLeading));
        auto a1 = _impl_a.bit_string[1];
        a1.resize((_impl_a.bitsCountLeading));
        auto b0 = _impl_b.bit_string[0];
        b0.resize((_impl_a.bitsCountLeading));
        auto b1 = _impl_b.bit_string[1];
        b1.resize((_impl_a.bitsCountLeading));
        auto xor1 = (a0 ^ b0);
        auto xor2 = (a1 ^ b1);
        return (max(xor1.count(), xor2.count()) == 1);
    } else {
        return false;
    }

}

void Implicant::join(const Implicant& _foreign) {
    int newBitSetSize = max(bit_string[0].size(), _foreign.bit_string[0].size());
    bitset* a = this->bit_string;
    bitset b[2] = _foreign.bit_string;
    a[0].resize(newBitSetSize);
    a[1].resize(newBitSetSize);
    b[0].resize(newBitSetSize);
    b[1].resize(newBitSetSize);
    auto x1 = (a[0] ^ b[0]);
    auto x2 = (a[1] ^ b[1]);
    if (max(x1.count(), x2.count()) > 1 || (bitsCountLeading) != (_foreign.bitsCountLeading))
        throw logic_error("Can not join these implicants");
    else {
        auto x3 = (x1 | x2);
        a[0] |= x3;
        a[1] |= x3;
    }

}

bool Implicant::operator<(const Implicant& foreign) const {
    return (bit_string[0].count() < foreign.bit_string[0].count());
}

Implicant::Implicant(const int& _minterm, uint8_t _bitsAll) : bitsCountLeading(_bitsAll) {
    int a = bit_string_length(_minterm);
    bit_string[0] = bitset(a, _minterm);
    bit_string[1].resize(bit_string[0].size());
}

Implicant::Implicant(const Implicant& orig) : bit_string {
    orig.bit_string[0], orig.bit_string[1]
}

, bitsCountLeading(orig.bitsCountLeading) {
}

Implicant::Implicant(const Implicant& a, const Implicant& b) : Implicant(a) {
    join(b);
}

string Implicant::to_string(void) const {
    string toReturn = "";
    for (int i = (bitsCountLeading - 1); i >= 0; i--) {
        if (i < bit_string[0].size())
            toReturn += toChar(bit_type(bit_string[0][i], bit_string[1][i]));
        else
            toReturn += toChar(ZERO);
    }
    return toReturn;
}

Quin::Quin(istream& _input, ostream& _output, ostream& _detailedLog) : output(_output), log(_detailedLog) {
    readExpression(_input, &minterms, &dontcare, &varsCount);
    all = minterms | dontcare;
}

void Quin::resetRunPrint(void) {
    reset();
    generatePrimes();
    findEssentialPrimes();
    printResults();
}

void Quin::reset(void) {
    log << "Now reseting the Quin object\n";
    for (int i = 0; i < all.size(); i++) {
        if (all[i]) {
            Implicant newImplicant(i, varsCount);
            implicant_list.insert(make_pair(newImplicant.getRank(), map<bitset, Implicant>()));
            bitset impCoverage(pow(2, varsCount));
            impCoverage.set(i, true);
            implicant_list.find(newImplicant.getRank())->second.insert(make_pair(impCoverage, newImplicant));
        }
    }
    log << "The initial list is\n"
            << "-------------------------------------------------------------------\n";
    for (auto x : implicant_list) {
        log << "Rank" << (x.second).begin()->second.getRank() << endl << "#########################\n";
        for (auto y : x.second) {
            log << mintermsCovering(y.first) << "\t" << y.second.to_string() << endl;
        }
    }
    log << "-------------------------------------------------------------------\n";
}

void Quin::generatePrimes(void) {
    bool flag = true;
    for (int level = 1; level <= varsCount && flag; level++) {
        //Initialize Taken
        for (auto x : implicant_list) {
            for (auto y : x.second) {
                taken.insert(make_pair(y.first, false));
            }
        }

        //////////////////////////
        map<int, map < bitset, Implicant >> temp;
        auto foo = implicant_list.end();
        if (implicant_list.size() != 0)
            foo--;
        for (decltype(implicant_list)::iterator x = implicant_list.begin(); x != (implicant_list.end()); x++) {
            for (auto y : (x->second)) {
                decltype(x) k = x;
                k++;
                if (x != foo) {
                    for (auto w : (k)->second) {
                        auto& a = y.second;
                        auto& b = w.second;
                        if (Implicant::canJoin(a, b)) {
                            Implicant c(a, b);
                            bitset covering(pow(2, varsCount));
                            covering = y.first | w.first;
                            temp.insert(make_pair(c.getRank(), map<bitset, Implicant>()));
                            temp.find(c.getRank())->second.insert(make_pair(covering, c));
                            (taken.find(y.first))->second = true;
                            (taken.find(w.first))->second = true;
                        }
                    }
                }
            }
        }

        if (!implicant_list.empty()) {
            log << "Level: " << level << "\n"
                    << "-------------------------------------------------------------------\n";
            for (auto x : implicant_list) {
                log << "Rank" << (x.second).begin()->second.getRank() << endl << "#########################\n";
                for (auto y : x.second) {
                    bool a = taken.find(y.first)->second;
                    log << mintermsCovering(y.first) << "\t" << y.second.to_string() << "\t" << ((a) ? "*" : "$") << endl;
                    if (!a)
                        primes.insert(make_pair(y.first, y.second));
                }
            }
            log << "-------------------------------------------------------------------\n";
        }
        implicant_list = temp;
        temp.clear();
        taken.clear();

    }
    log << "-------------------------------------------------------------------\n";
    log << "-------------------------------------------------------------------\n";
    log << "The generated prime implicants are\n";
    log << "-------------------------------------------------------------------\n";
    int x = 0;
    for (auto y : primes) {
        x++;
        log << setw(5) << "p" << to_string(x) << setw(20) << mintermsCovering(y.first) << setw(20) << y.second.to_string() << endl;
    }
    log << "-------------------------------------------------------------------\n";
    log << "-------------------------------------------------------------------\n";
}

void Quin::findEssentialPrimes(void) {
    int mintermsCount = minterms.count();
    vector < vector<bool>> adjMatrix;
    adjMatrix.resize(mintermsCount);
    int i = -1;
    for (int j = 0; j < int(pow(2,varsCount)); j++) {
        if (minterms[j]) {
            i++;
            adjMatrix[i].resize(primes.size(), false);
            int primeIndex = 0, count = 0;
            decltype(primes)::iterator coveredBy;
            for (decltype(primes)::iterator x = primes.begin(); x != primes.end(); x++) {
                if (x->first[j]) {
                    adjMatrix[i][primeIndex] = true;
                    count++;
                    coveredBy = x;
                }
                primeIndex++;
            }
            if (count <= 0 && (minterms.count() != (pow(2,varsCount) - 1)))
            {throw logic_error("Weird things happened: A minterm could not be covered");}
            else if (count == 1)
                essentialPrimes.insert(make_pair(coveredBy->first, coveredBy->second));
        }
    }
    log << "-------------------------------------------------------------------\n";
    log << "The adjacency matrix is\n";
    log << "-------------------------------------------------------------------\n";
    for (long long i = -1; i < (long long) (primes.size()); i++) {
            int k = -1;
        for (long long j = -1; j < (long long) (pow(2,varsCount)); j++) {
            if (i == -1 && j == -1) {
                log << setw(8) << " ";
            } else if (j == -1) {
                log << setw(8) << "p" << to_string(i + 1);
            } else if (i == -1) {
                if (minterms[j]) {
                    log << setw(8) << j;
                }
            } else {
                if (minterms[j]) {
                    k++;
                    log << setw(8) << ((adjMatrix[k][i]) ? "*" : "");
                }
            }
        }
        log << endl;
    }
    log << "-------------------------------------------------------------------\n";
    log << "The essential prime implicants are\n";
    log << "-------------------------------------------------------------------\n";
    for (auto y : essentialPrimes) {
        log << setw(20) << mintermsCovering(y.first) << setw(20) << y.second.to_string() << endl;
    }
    log << "-------------------------------------------------------------------\n";
    log << "-------------------------------------------------------------------\n";
}

/**void Quin::findEssentialPrimes(void) {
    for (int i = 0; i < minterms.size(); i++) {
        if (minterms[i]) {
            int count = 0;
            decltype(primes)::iterator coveredBy;
            for (decltype(primes)::iterator x=primes.begin();x!= primes.end() ; x++) {
                if ((x->first)[i]) {
                    count++;
                    coveredBy = x;
                }
            }
            if (count <= 0)
                throw logic_error("Weird things happened: A minterm could not be covered");
            else if (count == 1) {
                essentialPrimes.insert(make_pair(coveredBy->first, coveredBy->second));
            }
        }
    }
 log << "-------------------------------------------------------------------\n";
    log << "The essential prime implicants are\n";
    log << "-------------------------------------------------------------------\n";
    for (auto y : essentialPrimes) {
        log << setw(20) << mintermsCovering(y.first) << setw(20) << y.second.to_string() << endl;
    }
    log << "-------------------------------------------------------------------\n";
    log << "-------------------------------------------------------------------\n";
}
 * */

void Quin::printResults(void) const {
}
