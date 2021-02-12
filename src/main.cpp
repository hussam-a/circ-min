/*
Part of circ-min (github.com/hussam-a/circ-min)
Authors:
Hussam A. El-Araby (github.com/hussam-a)
Islam Faisal (github.com/decltypeme)

Project code and configurations are under the GNU GPL license (circ-min/LICENSE) 
*/

#include "Quin.h"
#include <fstream>
using namespace std;

int main() {
    ifstream input("input1.txt");
    ofstream output("output.txt");
    ofstream log("log.txt");
    
    try {
        if (!input.fail() && !output.fail() && !log.fail()) {
            Quin testQuin(input, output, log);
            testQuin.resetRunPrint();
        }
    } catch (const exception& e) {
        cout << "Unhandled Exception: " << e.what() << endl;
        cout << "Program terminating...." << endl;
    }
    input.close();
    output.close();
    log.close();
    return 0;
}
