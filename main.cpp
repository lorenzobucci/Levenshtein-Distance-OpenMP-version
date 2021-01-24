#include <iostream>
#include <chrono>
#include <random>
#include "LevenshteinDistance.h"

using namespace std;

int main() {

    // GENERATING STRINGS

    int stringLengthA = 61367;
    int stringLengthB = 62789;

    string str("abcdefghijklmnopqrstuvwxyz");

    mt19937 generator{random_device{}()};
    uniform_int_distribution<int> distribution{'a', 'z'};

    string a(stringLengthA, '\0');
    for(auto& dis: a)
        dis = distribution(generator);
    string b(stringLengthB, '\0');
    for(auto& dis: b)
        dis = distribution(generator);

    // SEQUENTIAL ALGORITHM

    auto **ptr = new unsigned short *[stringLengthA + 1];
    for (int i = 0; i <= stringLengthA; i++)
        ptr[i] = new unsigned short[stringLengthB + 1];

    cout << "Starting sequential algorithm..." << endl;
    auto startTime = chrono::high_resolution_clock::now();
    unsigned short sequentialResult = sequentialAlgorithm(a, b, ptr);
    auto endTime = chrono::high_resolution_clock::now();

    chrono::duration<float> timeElapsed = endTime - startTime;
    cout << "Sequential edit distance result: " << sequentialResult << endl;
    cout << "Sequential execution time in s : " << timeElapsed.count() << endl;

    for (int i = 0; i <= stringLengthA; i++)
        delete ptr[i];
    delete[] ptr;

    // PARALLEL ALGORITHM

    for (unsigned short subMatrixSize = 10; subMatrixSize <= 1000; subMatrixSize+=100) {
        ptr = new unsigned short *[stringLengthA + 1];
        for (int i = 0; i <= stringLengthA; i++)
            ptr[i] = new unsigned short[stringLengthB + 1];

        cout << endl << "Starting parallel algorithm..." << endl;
        startTime = chrono::high_resolution_clock::now();
        unsigned short parallelResult = parallelAlgorithm(a, b, subMatrixSize, ptr);
        endTime = chrono::high_resolution_clock::now();

        timeElapsed = endTime - startTime;
        cout << "Parallel edit distance result: " << parallelResult << endl;
        cout << "Parallel execution time in s: " << timeElapsed.count() << " with sub-matrices size: " << subMatrixSize << endl;

        for (int i = 0; i <= stringLengthA; i++)
            delete ptr[i];
        delete[] ptr;
    }
}
