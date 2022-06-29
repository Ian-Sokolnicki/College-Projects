// CS 4318, spring 2022
// Agent Challenge 5: Chocolate Russian roulette
//
// Here are the functions available to each agent.

#include "crr.h"

ChocolateSquareLocation::ChocolateSquareLocation(int column, int row)
{
   // Create new location with given column and row.
   this->column = column > 0 ? column : 0;
   this->row = row > 0 ? row : 0;
}

ChocolateBar::ChocolateBar(int barWidth, int barHeight)
{
   // Create new bar with given width and height.
   int column;
   this->barWidth = max(barWidth, 1);
   this->barHeight = max(barHeight, 1);
   squareThere = new bool *[barWidth];
   for (column = 0; column < barWidth; column += 1)
   {
      squareThere[column] = new bool[barHeight];
   }
   replenishBar();
}

ChocolateBar::ChocolateBar(const ChocolateBar &otherBar)
{
   // Create new bar identical to given bar.
   int column, row;
   barWidth = otherBar.getWidth();
   barHeight = otherBar.getHeight();
   squareThere = new bool *[barWidth];
   for (column = 0; column < barWidth; column += 1)
   {
      squareThere[column] = new bool[barHeight];
      for (row = 0; row < barHeight; row += 1)
      {
         squareThere[column][row] = otherBar.isSquareThere(column, row);
      }
   }
}

ChocolateBar::~ChocolateBar()
{
   // Destroy bar.
   int column;
   for (column = 0; column < barWidth; column += 1)
   {
      delete[] squareThere[column];
   }
   delete[] squareThere;
}

ChocolateBar &ChocolateBar::operator=(const ChocolateBar &otherBar)
{
   // Create new bar identical to given bar, destroying old one.
   int column, row;
   if (barWidth != otherBar.getWidth() || barHeight != otherBar.getHeight())
   {
      for (column = 0; column < barWidth; column += 1)
      {
         delete[] squareThere[column];
      }
      delete[] squareThere;
      barWidth = otherBar.getWidth();
      barHeight = otherBar.getHeight();
      squareThere = new bool *[barWidth];
      for (column = 0; column < barWidth; column += 1)
      {
         squareThere[column] = new bool[barHeight];
      }
   }
   for (column = 0; column < barWidth; column += 1)
   {
      for (row = 0; row < barHeight; row += 1)
      {
         squareThere[column][row] = otherBar.isSquareThere(column, row);
      }
   }
   return *this;
}

bool ChocolateBar::isAnyLeft() const
{
   // Return true if and only if there's at least one square left.
   ChocolateSquareLocation poison(0, 0);
   return isSquareThere(poison);
}

bool ChocolateBar::isSquareThere(ChocolateSquareLocation location) const
{
   // Return true if and only if the given square is still there.
   return isSquareThere(location.getColumn(), location.getRow());
}

bool ChocolateBar::isSquareThere(int column, int row) const
{
   // Return true if and only if the given square is still there.
   return column >= 0 && column < barWidth && row >= 0 && row < barHeight && squareThere[column][row];
}

void ChocolateBar::replenishBar()
{
   // Restore the bar to its original completeness.
   int column, row;
   for (column = 0; column < barWidth; column += 1)
   {
      for (row = 0; row < barHeight; row += 1)
      {
         squareThere[column][row] = true;
      }
   }
}

int ChocolateBar::takeBite(ChocolateSquareLocation bite)
{
   // Take a bite out of the bar and return number of squares eaten.
   return takeBite(bite.getColumn(), bite.getRow());
}

int ChocolateBar::takeBite(int biteColumn, int biteRow)
{
   // Take a bite out of the bar and return number of squares eaten.
   int column, row, squaresEaten;
   if (!isSquareThere(biteColumn, biteRow))
   {
      return 0;
   }
   squaresEaten = 0;
   for (column = biteColumn; column < barWidth; column += 1)
   {
      for (row = biteRow; row < barHeight; row += 1)
      {
         if (isSquareThere(column, row))
         {
            squareThere[column][row] = false;
            squaresEaten += 1;
         }
      }
   }
   return squaresEaten;
}
