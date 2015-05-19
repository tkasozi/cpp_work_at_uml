/* 
 * File:   main.cpp Assignment 5
 * Author: Talik Kasozi
 *
 * Created on November 6, 2014, 11:33 AM
 */

#include "Assign5.hpp"

using namespace std;


/*
 * 
 */
int main(int argc, char *argv[3]) {
    stringstream ss;
    string policy, file;
    int mem;

    if (argc < 3) {
        cout << "Provide arguments: ";
        cout << "policy, total memory, file w/ data" << endl;
        exit(2);
    }

    ss << argv[1] << " " << argv[2] << " " << argv[3];

    ss >> policy >> mem >> file;

    if (policy.compare("first") == 0) {

        first_Fit(mem, file);

    } else if (policy.compare("best") == 0) {
        
        best_Fit(mem, file);

    } else if (policy.compare("buddy") == 0) {
        
         buddySys(mem, file);

    } else {
        cout << "Policy not found." << endl;
        exit(2);
    }
    return 0;
}