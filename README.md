**README**

### CreateInstance
* Generates random `numScores`, `partners`, `adjMatrix`, `itemWidths`, `allItems` (numbering each item)

### Calculate lower bound (using `LowerBound` function)

### CreateInitialPopulation
* Creates `population` of solutions, one using `FFD`, the rest using `FFR`

### LocalSearch
#### PairPair - swapType = 1
1. Straight swap, no moveType
    * `stripX` = 2 items and `stripY` = 2 items
2. moveType = 11
    * `stripX` = 2 items and `stripY` > 2 items, c and d adjacent    
3. moveType = 0
    * `stripX` = 2 items and `stripY` > 2 items, c and d not adjacent    
4. moveType = 12
    * `stripY` = 2 items and `stripX` > 2 items, a and b adjacent    
5. moveType = 0    
    * `stripY` = 2 items and `stripX` > 2 items, a and b not adjacent    
6. moveType = 0;
    * `stripX` > 2 items and `stripY` > 2 items 

#### PairSin - swapType = 2
1. Straight swap, no moveType
    * `stripX` = 2 items and `stripY` = 1 item
2. moveType = 21
    * `stripX` = 2 items and `stripY` > 1 item
3. moveType = 22
    * `stripY` = 1 item and `stripX` > 2 items, a and b adjacent    
4. moveType = 0    
    * `stripY` = 1 item and `stripX` > 2 items, a and b not adjacent    
5. moveType = 0;
    * `stripX` > 2 items and `stripY` > 1 item 
    
#### SinSin - swapType = 3
1. Straight swap, no moveType
    * `stripX` = 1 item and `stripY` = 1 item
2. moveType = 31
    * `stripX` = 1 item and `stripY` > 1 item
3. moveType = 32
    * `stripY` = 1 item and `stripX` > 1 item    
4. moveType = 0    
    * `stripX` > 1 item and `stripY` > 1 item   
    
#### MoveSin - swapType = 4
1. moveType = 41
    * `stripY` = 1 item
2. moveType = 42
    * `stripY` = 2 items
3. moveType = 0
    * `stripY` > 2 items         
    

### Find solution in population that has the best fitness (using `Fitness` function)

### EA
* Choose two solutions from `population`, these will be parents (`parent1` = `stripX` and `parent2` = `stripY`)
* Calculate fitness of each of these parent solutions (`parent1Cost` and `parent2Cost` using `Fitness` function)
* Run `GGA`/`GPX`
* Go through strips in `offspring`, find strips that have best fullness, add these to `qualityStrips`
* Calculate fitness of `offspring` (`offspringCost` using `Fitness` function)
* If `parent1` has worst fitness, remove `parent1` from population and replace with `offspring`
* Else if `parent2` has worst fitness, remove `parent2` from population and replace with `offspring`
* Else if `parent1` and `parent2` have equal fitness, choose at random.
* If fitness of `offspring` is better than `bestFitness` (current best fitness of a solution in the population), set `bestFitness` = `offspringCost`
* End `EA`

### GGA
* Choose two strips `k` and `l` from `stripY` randomly
* Mark all items in strips between `k` and `l` (including `k` and `l`) as `checked`
* Go through `stripX`, if a strip contains an item that is checked, delete entire strip from `stripX`
* Put strips between `k` and `l` (including `k` and `l`) from `stripY` into `stripX`
* Go through `stripX`, mark all items as `checked`
* If all items are in `stripX`, `offspring = stripX`, return to `EA` function.
* If any items are not in `stripX` (i.e. `checked == 0`), put these items in `absentItems`
* Clear `stripY`, run `PartialFFD` on `absentItems` and `stripY`, then run `LocalSearch` using `stripX` and `stripY`, return to `EA` function.

### GPX
* Choose fullest strip out of all strips in `stripX` and `stripY` (e.g. fullest strip is in `stripX`)
* Mark items in fullest strip as `checked`, put this strip in `offspring`, erase this strip from stripX`
* Go through all strips in `stripY`, if there is a strip that contains a marked item, delete entire strip from `stripY`
* Now go to `stripY` and choose fullest strip
* Repeat, alternate between `stripX` and `stripY`
* If all items are in `offspring`, do nothing (i.e. `absentItems` vector is empty), return to `EA` function
* If any items are not in `offspring` (i.e. `checked == 0`), put these items in `absentItems`
* `stripX = offspring`, clear `offspring`, clear `stripY`, run `PartialFFD` on `absentItems` and `stripY`, then run `LocalSearch` using `stripX` and `stripY`, return to `EA` function.





































