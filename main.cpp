// g++ -std=c++17 main.cpp -o main

#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// https://www.geeksforgeeks.org/program-calculate-distance-two-points/
double euclideanDist(vector<double>& a, vector<double>& b, set<int>& features) {
    double sum = 0.0;
    for (int feature : features) {
        sum += pow(a[feature] - b[feature], 2);
    }
    return sqrt(sum);
}

// cross validation
double accuracy(vector<vector<double>>& data, set<int>& feature, int next) {
    set<int> selectedFeatures = feature;
    if (next != -1) {
        selectedFeatures.insert(next);
    }

    int correctlyClassified = 0;
    size_t dataSize = data.size();

    for (size_t i = 0; i < dataSize; i++) {
        vector<double> obj_to_classify = data[i];  
        int obj_label = obj_to_classify[0];

        double nearest_neighbor_distance = INFINITY;
        int nearest_neighbor_label = -1;

        for (size_t k = 0; k < dataSize; k++) {
            if (k != i) { 
                double distance = euclideanDist(obj_to_classify, data[k], selectedFeatures);
                if (distance < nearest_neighbor_distance) {
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_label = data[k][0]; 
                }
            }
        }
        if (obj_label == nearest_neighbor_label) {
            correctlyClassified++;
        }
    }
    return (double)correctlyClassified / dataSize;
}

void forwardSelection(vector<vector<double>>& data) {
    set<int> curr_features = {}; 
    set<int> best = {}; 
    double bestAccuracy = -1;

    bestAccuracy = accuracy(data, best, -1);

    cout << "Running nearest neighbor with no feature, using 'leave-one-out' evaluation, I get a default rate accuracy of " << bestAccuracy * 100 << "%" << endl << endl;

    bestAccuracy = -1;

    cout << "Beginning search." << endl << endl;

    for (int i = 1; i < (data[0].size()); ++i) {
        int featureToAdd = -1;
        double best_so_far_Acc = -1;

        for (int j = 1; j < (data[0].size()); ++j) {  
            if (curr_features.find(j) == curr_features.end()) {  
                double acc = accuracy(data, curr_features, j); 
                cout << "    Using feature(s) {";
                for (auto feature : curr_features) {
                    cout << feature << ", ";
                }
                cout << j << "} accuracy is " << acc * 100 << "%" << endl;
                if (acc > best_so_far_Acc) {
                    best_so_far_Acc = acc;
                    featureToAdd = j;
                }
            }
        }

        if (featureToAdd != -1) {
            curr_features.insert(featureToAdd);
            cout << "\nFeature set {";

            // add commas in between
            bool first = true; 
            for (auto f : curr_features) {
                if (!first) cout << ", ";
                cout << f;
                first = false;
            }

            cout << "} was best, accuracy is " << best_so_far_Acc * 100 << "%" << endl << endl;
        }

        if (best_so_far_Acc < bestAccuracy) {
            cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
        } 
        else {
            bestAccuracy = best_so_far_Acc;
            best = curr_features;
        }
    }

    cout << "Finished search!! The best feature subset is {";
    for (auto feature : best) {
        cout << feature;
    }
    cout << "} which has an accuracy of " << bestAccuracy * 100 << "%" << endl << endl;
}

void backwardSelection(vector<vector<double>>& data) { 
    set<int> curr_features = {};  
    set<int> best = {}; 
    int numFeatures = data[0].size() - 1;
    
    for (int i = 1; i <= numFeatures; ++i) {
        curr_features.insert(i);
    }

    double bestAccuracy = accuracy(data, curr_features, -1);
    cout << "Running nearest neighbor with all feature, accuracy is " << bestAccuracy * 100 << "%" << endl << endl;
    cout << "Beginning search." << endl << endl;

    for (int i = numFeatures; i > 0; --i) {  
        int featureToRemove = -1;
        double best_so_far_Acc = -1;

        for (int feature : curr_features) {
            set<int> temp = curr_features;
            temp.erase(feature);  // removing feature

            double acc = accuracy(data, temp, -1);

            cout << "    Using feature(s) {";
            bool first = true;
            for (auto f : temp) {
                if(!first) cout << ", ";
                cout << f;
                first = false;
            }
            cout <<"} accuracy is " << acc * 100 << "%" << endl;

            if (acc > best_so_far_Acc) {
                best_so_far_Acc = acc;
                featureToRemove = feature;
            }
        }

        if (featureToRemove != -1) {
            curr_features.erase(featureToRemove);
            cout << "\nFeature set {";

            bool first = true; 
            for (auto f : curr_features) {
                if (!first) cout << ", ";
                cout << f;
                first = false;
            }

            cout << "} was best, accuracy is " << best_so_far_Acc * 100 << "%" << endl << endl;
        }

        if (best_so_far_Acc < bestAccuracy) {
            cout << "(Warning, Accuracy has decreased! Continue search in case of local maxima)" << endl;
        } 
        else {
            bestAccuracy = best_so_far_Acc;
            best = curr_features;
        }
    }

  cout << "Finished search!! The best feature subset is {";
  bool first = true;
    for (auto feature : best) {
        if(!first) cout << ", ";
        cout << feature;
        first = false;
    }
    cout << "} which has an accuracy of " << bestAccuracy * 100 << "%" << endl << endl;
}

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
