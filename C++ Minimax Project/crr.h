// CS 4318, spring 2022
// Agent Challenge 5: Chocolate Russian roulette
//
// Here are the #includes and definitions available to each agent.

#ifndef CRR_H
#define CRR_H

#include <algorithm>
#include <cfloat>
#include <climits>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// The immutable location of one square of chocolate.
class ChocolateSquareLocation
{
private:
   int column;
   int row;
public:
   ChocolateSquareLocation(int column = 0, int row = 0);
   int getColumn() const {return column;}
   int getRow() const {return row;}
};

// A chocolate bar being bitten into.
class ChocolateBar
{
private:
   bool **squareThere;
   int barHeight;
   int barWidth;
public:
   ChocolateBar(int barWidth = 1, int barHeight = 1);
   ChocolateBar(const ChocolateBar &otherBar);
   ~ChocolateBar();
   ChocolateBar &operator=(const ChocolateBar &otherBar);
   int getHeight() const {return barHeight;}
   int getWidth() const {return barWidth;}
   bool isAnyLeft() const;
   bool isSquareThere(ChocolateSquareLocation location) const;
   bool isSquareThere(int column, int row) const;
   void replenishBar();
   int takeBite(ChocolateSquareLocation bite);
   int takeBite(int biteColumn, int biteRow);
};

#endif // #ifndef CRR_H
