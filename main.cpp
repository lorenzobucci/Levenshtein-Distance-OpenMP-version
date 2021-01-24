#include <iostream>
#include <chrono>
#include <random>
#include "LevenshteinDistance.h"

using namespace std;

int main() {

    int stringLengthA = 32041;
    int stringLengthB = 30137;

    string str("abcdefghijklmnopqrstuvwxyz");

    mt19937 generator{random_device{}()};
    uniform_int_distribution<int> distribution{'a', 'z'};

    string a(stringLengthA, '\0');
    for(auto& dis: a)
        dis = distribution(generator);
    string b(stringLengthB, '\0');
    for(auto& dis: b)
        dis = distribution(generator);

    auto startTime = chrono::high_resolution_clock::now();
    unsigned short sequentialResult = sequentialAlgorithm(a, b);
    auto endTime = chrono::high_resolution_clock::now();

    chrono::duration<float> timeElapsed = endTime - startTime;
    cout << "Sequential edit distance result: " << sequentialResult << endl;
    cout << "Sequential execution time in s : " << timeElapsed.count() << endl;


    for (unsigned short i = 10; i <= 1000; i+=100) {
        startTime = chrono::high_resolution_clock::now();
        unsigned short parallelResult = parallelAlgorithm(a, b, i);
        endTime = chrono::high_resolution_clock::now();
        timeElapsed = endTime - startTime;
        cout << endl << "Parallel edit distance result: " << parallelResult << endl;
        cout << "Parallel execution time in s: " << timeElapsed.count() << " with sub-matrices size: " << i << endl;
    }
}
