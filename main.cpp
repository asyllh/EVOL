/*--------------/
ALH
main.cpp
Evolutionary Algorithm with Local Search
23/08/2018
/--------------*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <cmath>
#include "base.h"
#include "methods.h"

int main(int argc, char** argv){
    if(argc <= 1){
        ProgramInfo();
        exit(1);
    }

    int x;
    int numIterations = 1000;
    int tau = 70;
    int numItem = 500;
    int minWidth = 1;
    int maxWidth = 70;
    int minItemWidth = 150;
    int maxItemWidth = 1000;
    int stripWidth = 5000;
    int numPop = 0;
    int xOver = 1;
    int randomSeed = 1;


    //region User Arguments
    for(x = 1; x < argc; ++x){
        if(strcmp("-i", argv[x]) == 0){
            numIterations = atoi(argv[++x]);
        }
        else if(strcmp("-t", argv[x]) == 0){
            tau = atoi(argv[++x]);
        }
        else if(strcmp("-n", argv[x]) == 0){
            numItem = atoi(argv[++x]);
        }
        else if(strcmp("-a", argv[x]) == 0){
            minWidth = atoi(argv[++x]);
        }
        else if(strcmp("-b", argv[x]) == 0){
            maxWidth = atoi(argv[++x]);
        }
        else if(strcmp("-m", argv[x]) == 0){
            minItemWidth = atoi(argv[++x]);
        }
        else if(strcmp("-M", argv[x]) == 0){
            maxItemWidth = atoi(argv[++x]);
        }
        else if(strcmp("-W", argv[x]) == 0){
            stripWidth = atoi(argv[++x]);
        }
        else if(strcmp("-p", argv[x]) == 0){
            numPop = atoi(argv[++x]);
        }
        else if(strcmp("-r", argv[x]) == 0){
            xOver = atoi(argv[++x]);
        }
        else if(strcmp("-s", argv[x]) == 0){
            randomSeed = atoi(argv[++x]);
        }
    }
    //endregion

    ArgumentCheck(numIterations, tau, numItem, minWidth, maxWidth, minItemWidth, maxItemWidth, stripWidth, numPop, xOver, randomSeed);

    int i, j, k, bestStart, bestEnd;
    int iteration;
    int numScores = numItem * 2;
    double totalItemWidth;
    std::vector<int> allScores;
    std::vector<int> partners(numScores, 0);
    std::vector<std::vector<int> > itemWidths(numScores, std::vector<int>(numScores, 0));
    std::vector<std::vector<int> > adjMatrix(numScores, std::vector<int>(numScores, 0));
    std::vector<std::vector<int> > allItems(numScores, std::vector<int>(numScores, 0));
    std::vector<std::vector<std::vector<int> > > population;
    std::vector<std::vector<int> > populationSum;
    std::vector<std::vector<int> > bestSolnStart;
    std::vector<int> bestSolnStartSum;
    double bestFitness = 0.0;
    double tempFitness;

    std::vector<std::vector<int> > qualityStrips;
    std::vector<int> qualityStripsSum;
    std::vector<int> qualityItems(numItem, 0);

    srand(randomSeed);
    Timer timer;

    CreateInstance(tau, numScores, numItem, minWidth, maxWidth, minItemWidth, maxItemWidth, totalItemWidth, allScores, partners,
                   adjMatrix, itemWidths, allItems);


    int LB = LowerBound(stripWidth, totalItemWidth);
    std::cout << "Lower bound: " << LB << " strips." << std::endl << std::endl;

    CreateInitPop(tau, numPop, numScores, numItem, maxItemWidth, stripWidth, allScores, partners, adjMatrix, itemWidths,
                  populationSum, population);


    //Finding the solution in the population that has the best fitness value
    for(i = 0; i < population.size(); ++i){
        tempFitness = Fitness(stripWidth, populationSum[i], population[i]);
        if(tempFitness > bestFitness){
            bestFitness = tempFitness;
            bestStart = i;
        }
    }
    bestSolnStart = population[bestStart];
    bestSolnStartSum = populationSum[bestStart];


    std::cout << "START - Best solution in the population:\n";
    std::cout << "Solution: " << bestStart << "\nfitness: " << bestFitness << "\nSize: " << bestSolnStart.size() << " strips.\n\n";

    bestEnd = bestStart;


    for(iteration = 0; iteration < numIterations; ++iteration) {
        EA(tau, xOver, numScores, maxItemWidth, stripWidth, bestEnd, bestFitness, allScores, partners, adjMatrix, itemWidths,
           allItems, populationSum, population, qualityStripsSum, qualityStrips, qualityItems);
    }

    std::cout << "qualityStrip\n";
    for(i = 0; i < qualityStrips.size(); ++i){
        for(j = 0; j < qualityStrips[i].size(); ++j){
            std::cout << qualityStrips[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;

    std::cout << "qualityStripSum\n";
    for(i = 0; i < qualityStripsSum.size(); ++i){
        std::cout << qualityStripsSum[i] << " ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "qualityItems\n";
    for(i = 0; i < qualityItems.size(); ++i){
        std::cout << qualityItems[i] << " ";
    }
    std::cout << std::endl << std::endl;

    std::vector<std::vector<int> > A(numItem, std::vector<int>(qualityStrips.size(), 0));

    for(i = 0; i < qualityStrips.size(); ++i){
        for(j = 0; j < qualityStrips[i].size() -1; j+=2){
            A[allItems[qualityStrips[i][j]][qualityStrips[i][j+1]]][i] = 1;
        }
    }

    std::cout << "Matrix A\n";
    for (i = 0; i < A.size(); ++i){
        for(j = 0; j < A[i].size(); ++j){
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;

    std::cout << "END - Best solution in the population:\n";
    std::cout << "Solution: " << bestEnd << "\nfitness: " << bestFitness << "\nSize: " << population[bestEnd].size() << " strips." << std::endl << std::endl;


}//END INT MAIN


