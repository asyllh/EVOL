/*--------------/
ALH
main.cpp
Evolutionary Algorithm with Local Search
23/08/2018
/--------------*/
/** NOTES **/
/* Why does the program take longer when n = 10? (numItems = 10)
 * Change local search into functions/remove gotolabels
 * Fill up setQS during EA rather than creating qualityStrips, then dlxMatrix, then putting it into setQS
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
    int numIterations = 1000; //Number of times EA will be called
    int tau = 70; //Minimum scoring distance
    int numItem = 500; //Number of items n in the set I
    int minWidth = 1; //Minimum width of the scores
    int maxWidth = 70; //Maximum width of the scores
    int minItemWidth = 150; //Minimum width of the items
    int maxItemWidth = 1000; //Maximum width of the items
    int stripWidth = 5000; //Width of strips
    int numPop = 0; //Number of initial solutions in population
    int xOver = 1; //Crossover Type, 1 = GGA, 2 = GPX
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
    int bestStart, bestEnd; //Takes index i of population (i.e. the solution i in the population) that has the best Fitness at the beginning and end
    int numScores = numItem * 2; //Number of score widths
    double totalItemWidth; //Sum of all n item widths in the set I
    double tempFitness; //temp variable to help keep track of the best Fitness so far
    double bestFitness = 0.0; //Value of the Fitness of the best solution
    std::vector<int> allScores; //Vector containing all score widths (size = numScores)
    std::vector<int> partners(numScores, 0); //Vector containing partners
    std::vector<std::vector<int> > itemWidths(numScores, std::vector<int>(numScores, 0)); //Matrix containing item widths
    std::vector<std::vector<int> > adjMatrix(numScores, std::vector<int>(numScores, 0)); //Matrix showing score widths that meet VSC or that are partners
    std::vector<std::vector<int> > allItems(numScores, std::vector<int>(numScores, 0)); //Matrix numbering all items from 0 to numItem - 1
    std::vector<std::vector<std::vector<int> > > population; //3D matrix containing solutions
    std::vector<std::vector<int> > populationSum; //Contains sum of each strip for every solution in the population
    std::vector<std::vector<int> > bestSolnStart; //Contains the solution that has the best Fitness at the start
    std::vector<int> bestSolnStartSum; //Contains the sum of the strips in the solution that has the best Fitness at the start
    std::vector<std::vector<int> > qualityStrips; //Contains strips that will be used for postoptimization
    std::vector<int> qualityStripsSum; //Contains sum of the strips in qualityStrips
    std::vector<int> qualityItems(numItem, 0); //Contains the number of times each item appears in qualityStrips
    std::set<std::vector<int> > feasiblePacking; //Set of Vectors containing strips that have a feasible arrangement of items
    std::set<std::vector<int> > infeasiblePacking; //Set of Vectors containing strips that have an infeasible arrangement of items
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

    for(int iteration = 0; iteration < numIterations; ++iteration) {
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
    std::cout << std::endl;

    std::cout << "qualityStripSum - Sum of all items on each strip:\n";
    for(const auto& v : qualityStripsSum){
        std::cout << v << " ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "qualityItems - Number of times item appears in qualityStrips set\n";
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
