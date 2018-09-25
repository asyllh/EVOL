/*--------------/
ALH
base.h
Evolutionary Algorithm with Local Search
23/08/18
/--------------*/
#ifndef EVOL_BASE_H
#define EVOL_BASE_H

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <vector>

struct Timer{

    std::chrono::high_resolution_clock::time_point startTime, endTime;
    std::chrono::duration<float> totalTime;

    Timer(){
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
        endTime = std::chrono::high_resolution_clock::now();
        totalTime = endTime - startTime;

        float totalTimems = totalTime.count() * 1000.0f;
        std::cout << "\nCPU Time: " << totalTimems << "ms (" << totalTime.count() << "s)" << std::endl;
    }
};

void ProgramInfo();

void ArgumentCheck(int numIterations, int tau, int numItem, int minWidth, int maxWidth, int minItemWidth, int maxItemWidth,
                   int stripWidth, int numPop, int xOver, int randomSeed);

void CreateInstance(int tau, int numScores, int numItem, int minWidth, int maxWidth, int minItemWidth, int maxItemWidth, double& totalItemWidth,
                    std::vector<int>& allScores, std::vector<int>& partners, std::vector<std::vector<int> >& adjMatrix,
                    std::vector<std::vector<int> >& itemWidths, std::vector<std::vector<int> >& allItems);

#endif
