//
//  Board.cpp
//
//  Created by shen sw on 14-5-10.
//  Copyright (c) 2014年 shen sw. All rights reserved.
//

#include "Board.h"
#include "Side.h"
#include <iostream>
#include <string>
#include <cstdlib>
//#include <map>
using namespace std;
Board::Board(int nHoles, int nInitialBeansPerHole)
{
    if(nHoles<=0 || nInitialBeansPerHole < 0)  // if the holeis not valid or the initialized beans is negative
        exit(1);
    m_nHoles = nHoles;
    North[0] = 0;  // set the holes.
    South[0] = 0;
    for(int i = 1; i <= nHoles; i++)  // initialize the holes
    {
        North[i] = nInitialBeansPerHole;
        South[i] = nInitialBeansPerHole;
    }
}
//Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole.
int Board::holes() const
{
    return m_nHoles;
}
//Return the number of holes on a side (not counting the pot).
int Board::beans(Side s, int hole) const
{
    if (hole < 0 || hole > m_nHoles)  // if the hole is not valid
        return -1;
    if(s == SOUTH)
        return South[hole];
    else
        return North[hole];
}
//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
int Board::beansInPlay(Side s) const
{
    int total = 0;
    for(int i = 1; i <= m_nHoles; i++)
    {
        if(s == SOUTH)
        {
            total += South[i];   //get the sum of the south
        }
        else
        {
            total += North[i];   //get the sum of the north
        }
    }
    return total;
}
//Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
int Board::totalBeans() const
{
    int total = 0;
    for(int i = 0; i<= m_nHoles; i++)
    {
        total += South[i];  //get the sum of beans in two sides
        total += North[i];
    }
    return total;
}
//Return the total number of beans in the game.
bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    if(hole <= 0 || hole >m_nHoles || ((s== SOUTH)&&(South[hole] == 0)) || ((s== NORTH)&&(North[hole] == 0)))  // if the hole is not valid or the hole is empty
        return false;
    int n;
    if (s == SOUTH)   // record the number of beans and clear the beans in the hole
    {
        n = South[hole];
        South[hole] = 0;
    }
    else
    {
        n = North[hole];
        North[hole] = 0;
    }

       if(s == SOUTH)
        {
            for(int i = hole+1; i<= m_nHoles; i++)  //set the beans in the next successive holes
            {
                South[i] ++;
                n--;
                endHole = i;
                endSide = SOUTH;
                if(n==0)
                    return true;
            }
         while(n!=0)  //if it is not empty
              {
                South[0]++;
                n--;
                endHole = 0;
                endSide = SOUTH;
                if(n==0)
                    return true;
            for(int i = m_nHoles; i>=1; i--)  //it is in the range
            {
                North[i]++;
                n--;
                endSide = NORTH;
                endHole = i;
                if(n==0)
                    return true;
            }
            for(int i = 1; i <= m_nHoles; i++)  //it is in the range
                  {
                      South[i]++;
                      n--;
                      endSide = SOUTH;
                      endHole = i;
                      if(n==0)
                          return true;
                  }
              }
          }
        else //if it is north
        {
            for(int i = hole-1; i >= 1; i--) //record the number of beans
            {
                North[i]++;
                n--;
                endHole = i;
                endSide = NORTH;
                if(n==0)
                    return true;
            }
            while(n!=0)    // set the beans in the next successive holes
            {
                North[0]++;
                n--;
                endHole = 0;
                endSide = NORTH;
                if(n==0)
                    return true;
                for(int i = 1; i<=m_nHoles; i++)  // it is in the range
                {
                    South[i]++;
                    n--;
                    endSide = SOUTH;
                    endHole = i;
                    if(n==0)
                        return true;
                }
                for(int i = m_nHoles; i >= 1; i--)  // it is in the range
                {
                    North[i]++;
                    n--;
                    endSide = NORTH;
                    endHole = i;
                    if(n==0)
                        return true;
                }
            }
        }
    return false;
}
//If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The parameters endSide and endHole are set to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if(hole <= 0 || hole >m_nHoles)  //if hte hole is not valid
        return false;
    if (potOwner == SOUTH)
    {
        if(s == SOUTH)     //get the beans in the hole into the pot and set the original hole to empty
        {
            South[0] += South[hole];
            South[hole] = 0;
        }
        else
        {
            South[0] += North[hole];
            North[hole] = 0;
        }
    }
    else if (potOwner == NORTH)  //get the beans in the hole into the pot and set the original hole to empty
    {
        if(s == SOUTH)
        {
            North[0] += South[hole];
            South[hole] = 0;
        }
        else
        {
            North[0] += North[hole];
            North[hole] = 0;
        }
    }
    return true;
}
//If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
bool Board::setBeans(Side s, int hole, int beans)
{
    if(hole < 0 || hole > m_nHoles || beans < 0)  // if the hole is invalide or the beans is negative
        return false;
    if(s == SOUTH)
        South[hole] = beans;  // set the beans in htis hole
    else
        North[hole] = beans;
    return true;
}
//If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This may change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)

