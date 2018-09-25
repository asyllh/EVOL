/*--------------/
ALH
methods.h
Evolutionary Algorithm with Local Search
23/08/18
/--------------*/

#ifndef EVOL_METHODS_H
#define EVOL_METHODS_H

#include <iostream>
#include <algorithm>
#include <vector>


void Swap(int& a, int& b);

int LowerBound(int stripWidth, double totalItemWidth);

double Fitness(int stripWidth, std::vector<int>& stripSum, std::vector<std::vector<int> >& strip);

void FFD(int numScores, int numItem, int maxItemWidth, std::vector<int>& partners, std::vector<std::vector<int> >& itemWidths,
         std::vector<int>& itemOrder);

void FFR(int numScores, int numItem, std::vector<int>& partners, std::vector<std::vector<int> >& itemWidths, std::vector<int>& itemOrder);

void FFShell(int numScores, int numItem, int maxItemWidth, int stripWidth, std::vector<int>& partners,
             std::vector<std::vector<int> >& adjMatrix, std::vector<std::vector<int> >& itemWidths, std::vector<int>& stripSum,
             std::vector<std::vector<int> >& strip, bool decrease);


void PartialFFD(int numScores, int maxItemWidth, int stripWidth, std::vector<int>& partners, std::vector<std::vector<int> >& adjMatrix,
                std::vector<std::vector<int> >& itemWidths, std::vector<int>& partialItem, std::vector<int>& partialSum,
                std::vector<std::vector<int> >& partialSol);


void CreateInitPop(int tau, int numPop, int numScores, int numItem, int maxItemWidth, int stripWidth, std::vector<int>& allScores,
                   std::vector<int>& partners, std::vector<std::vector<int> >& adjMatrix, std::vector<std::vector<int> >& itemWidths,
                   std::vector<std::vector<int> >& populationSum, std::vector<std::vector<std::vector<int> > >& population);


void Mutation(int tau, int numScores, int maxItemWidth, int stripWidth, std::vector<int>& allScores, std::vector<int>& partners,
              std::vector<std::vector<int> >& adjMatrix, std::vector<std::vector<int> >& itemWidths, std::vector<int>& stripSum,
              std::vector<std::vector<int> >& strip);


void LocalSearch(int tau, int numScores, int maxItemWidth, int stripWidth, std::vector<int>& allScores,
                 std::vector<int>& partners, std::vector<std::vector<int> >& adjMatrix, std::vector<std::vector<int> >& itemWidths,
                 std::vector<int>& stripSum, std::vector<std::vector<int> >& strip, std::vector<int>& stripSumX,
                 std::vector<std::vector<int> >& stripX, std::vector<int>& stripSumY, std::vector<std::vector<int> >& stripY);


void InitAHCA(int tau, int swapType, int moveType, int& feasible, int i1, int a1, int b1, int j1, int c1, int d1,
              std::vector<int>& allScores, std::vector<std::vector<int> >& itemWidths, std::vector<int>& stripSumX,
              std::vector<std::vector<int> >& stripX, std::vector<int>& stripSumY, std::vector<std::vector<int> >& stripY);


void AHCA(int tau, int& feasible, std::vector<int>& scores, std::vector<int>& original, std::vector<int>& final);


void InitInstance(int tau, int nScores, std::vector<std::vector<int> >& adjMat, std::vector<int>& scores,
                  std::vector<int>& order, std::vector<int>& partnersX);


void MMCM(int nScores, int& matchSize, std::vector<std::vector<int> >& adjMat, std::vector<int>& partnersX, std::vector<int>& matchList,
          std::vector<int>& cycleVertex);


void MPS(int nScores, int& nCycles, std::vector<int>& partnersX, std::vector<int>& matchList, std::vector<std::vector<int> >& mpStructure);


void BR(int& qstar, int matchSize, std::vector<std::vector<int> >& adjMat, std::vector<int>& matchList, std::vector<int>& cycleVertex,
        std::vector<int>& edge, std::vector<std::vector<int> >& mpStructure, std::vector<std::vector<int> >& C,
        std::vector<std::vector<int> >& S);

void CP(int nScores, int nComp, int& feasible, int qstar, int nCycles, std::vector<int>& partnersX, std::vector<int>& matchList,
        std::vector<int>& cycleVertex, std::vector<int>& edge, std::vector<std::vector<int> >& adjMat, std::vector<std::vector<int> >& C,
        std::vector<std::vector<int> >& S, std::vector<int>& altHam);


void EA(int tau, int recomb, int numScores, int maxItemWidth, int stripWidth, int& bestEnd, double& bestFitness, std::vector<int>& allScores,
        std::vector<int>& partners, std::vector<std::vector<int> >& adjMatrix, std::vector<std::vector<int> >& itemWidths,
        std::vector<std::vector<int> >& allItems, std::vector<std::vector<int> >& populationSum, std::vector<std::vector<std::vector<int> > >& population,
        std::vector<int>& qualityStripsSum, std::vector<std::vector<int> >& qualityStrips, std::vector<int>& qualityItems);

void GGA(int tau, int numScores, int maxItemWidth, int stripWidth, std::vector<int>& allScores, std::vector<int>& partners,
         std::vector<std::vector<int> >& adjMatrix, std::vector<std::vector<int> >& itemWidths, std::vector<int>& offspringSum,
         std::vector<std::vector<int> >& offspring, std::vector<int>& stripSumX, std::vector<std::vector<int> >& stripX,
         std::vector<int>& stripSumY, std::vector<std::vector<int> >& stripY);

void GPX(int tau, int numScores, int maxItemWidth, int stripWidth, std::vector<int>& allScores, std::vector<int>& partners,
         std::vector<std::vector<int> >& adjMatrix, std::vector<std::vector<int> >& itemWidths, std::vector<int>& offspringSum,
         std::vector<std::vector<int> >& offspring, std::vector<int>& stripSumX, std::vector<std::vector<int> >& stripX,
         std::vector<int>& stripSumY, std::vector<std::vector<int> >& stripY);

#endif
