// YOUR NAME: Ian Sokolnicki
//
// CS 4318, spring 2022
// Agent Challenge 5: Chocolate Russian roulette

#include "crr.h"
#include <vector>

namespace
{
   // Function prototypes.
   vector<int> maxBar(vector<vector<int>>);
   vector<int> minBar(vector<vector<int>>);
   
   // Checks if the chocolate bar is one row.
   bool isOneRow(vector<vector<int>> chocolateBar)
   {
      if (chocolateBar[0][1] == 0)
         return true;
      return false;
   }
   
   // Checks if the chocolate bar is one column.
   bool isOneCol(vector<vector<int>> chocolateBar)
   {
      if (chocolateBar[1][0] == 0)
         return true;
      return false;
   }
   
   // Checks if the chocolate bar is one row and one column of equal length.
   bool isTwoArms(vector<vector<int>> chocolateBar)
   {
      if (chocolateBar[1][1] == 1)
         return false;
      uint r = 0;
      // Checking the size first with double & ensures I don't segment fault.
      while (r < chocolateBar.size() && chocolateBar[r][0] == 1)
         r += 1;
      uint c = 0;
      while (c < chocolateBar[0].size() && chocolateBar[0][c] == 1)
         c += 1;
      if (r == c)
         return true;
      return false;
   }
   
   // Checks if the chocolate bar is two rows, the bottom larger than the top by one.
   bool isTetrisRow(vector<vector<int>> chocolateBar)
   {
      if (chocolateBar[0][2] == 1)
         return false;
      uint r1 = 0;
      while (r1 < chocolateBar.size() && chocolateBar[r1][0] == 1)
         r1 += 1;
      uint r2 = 0;
      while (r2 < chocolateBar.size() && chocolateBar[r2][1] == 1)
         r2 += 1;
      if (r1 - 1 == r2)
         return true;
      return false;
   }
   
   // Checks if the chocolate bar is two column, the left larger than the right by one.
   bool isTetrisCol(vector<vector<int>> chocolateBar)
   {
      if (chocolateBar[2][0] == 1)
         return false;
      uint c1 = 0;
      while (c1 < chocolateBar[0].size() && chocolateBar[0][c1] == 1)
         c1 += 1;
      uint c2 = 0;
      while (c2 < chocolateBar[0].size() && chocolateBar[1][c2] == 1)
         c2 += 1;
      if (c1 - 1 == c2)
         return true;
      return false;
   }
   
   // Given a chocolate bar and where to bite, it return the chocolate with the bite taken out of it.
   vector<vector<int>> takeABite(uint biteX, uint biteY, vector<vector<int>> chocolateBar)
   {
      for (uint i = biteX; i < chocolateBar.size(); i++)
      {
         for (uint j = biteY; j < chocolateBar[0].size(); j++)
         {
            chocolateBar[i][j] = 0;
         }
      }
      return chocolateBar;
   }
   
   // Max function for minimax.
   vector<int> maxBar(vector<vector<int>> chocolateBar)
   {
      // If it is one row or one column, that's a hot position for max.
      if (isOneRow(chocolateBar) || isOneCol(chocolateBar))
      {
         return {INT_MAX, 0, 0};
      }
      // If it is two arms, two rows, or two columns, that's a cold position for max.
      if (isTwoArms(chocolateBar) || isTetrisRow(chocolateBar) || isTetrisCol(chocolateBar))
      {
         return {INT_MIN, 0, 0};
      }

      // INT_MIN will get replaced by anything when maximizing.
      vector<int> best = {INT_MIN, 0, 0};
      // For each piece...
      for (uint i = 0; i < chocolateBar.size(); i++)
      {
         for (uint j = 0; j < chocolateBar[0].size(); j++)
         {
            // ... if it exists...
            if (chocolateBar[i][j] == 1)
            {
               // ... and it's not the poison...
               if ( !(i == 0 && j == 0) )
               {
                              //int originalCount = 0;
                              //int bittenCount = 0;
                  // ... take the min.
                  vector<int> min = minBar(takeABite(i, j, chocolateBar));
                  // We only care about the value given to us by minBar, so replace the 
                  //    dummy row and column coordinates with the real ones.
                  min[1] = i;
                  min[2] = j;
                  // If max found a way to win, immediately quit and return that.
                  if (min[0] == INT_MAX)
                     return min;
                  // Useless at this stage. Since only INT_MIN and INT_MAX are present,
                  //    there will never be a value to replace.
                  if (min[0] > best[0])
                  {
                     best[0] = min[0];
                     best[1] = min[1];
                     best[2] = min[2];
                  }
               }
            }
         }
      }
      // Will return INT_MIN if we didn't find any INT_MAXes.
      return best;
   }
   
   // Min function for minimax.
   vector<int> minBar(vector<vector<int>> chocolateBar)
   {
      // If it is one row or one column, that's a hot position for min.
      if (isOneRow(chocolateBar) || isOneCol(chocolateBar))
      {
         return {INT_MIN, 0, 0};
      }
      // If it is two arms, two rows, or two columns, that's a cold position for min.
      if (isTwoArms(chocolateBar) || isTetrisRow(chocolateBar) || isTetrisCol(chocolateBar))
      {
         return {INT_MAX, 0, 0};
      }
      
      // INT_MAX will get replaced by anything when minimizing.
      vector<int> best = {INT_MAX, 0, 0};
      // For each piece...
      for (uint i = 0; i < chocolateBar.size(); i++)
      {
         for (uint j = 0; j < chocolateBar[0].size(); j++)
         {
            // ... if it exists...
            if (chocolateBar[i][j] == 1)
            {
               // ... and it's not the poison...
               if ( !(i == 0 && j == 0) )
               {
                  // ... take the max.
                  vector<int> max = maxBar(takeABite(i, j, chocolateBar));
                  // We only care about the value given to us by maxBar, so replace the 
                  //    dummy row and column coordinates with the real ones.
                  max[1] = i;
                  max[2] = j;
                  // If min found a way to win, immediately quit and return that.
                  if (max[0] == INT_MIN)
                     return max;
                  // Useless at this stage. Since only INT_MIN and INT_MAX are present,
                  //    there will never be a value to replace.
                  if (max[0] < best[0])
                  {
                     best[0] = max[0];
                     best[1] = max[1];
                     best[2] = max[2];
                  }
               }
            }
         }
      }
      // Will return INT_MAX if we didn't find any INT_MINes.
      return best;
   }
}

/*
   My agent uses a minimax algorithm to determine what the best move is. As we discussed, I didn't realize you had
   the functions that you did, so I wrote mine in terms of a 2D vector. Also, I wanted mine to find wins and losses
   right now instead of finding what simply the best bite would be, so (for maximizing) INT_MAX is a winning move
   and INT_MIN is a losing move. The agent performs minimax, and it takes a bite wherever it find INT_MAX. If it 
   doesn't, then it found INT_MIN, so it takes a small bite, giving the enemy as many chances as possible to screw
   up.
   
   In terms of pruning to reduce time, I had to take some shortcuts. Since I didn't give bites values this 
   iteration, I couldn't do a depth limit. Doing so would cause me to return INT_MIN if a sequence of bites didn't 
   quickly find a win, which would be terrible for my agent. So, I first started with including the special cases
   we talked about in class, forward pruning the tree from any point after a case is found. Additionally, I have my 
   agent take giant bites out of the bar until it gets to a small enough size I can run minimax on without it being
   too slow. This came out to be a 4x6 bar. I'm decently comfy with that size, but I don't have a better
   option with how I structured my agent this iteration.
*/
ChocolateSquareLocation crrAgentIan(const ChocolateBar &bar)
{
   // Save the width and height and create the 2D vector.
   const int width = bar.getWidth();
   const int height = bar.getHeight();
   vector<vector<int>> chocolateBar(width);
   for (int i = 0; i < width; i++)
   {
      chocolateBar[i] = vector<int>(height);
   }
   
   // Populate the 2D vector with pieces where they exist.
   for (int i = 0; i < width; i++)
   {
      for (int j = 0; j < height; j++)
      {
         if (bar.isSquareThere(i, j))
            chocolateBar[i][j] = 1;
         else
            chocolateBar[i][j] = 0;
      }
   }
   
   // Handle the cases in which the bar is just a row or column.
   if (bar.isSquareThere(1, 0) && !bar.isSquareThere(0, 1))
      return ChocolateSquareLocation(1, 0);
   if (!bar.isSquareThere(1, 0) && bar.isSquareThere(0, 1))
      return ChocolateSquareLocation(0, 1);

   // Reduce the bar size if it is too big for minimax.
   if (bar.isSquareThere(4, 0))
      return ChocolateSquareLocation(4, 0);
   if (bar.isSquareThere(0, 6))
      return ChocolateSquareLocation(0, 6);

   // Start minimax.
   vector<int> max = maxBar(chocolateBar);
   
   // If we got a INT_MIN from minimax, then the default value of 0, 0 would be bit. Instead,
   //    find a bite in which only one piece would be eaten to maximize the number of chances 
   //    my opponent can screw up and give me a win. Theoretically, it's better to eat all the
   //    chocolate in terms of goals, but I don't believe that to be important for at least
   //    the first iteration. This runs the risk of passing them a "more" favorable bar (such
   //    as one that we discussed in class), but that really shouldn't matter if I'm projected
   //    to lose anyway.
   if (max[0] == INT_MIN)
   {
      for (int i = width - 1; i >= 0; i--)
      {
         for (int j = height - 1; j >= 0; j--)
         {
            if (bar.isSquareThere(i, j))
               return ChocolateSquareLocation(i, j);
         }
      }
   }
   
   // Return the second and third value in the vector, which are the x and y coordinates of the bite.
   //    These are be default 0, 0, so I should never return an invalid bite either.
   return ChocolateSquareLocation(max[1], max[2]);
}

/*
   Directions:
 - First, carefully comment your code above to clarify how it works.
 - Here, describe your approach and analyze it.  How exactly did you develop
   and test it?  What are its strengths and weaknesses?  Why do you expect
   it to do well against the other submitted agents?
 - Also make a note here if you talked about the assignment with anyone or
   gave or received any kind of help.
   
   Instead of trying to do something sneaky this time, I decided to go directly with the best approach, and man 
   was it hard to code minimax. However, I finished it without any limits to it - which it obviously needs. I 
   first went to take advantage of the patterns we discussed in class. I knew everyone would, even the people
   without minimax, so it was required of me. I used it as a form of forward pruning; if a chocolate bar fit
   one of those parameters, then I returned MAX or MIN dependent on whose turn it would be. It most certainly was
   not enough, as it's only quick enough when the bar is skinny enough for both arms.
   Afterwards, I decided this would be good enough for the first go. So, I did my best to force the bar to be small
   enough to work with my minimax. Basically, I just chomp a huge chunk of the bar. In general, this is an ok move
   to do if I do indeed have a stronger agent. However, if their agent can do a deeper serach than mine can, then
   I'll only be sealing my fate by doing this.
   
   I discussed a few things with Womack, Jordan, and Julio. Nothing too in-depth.
   
   
   
   
   Second iteration, I did nothing extra. I only lost to Clef, and I'm really not sure how to quicken the search.
   I added a depth limit, but I could only do 6 to make it fast enough. However, doing 6 caused it to nibble often
   on the early bites, which isn't great for me. I even had a weird bug where, when the depth was 5, it would bite 
   0, 2 as the opening move every time. I decided it was too risky, and since I already had good results the first 
   time, throwing in the same agent the second time wouldn't be that bad.
   
   Discussed with Mason some about how his agent worked to try to figure out why mine was freaking out, to no avail.

*/




