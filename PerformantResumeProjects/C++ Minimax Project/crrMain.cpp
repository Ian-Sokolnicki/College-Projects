// CS 4318, spring 2022
// Agent Challenge 5: Chocolate Russian roulette
//
// Here's the main function that plays all agents against each other and
// summarizes the results.

#include "crr.h"

extern const int numAgents;
extern ChocolateSquareLocation (*agentFunc[])(const ChocolateBar &);
extern string agentStr[];

struct GameResult
{
   bool wasInvalidBiteTaken;
   char winner;
   int numBitesTakenByA;
   int numBitesTakenByB;
   int numSquaresEatenByA;
   int numSquaresEatenByB;
};

GameResult playChocolateRussianRouletteGame(ChocolateSquareLocation (*agentA)(const ChocolateBar &), ChocolateSquareLocation (*agentB)(const ChocolateBar &), int barWidth, int barHeight);

int main()
{
   GameResult result;
   int barHeight, barWidth, i, j, numBitesTaken[numAgents],
       numInvalidBites[numAgents], numLossesAsA[numAgents],
       numLossesAsB[numAgents], numSquaresEaten[numAgents],
       numWinsAsA[numAgents], numWinsAsB[numAgents], order[numAgents], temp;

   cout << "Agent Challenge 5: Chocolate Russian roulette\n"
        << "Iteration results\n\n";
   for (i = 0; i < numAgents; i += 1)
   {
      numWinsAsA[i] = 0;
      numLossesAsA[i] = 0;
      numWinsAsB[i] = 0;
      numLossesAsB[i] = 0;
      numSquaresEaten[i] = 0;
      numBitesTaken[i] = 0;
      numInvalidBites[i] = 0;
   }
   
   for (barWidth = 3; barWidth <= 6; barWidth += 1)
   {
      for (barHeight = barWidth + 1; barHeight <= 9; barHeight += 1)
      {
         for (i = 0; i < numAgents; i += 1)
         {
            for (j = 0; j < numAgents; j += 1)
            {
               if (i != j)
               {
                  cout << barWidth << "x" << barHeight << " bar, A = "
                       << agentStr[i] << ", B = " << agentStr[j] << ":"
                       << flush;
                  result = playChocolateRussianRouletteGame(agentFunc[i], agentFunc[j], barWidth, barHeight);
                  cout << "Winner: " << result.winner << " ("
                       << (result.winner == 'A' ? agentStr[i] : agentStr[j])
                       << ")\n"
                       << "Squares eaten: A " << result.numSquaresEatenByA
                       << ", B " << result.numSquaresEatenByB << "\n\n";
                  if (result.winner == 'A')
                  {
                     numWinsAsA[i] += 1;
                     numLossesAsB[j] += 1;
                  }
                  else
                  {
                     numLossesAsA[i] += 1;
                     numWinsAsB[j] += 1;
                  }
                  numSquaresEaten[i] += result.numSquaresEatenByA;
                  numSquaresEaten[j] += result.numSquaresEatenByB;
                  numBitesTaken[i] += result.numBitesTakenByA;
                  numBitesTaken[j] += result.numBitesTakenByB;
                  if (result.wasInvalidBiteTaken)
                  {
                     numInvalidBites[result.winner == 'A' ? j : i] += 1;
                  }
               }
            }
         }
      }
   }

   for (i = 0; i < numAgents; i += 1)
   {
      order[i] = i;
   }
   for (i = 0; i < numAgents - 1; i += 1)
   {
      for (j = i + 1; j < numAgents; j += 1)
      {
         if (numWinsAsA[order[i]] + numWinsAsB[order[i]] < numWinsAsA[order[j]] + numWinsAsB[order[j]] || (numWinsAsA[order[i]] + numWinsAsB[order[i]] == numWinsAsA[order[j]] + numWinsAsB[order[j]] &&
             (numSquaresEaten[order[i]] < numSquaresEaten[order[j]] || (numSquaresEaten[order[i]] == numSquaresEaten[order[j]] &&
              (numBitesTaken[order[i]] > numBitesTaken[order[j]] || (numBitesTaken[order[i]] == numBitesTaken[order[j]] &&
               agentStr[order[i]] > agentStr[order[j]]))))))
         {
            temp = order[i];
            order[i] = order[j];
            order[j] = temp;
         }
      }
   }
   cout << "\n"
        << "Overall standings:       all          as A         as B     squares    bites    squares   invalid\n"
        << "                        W    L       W    L       W    L      eaten    taken   per bite     bites\n"
        << fixed;
   for (i = 0; i < numAgents; i += 1)
   {
      cout << setw(20) << left << agentStr[order[i]]
           << " " << setw(4) << right << numWinsAsA[order[i]] + numWinsAsB[order[i]]
           << " " << setw(4) << right << numLossesAsA[order[i]] + numLossesAsB[order[i]]
           << " " << setw(7) << right << numWinsAsA[order[i]]
           << " " << setw(4) << right << numLossesAsA[order[i]]
           << " " << setw(7) << right << numWinsAsB[order[i]]
           << " " << setw(4) << right << numLossesAsB[order[i]]
           << " " << setw(10) << right << numSquaresEaten[order[i]]
           << " " << setw(8) << right << numBitesTaken[order[i]]
           << " " << setprecision(4) << setw(10) << right << static_cast<double>(numSquaresEaten[order[i]]) / numBitesTaken[order[i]];
      if (numInvalidBites[order[i]] > 0)
      {
         cout << " " << setw(9) << right << numInvalidBites[order[i]];
      }
      cout << "\n";
   }
   return 0;
}

GameResult playChocolateRussianRouletteGame(ChocolateSquareLocation (*agentA)(const ChocolateBar &), ChocolateSquareLocation (*agentB)(const ChocolateBar &), int barWidth, int barHeight)
{
   // Play a chocolate Russian roulette game between given agents.
   char player;
   ChocolateBar bar(barWidth, barHeight), previousBar;
   ChocolateSquareLocation bite;
   GameResult result;
   int column, numSquaresEaten, row;
   string barHistory[barHeight];

   for (row = 0; row < barHeight; row += 1)
   {
      barHistory[row] = "|";
   }
   result.numSquaresEatenByA = 0;
   result.numBitesTakenByA = 0;
   result.numSquaresEatenByB = 0;
   result.numBitesTakenByB = 0;
   result.wasInvalidBiteTaken = false;
   for (player = 'A'; bar.isAnyLeft(); player = player == 'A' ? 'B' : 'A')
   {
      bite = (player == 'A' ? *agentA : *agentB)(bar);
      previousBar = bar;
      numSquaresEaten = bar.takeBite(bite);
      if (numSquaresEaten > 0)
      {
         if (player == 'A')
         {
            result.numSquaresEatenByA += numSquaresEaten;
            result.numBitesTakenByA += 1;
         }
         else
         {
            result.numSquaresEatenByB += numSquaresEaten;
            result.numBitesTakenByB += 1;
         }
         for (row = 0; row < barHeight; row += 1)
         {
            barHistory[row] += " ";
            for (column = 0; column < barWidth; column += 1)
            {
               barHistory[row] += bar.isSquareThere(column, row) ? column == 0 && row == 0 ? "X" : "O" : previousBar.isSquareThere(column, row) ? "." : " ";
            }
            barHistory[row] += " |";
         }
      }
      else
      {
         cout << "\n";
         for (row = barHeight - 1; row >= 0; row -= 1)
         {
            cout << barHistory[row] << "\n";
         }
         cout << player << " took an invalid bite (column " << bite.getColumn() << ", row " << bite.getRow() << ").\n";
         numSquaresEaten = bar.takeBite(0, 0);
         if (player == 'A')
         {
            result.winner = 'B';
            result.numSquaresEatenByB += numSquaresEaten;
            result.numBitesTakenByA += 1;
         }
         else
         {
            result.winner = 'A';
            result.numSquaresEatenByA += numSquaresEaten;
            result.numBitesTakenByB += 1;
         }
         result.wasInvalidBiteTaken = true;
         return result;
      }
   }
   cout << "\n";
   for (row = barHeight - 1; row >= 0; row -= 1)
   {
      cout << barHistory[row] << "\n";
   }
   result.winner = player;
   return result;
}
