/*--------------/
ALH
main.cpp
Evolutionary Algorithm with Local Search
23/08/2018
/--------------*/

/** NOTES **/
/* Why does the program take longer when n = 10? (numItems = 10)
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <iterator>
#include <set>
#include "base.h"
#include "methods.h"

int main(int argc, char** argv){
    if(argc <= 1){
        ProgramInfo();
        exit(1);
    }

    //region Default Variables
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
    //endregion


    //region User Arguments
    for(int x = 1; x < argc; ++x){
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

    //region Variables
    int bestStart, bestEnd;
    int iteration;
    int numScores = numItem * 2;
    double totalItemWidth;
    double tempFitness;
    double bestFitness = 0.0;
    std::vector<int> allScores;
    std::vector<int> partners(numScores, 0);
    std::vector<std::vector<int> > itemWidths(numScores, std::vector<int>(numScores, 0));
    std::vector<std::vector<int> > adjMatrix(numScores, std::vector<int>(numScores, 0));
    std::vector<std::vector<int> > allItems(numScores, std::vector<int>(numScores, 0));
    std::vector<std::vector<std::vector<int> > > population;
    std::vector<std::vector<int> > populationSum;
    std::vector<std::vector<int> > bestSolnStart;
    std::vector<int> bestSolnStartSum;
    std::vector<std::vector<int> > qualityStrips;
    std::vector<int> qualityStripsSum;
    std::vector<int> qualityItems(numItem, 0);
    std::set<std::vector<int> > feasiblePacking;
    std::set<std::vector<int> > infeasiblePacking;
    //endregion

    srand(randomSeed);
    Timer timer;

    CreateInstance(tau, numScores, numItem, minWidth, maxWidth, minItemWidth, maxItemWidth, totalItemWidth, allScores, partners,
                   adjMatrix, itemWidths, allItems);

    std::vector<int> temp;
    for(int i = 0; i < numItem; ++i){
        temp.push_back(i);
        feasiblePacking.insert(temp);
        temp.clear();
    }





    int lowerBound = LowerBound(stripWidth, totalItemWidth);
    //std::cout << "Lower bound: " << lowerBound << " strips.\n\n";

    CreateInitPop(tau, numPop, numScores, numItem, maxItemWidth, stripWidth, allScores, partners, adjMatrix, itemWidths,
                  populationSum, population, allItems, feasiblePacking);


    //Finding the solution in the population that has the best fitness value
    for(int i = 0; i < population.size(); ++i){
        tempFitness = Fitness(stripWidth, populationSum[i], population[i]);
        if(tempFitness > bestFitness){
            bestFitness = tempFitness;
            bestStart = i;
        }
    }
    bestSolnStart = population[bestStart];
    bestSolnStartSum = populationSum[bestStart];


    //std::cout << "START - Best solution in the population:\n";
    //std::cout << "Solution: " << bestStart << "\nfitness: " << bestFitness << "\nSize: " << bestSolnStart.size() << " strips.\n\n";

    bestEnd = bestStart;


    for(iteration = 0; iteration < numIterations; ++iteration) {
        EA(tau, xOver, numScores, maxItemWidth, stripWidth, bestEnd, bestFitness, allScores, partners, adjMatrix, itemWidths,
           allItems, populationSum, population, qualityStripsSum, qualityStrips, qualityItems);
    }

    std::cout << "feasiblePacking:\n";
    for(const auto& subVec : feasiblePacking){
        for(const auto& v : subVec){
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /*std::cout << "qualityStrips - Strips that are in qualityStrips set:\n";
    for(const auto& subVec : qualityStrips){
        for(const auto& v : subVec){
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;*/

    /*std::cout << "qualityStripSum - Sum of all items on each strip:\n";
    for(const auto& v : qualityStripsSum){
        std::cout << v << " ";
    }
    std::cout << std::endl << std::endl;*/

    /*std::cout << "qualityItems - Number of times item appears in qualityStrips set\n";
    for(const auto& v : qualityItems){
        std::cout << v << " ";
    }
    std::cout << std::endl <<std::endl;*/

    //Rows of matrix are strips, columns are items
    std::vector<std::vector<int> > dlxMatrix(qualityStrips.size());

    for(int i = 0; i < qualityStrips.size(); ++i){
        for(int j = 0; j < qualityStrips[i].size() -1; j+=2){
            dlxMatrix[i].push_back(allItems[qualityStrips[i][j]][qualityStrips[i][j+1]]);
        }
        std::sort(dlxMatrix[i].begin(), dlxMatrix[i].end());

    }

    //std::cout << "Size of dlxMatrix: " << dlxMatrix.size() << std::endl;

    std::set<std::vector<int> > setQS;
    for(int i = 0; i < dlxMatrix.size(); ++i){
        setQS.insert(dlxMatrix[i]);
    }

    //std::cout << "Size of setQS: " << setQS.size() << std::endl;

    std::set<std::vector<int> >::const_iterator itSet;
    std::vector<int>::const_iterator itVector;

    std::ofstream ofs("test.txt");
    if(!ofs){
        std::cerr << "[ERROR]: Cannot write to file." << std::endl;
        exit(1);
    }
    std::vector<int> temp2;
    for(int i = 0; i < numItem; ++i){
        temp.push_back(i);
    }
    std::copy(temp.begin(), temp.end(), std::ostream_iterator<int>(ofs, " "));
    ofs << std::endl;
    //std::cout << "setQS:\n;
    for(itSet = setQS.begin(); itSet != setQS.end(); ++itSet){
        for(itVector = itSet->begin(); itVector != itSet->end(); ++itVector){
            ofs << *itVector << " ";
            //std::cout << *itVector << " ";
        }
        ofs << std::endl;
        //std::cout << std::endl
    }
    ofs.close();
    std::cout << std::endl;

    //std::cout << "END - Best solution in the population:\n";
    //std::cout << "Solution: " << bestEnd << "\nfitness: " << bestFitness << "\nSize: " << population[bestEnd].size() << " strips." << std::endl << std::endl;


}//END INT MAIN


