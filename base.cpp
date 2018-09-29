/*--------------/
ALH
base.cpp
Evolutionary Algorithm with Local Search
23/08/2018
/--------------*/

#include "base.h"

void ProgramInfo(){

    std::cout << "Evolutionary Algorithm for the SCSPP:\n-------------\n"
              << "PARAMETERS:\n"
              << "       -i <int>    [Number of iterations. Default = 1000.]\n"
              << "       -t <int>    [Constraint value. Default = 70.]\n"
              << "       -n <int>    [Number of items. Default = 500.]\n"
              << "       -a <int>    [Minimum score width. Default = 1.]\n"
              << "       -b <int>    [Maximum score width. Default = 70.]\n"
              << "       -m <int>    [Minimum item width. Default = 150.]\n"
              << "       -M <int>    [Maximum item width. Default = 1000.]\n"
              << "       -W <int>    [Width of strips. Default = 5000.]\n"
              << "       -p <int>    [Number of solutions in population.]\n"
              << "       -r <int>    [Crossover operator. 1: GGA. 2: GPX'.]\n"
              << "       -s <int>    [Random seed. Default = 1.]\n"
              << "---------------\n\n";
}

void ArgumentCheck(int numIterations, int tau, int numItem, int minWidth, int maxWidth, int minItemWidth, int maxItemWidth,
                   int stripWidth, int numPop, int xOver, int randomSeed){

    bool error = false;

    std::cout << "Evolutionary Algorithm for the SCSPP\n------------------------------\n";
    if(tau == 0){
        std::cerr << "[ERROR]: Constraint value cannot be zero.\n";
        error = true;
    }
    if(stripWidth == 0){
        std::cerr << "[ERROR]: Strip cannot have length zero.\n";
        error = true;
    }
    if(2*minWidth >= tau){
        std::cerr << "[ERROR]: Constraint value is less than or equal to twice the minimum score width, vicinal sum constraint always valid.\n";
        std::cerr << "         Problem instance is therefore classical strip-packing problem without score constraint (i.e. tau = 0).\n";
        error = true;
    }
    if(2*maxWidth < tau){
        std::cerr << "[ERROR]: Constraint value is greater than double maximum score width, vicinal sum constraint never valid.\n";
        error = true;
    }
    if(2*maxWidth >= minItemWidth){
        std::cerr << "[ERROR]: Minimum item width is less than double maximum score width, scores may overlap.\n";
        error = true;
    }
    if(minWidth > maxWidth){
        std::cerr << "[ERROR]: Minimum score width is greater than maximum score width.\n";
        error = true;
    }
    if(maxItemWidth > stripWidth){
        std::cerr << "[ERROR]: Maximum item width is larger than length of strip.\n";
        error = true;
    }
    if(numPop < 5){
        std::cerr << "[ERROR]: Insufficient number of solutions in population.\n";
        error = true;
    }
    if(xOver != 1 && xOver != 2){
        std::cerr << "[ERROR]: Invalid choice of recombination operator. Please choose either 1: GGA, or 2: GPX'.\n";
        error = true;
    }

    if(error){
        std::cerr << "[EXIT PROGRAM.]\n";
        exit(1);
    }

    std::cout << std::left << std::setw(20) << "Number of iterations:" << std::right << std::setw(9) << numIterations << std::endl
              << std::left << std::setw(20) << "Constraint value:" << std::right << std::setw(10) << tau << std::endl
              << std::left << std::setw(20) << "Number of items:" << std::right << std::setw(10) << numItem << std::endl
              << std::left << std::setw(20) << "Minimum score width:" << std::right << std::setw(10) << minWidth << std::endl
              << std::left << std::setw(20) << "Maximum score width:" << std::right << std::setw(10) << maxWidth << std::endl
              << std::left << std::setw(20) << "Minimum item width:" << std::right << std::setw(10) << minItemWidth << std::endl
              << std::left << std::setw(20) << "Maxmimum item width:" << std::right << std::setw(10) << maxItemWidth << std::endl
              << std::left << std::setw(20) << "Width of strips:" << std::right << std::setw(10) << stripWidth << std::endl
              << std::left << std::setw(20) << "Population size:" << std::right << std::setw(10) << numPop << std::endl;
    if(xOver == 1){
        std::cout << std::left << std::setw(20) << "Crossover operator: " << std::right << std::setw(10) << "GGA" << std::endl;
    }
    else if(xOver == 2){
        std::cout << std::left << std::setw(20) << "Recombination operator: " << std::right << std::setw(6) << "GPX" << std::endl;
    }
    std::cout << std::left << std::setw(20) << "Random seed:" << std::right << std::setw(10) << randomSeed << std::endl;
    std::cout << "------------------------------\n\n";
}

void CreateInstance(int tau, int numScores, int numItem, int minWidth, int maxWidth, int minItemWidth, int maxItemWidth, double& totalItemWidth,
                    std::vector<int>& allScores, std::vector<int>& partners, std::vector<std::vector<int> >& adjMatrix,
                    std::vector<std::vector<int> >& itemWidths, std::vector<std::vector<int> >& allItems){

    int i, j, k;
    int count = 1;
    std::vector<int> randOrder;
    std::vector<int> checkItem(numScores, 0);
    totalItemWidth = 0.0;

    //Create random values to be used as score widths, put in allScores vector (except last two elements)
    for (i = 0; i < numScores; ++i) {
        allScores.push_back(rand() % (maxWidth - minWidth + 1) + minWidth);
    }


    //Sort all of the scores in the allScores vector in ascending order
    std::sort(allScores.begin(), allScores.end()); //sorts elements of vector in ascending order

    //std::cout << "All scores:\n";
    /*for(const auto& v : allScores){
        std::cout << v << " ";
    }
    std::cout << std::endl;*/

    //Filling in adjacency matrix - if sum of two scores >= tau (70), then insert 1 into the matrix, else leave as 0
    for (i = 0; i < allScores.size() - 1; ++i) {
        for (j = i + 1; j < allScores.size(); ++j) {
            if (allScores[i] + allScores[j] >= tau) {
                adjMatrix[i][j] = 1;
                adjMatrix[j][i] = 1;
            }
        }

    }

    //Initially, randOrder vector will contain elements in the order 0, ..., numScores -2, numScores -1
    for (i = 0; i < numScores; ++i) {
        randOrder.push_back(i);
    }

    //Randomly shuffle all values in randOrder vector
    std::random_shuffle(randOrder.begin(), randOrder.end());

    //Assign partners to each score (i.e. pair up scores to define which scores are either side of the same box)
    //In the adjacency matrix, this will be represented by value 2
    //Therefore there will be a value of 2 in every row and every column, non repeating
    for (i = 0; i < numItem; ++i) {
        adjMatrix[randOrder[2 * i]][randOrder[2 * i + 1]] = 2;
        adjMatrix[randOrder[2 * i + 1]][randOrder[2 * i]] = 2;
    }

    /*std::cout << "AdjMatrix:\n";
    for(const auto& subVec : adjMatrix)
        for(const auto& v : subVec){
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;*/



    for (i = 0; i < numScores; ++i) {
        for (j = 0; j < numScores; ++j) {
            if (adjMatrix[i][j] == 2) {
                partners[i] = j;
                break;
            }
        }
    }
    /*std::cout << "Mates Vector:\n";
    for(const auto& v : partners){
        std::cout << v << " ";
    }
    std::cout << std::endl << std::endl;*/

    for(i = 0; i < numScores; ++i){
        for(j = 0; j < numScores; ++j){
            if(adjMatrix[i][j] == 2 && itemWidths[i][j] == 0){
                itemWidths[i][j] = rand() % (maxItemWidth - minItemWidth + 1) + minItemWidth;
                itemWidths[j][i] = itemWidths[i][j];
                break;
            }

        }
    }

    k = 0;
    for(i = 0; i < numScores; ++i){
        for(j = i+1; j < numScores; ++j){
            if(adjMatrix[i][j] == 2){
                allItems[i][j] = k;
                allItems[j][i] = k;
                ++k;
                break;
            }
        }
    }
    //std::cout << std::endl;

    //cout << right << setw(5) << "Box#" << setw(12) << "Scores" << setw(12) << "Mates" << setw(12) << "Width\n";
    for(i = 0; i < numScores; ++i){
        if(checkItem[i] == 1){
            continue;
        }
        //cout << setw(5) << count << setw(10) << allScores[i] << "-" << allScores[partners[i]] << setw(10) << i  << "-" << partners[i] << setw(10) << itemWidths[i][partners[i]] << endl;
        totalItemWidth += itemWidths[i][partners[i]];
        checkItem[i] = 1;
        checkItem[partners[i]] = 1;
        ++count;

    }

    //cout << "Total Item Widths: " << totalItemWidth << endl << endl;

    /*std::cout << "allItems:\n";
    for(const auto& subVec : allItems)
        for(const auto& v : subVec){
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;*/

}

