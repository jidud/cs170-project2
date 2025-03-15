// g++ -std=c++17 main.cpp -o main

#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main() {
    int choice{};
    string fileName = "";

    cout << "Welcome to Jenny Lee's Feature Selection Algorithm." << endl;
    cout << "Type in the name of the file to test: ";
    cin >> fileName;
    cout << "Type the number of the algorithm you want to run." << endl;
    cout << "   1) Forward Selection" << endl;
    cout << "   2) Backwards Elimination" << endl;
    cin >> choice;
    cout << endl;

    // open file
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error opening the file!" << endl;
        return 1;
    }

    vector<vector<double>> data;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<double> row;
        double val;

        while (ss >> val) {
            row.push_back(val); 
        }
        data.push_back(row);
    }
    file.close();

    cout << "This dataset has " << data[0].size() - 1 << " features (not including the class attribute), with " << data.size() << " instances." << endl << endl;

    if (choice == 1) {
        // Forward Selection
    } 
    else if (choice == 2) {
        // Backwards Elimination
    }

    return 0;
}
