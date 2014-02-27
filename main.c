#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <time.h>
using namespace std;


int main() {
    // Keeps the number of the sets
    int numOfSets;
    // Used to keep tracked of which line we're assessing
    int lineCount = 0;
    // A flag used so it knows if the line that it's assessing is a space
    int nextSpace = 2;
    // used for text file reading
    int flag = 0;
    int i;
    string line;
    ifstream myfile("input.txt");
    ofstream writeFile("group12.txt");

    //Checks to make sure the file is open
    if(myfile.is_open()) {
        // Reads the next line until the file is empty
        while(getline(myfile, line)) {
            lineCount++;
            // If it is the first line, then we know it's the number of sets
            if(lineCount == 1) numOfSets = atoi(line.c_str());
            // if the line count is the next space, then we know to do no reading
            else if(lineCount == nextSpace) {
                flag = 1;
                continue;
            }
    myfile.close();
    writeFile.close();
    return 0;
}