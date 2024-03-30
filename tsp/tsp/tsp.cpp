#include "localsolver.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>

using namespace localsolver;
using namespace std;

class Tsp {
public:
    // Number of cities
    int nbCities;

    // Vector of distance between two cities
    vector<vector<int>> distMatrixData;

    // LocalSolver
    LocalSolver localsolver;

    // Decision variables
    LSExpression cities;

    // Objective
    LSExpression obj;

    /* Read instance data */
    void readInstance(const string& fileName) {
        ifstream infile;
        infile.exceptions(ifstream::failbit | ifstream::badbit);
        infile.open(fileName.c_str());

        // The input files follow the TSPLib "explicit" format
        string str;
        char* pch;
        char* line;

        while (true) {
            getline(infile, str);
            line = strdup(str.c_str());
            pch = strtok(line, " :");
            if (strcmp(pch, "DIMENSION") == 0) {
                getline(infile, str);
                line = strdup(str.c_str());
                pch = strtok(NULL, " :");
                nbCities = atoi(pch);
            } else if (strcmp(pch, "EDGE_WEIGHT_SECTION") == 0) {
                break;
            }
        }

        // Distance from i to j
        distMatrixData.resize(nbCities);
        for (int i = 0; i < nbCities; ++i) {
            distMatrixData[i].resize(nbCities);
            for (int j = 0; j < nbCities; ++j) {
                infile >> distMatrixData[i][j];
            }
        }
    }

    void solve(int limit) {
        // Declare the optimization model
        LSModel model = localsolver.getModel();

        // A list variable: cities[i] is the index of the ith city in the tour
        cities = model.listVar(nbCities);

        // All cities must be visited
        model.constraint(model.count(cities) == nbCities);

        // Create a LocalSolver array in order to be able to access it with "at" operators
        LSExpression distMatrix = model.array();
        for (int i = 0; i < nbCities; ++i) {
            LSExpression row = model.array(distMatrixData[i].begin(), distMatrixData[i].end());
            distMatrix.addOperand(row);
        }

        // Minimize the total distance
        LSExpression distLambda =
            model.createLambdaFunction([&](LSExpression i) { return model.at(distMatrix, cities[i - 1], cities[i]); });
        obj = model.sum(model.range(1, nbCities), distLambda) + model.at(distMatrix, cities[nbCities - 1], cities[0]);

        model.minimize(obj);

        model.close();

        // Parametrize the solver
        localsolver.getParam().setTimeLimit(limit);

        localsolver.solve();
    }

    /* Write the solution in a file */
    void writeSolution(const string& fileName) {
        ofstream outfile;
        outfile.exceptions(ofstream::failbit | ofstream::badbit);
        outfile.open(fileName.c_str());

        outfile << obj.getValue() << endl;
        LSCollection citiesCollection = cities.getCollectionValue();
        for (int i = 0; i < nbCities; ++i) {
            outfile << citiesCollection[i] << " ";
        }
        outfile << endl;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: tsp inputFile [outputFile] [timeLimit]" << endl;
        return 1;
    }

    const char* instanceFile = argv[1];
    const char* solFile = argc > 2 ? argv[2] : NULL;
    const char* strTimeLimit = argc > 3 ? argv[3] : "5";
    try {
        Tsp model;
        model.readInstance(instanceFile);
        model.solve(atoi(strTimeLimit));
        if (solFile != NULL)
            model.writeSolution(solFile);
        return 0;
    } catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
        return 1;
    }
}
