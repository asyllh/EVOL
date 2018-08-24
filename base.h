/*--------------/
ALH
base.h
Evolutionary Algorithm with Local Search
23/08/18
/--------------*/

#ifndef EVOL_BASE_H
#define EVOL_BASE_H

#include <iostream>
#include <vector>
using namespace std;

void CreateInstance(int tau, int numScores, int numItem, int minWidth, int maxWidth, int minItemWidth, int maxItemWidth, double &totalItemWidth,
                    vector<int> &allScores, vector<int> &partners, vector<vector<int> > &adjMatrix,
                    vector<vector<int> > &itemWidths, vector<vector<int> > &allItems);

#endif
