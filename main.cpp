//
//  main.cpp
//  project 3
//
//  Created by shen sw on 14-5-10.
//  Copyright (c) 2014年 shen sw. All rights reserved.
//

#include "Board.h"
#include "Player.h"
#include "Side.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <stack>
using namespace std;

int evaluation(Side s,const Board& b)
{
    Board test = b;
    if(b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0) // if the game is over
    {
        if (b.beansInPlay(SOUTH) + b.beans(SOUTH, 0) - b.beansInPlay(NORTH) - b.beans(NORTH, 0) > 0)  // if opponent have more beans
            return 1000;
        if ((b.beansInPlay(SOUTH) + b.beans(SOUTH, 0) - b.beansInPlay(NORTH) - b.beans(NORTH, 0)) < 0) // if opponent have fewer beans
            return -1000;
        if ((b.beansInPlay(SOUTH) + b.beans(SOUTH, 0) - b.beansInPlay(NORTH) - b.beans(NORTH, 0)) == 0) // if both sides have the same beans
            return 0;
    }
    if((b.beans(SOUTH, 0) - b.beans(NORTH, 0) - b.beansInPlay(SOUTH) - b.beansInPlay(NORTH)) > 0)
        return 1000;
    if((b.beans(NORTH, 0) - b.beans(SOUTH, 0) - b.beansInPlay(SOUTH) - b.beansInPlay(NORTH)) > 0)
       return -1000;
    //return (b.beans(SOUTH, 0) - b.beans(NORTH, 0));
    
    int south1 = 0;
    int north1 = 0;
    if(s == SOUTH)
    {
        for(int i = 1; i <= b.holes(); i++)  // find the largest capture
        {
            test = b;
            Side m_endSide;
            int m_endHole = 0;
            if( 0 < test.beans(SOUTH, i))
            {
                test.sow(SOUTH, i, m_endSide, m_endHole);
                if(m_endSide == SOUTH && m_endHole != 0 && test.beans(SOUTH, m_endHole) == 1 && test.beans(NORTH, m_endHole) != 0)
                {
                    if( test.beans(NORTH, m_endHole) > south1)
                    {
                        south1 = test.beans(NORTH, m_endHole);
                    }
                }
            }
        }
        if(south1 != 0) //if there is capture
            return (south1 + 1 + b.beans(SOUTH, 0) - b.beans(NORTH, 0)); //get the sum of the difference of beans in the pot and the largest capture
        else  // if there is no capture
            return (b.beans(SOUTH, 0) - b.beans(NORTH, 0));  //get the sum of the difference of beans in the pot
    }
    else if (s == NORTH)
    {
        for(int i = 1; i <= b.holes(); i++)
        {
            test = b;
            Side m_endSide;
            int m_endHole = 0;
            if( 0 < b.beans(NORTH, i))
            {
                test.sow(NORTH, i, m_endSide, m_endHole);
                if(m_endSide == NORTH && m_endHole != 0 && test.beans(NORTH, m_endHole) == 1 && test.beans(SOUTH, m_endHole) != 0 )
                {
                    if( test.beans(SOUTH, m_endHole) > north1)
                    {
                        north1 = test.beans(SOUTH, m_endHole);
                    }
                }
            }
            
        }
        if(north1 != 0)//if there is capture
            return ( -north1 - 1 + b.beans(SOUTH, 0) - b.beans(NORTH, 0));  //get the sum of the difference of beans in the pot and the largest capture
        else  // if there is no capture
            return (b.beans(SOUTH, 0) - b.beans(NORTH, 0));   //get the sum of the difference of beans in the pot
    }
    
    
    return 0;
}



void choose(Side s, int& bestHole, int& value, int deepsearch, const Board& b,  std::stack<Board>& testStack,
            std::stack<int>& bestMove, std::stack<int>& numberinperdeepth )
{
    if(b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0) // if the game is over
    {
        bestHole = -1;
        value = evaluation(s, b);
        return;
    }
    
    if(deepsearch == 3)  // if the depth is reached
    {
        bestHole = -1;
        value = evaluation(s, b);
        return;
    }
    
    Board test = b;
    Side m_endSide;
    int m_endHole = 0;
    int testi = 1;
    int testboard[100];
    for(int i = 1; i <= test.holes(); i++) //find the legal move
    {
        if(test.beans(s, i) > 0)
        {
            testboard[testi] = i;
            testi++;
        }
    }
    for(int i = 1; i < testi; i++)  //start the loop for every legal move
    {
        int deep = deepsearch;
        test = b;
        test.sow(s, testboard[i], m_endSide, m_endHole);
            if(m_endHole == 0)  // if the endhole is 0
            {
                if(test.beansInPlay(SOUTH) == 0 || test.beansInPlay(NORTH) == 0) // if hte game is over
                {
                    testStack.push(test);
                    bestMove.push(testboard[i]);
                    numberinperdeepth.push(deepsearch);
                   
                }
                choose(s, bestHole, value, deepsearch, test, testStack, bestMove, numberinperdeepth); //get another move in the sam deepth
            }
            else if(m_endSide == s && m_endHole != 0 && test.beans(m_endSide, m_endHole) == 1 && test.beans(opponent(s), m_endHole) != 0)//if there is a capture
            {
                test.moveToPot(NORTH, m_endHole, s);
                test.moveToPot(SOUTH, m_endHole, s);
                bestMove.push(testboard[i]);
                numberinperdeepth.push(deepsearch);
                testStack.push(test);
               
            }
            else //other case
            {
                bestMove.push(testboard[i]);
                numberinperdeepth.push(deepsearch);
                testStack.push(test);
            }
        
 
        deep++;  // increment the deepth
        int bestmove2;
        int bestvalue2;
        choose(opponent(s), bestmove2, bestvalue2, deep, test, testStack, bestMove, numberinperdeepth); // get into the next depth
        test = b;
        
        if(deepsearch %2 == 0) //if the deep is on this side
        {
            if(bestvalue2 > value) // if the value is lager
            {
                value = bestvalue2;
                bestHole = testboard[i];
            }
        }
        if(deepsearch %2 == 1)  // if teh deep is on another side
        {
            if(bestvalue2 < value)  // if hte value is smaller
            {
                value = bestvalue2;
                bestHole = testboard[i];
            }
        }
        
    }
    return;
}


int main()
{
        HumanPlayer hp("Marge");
        SmartPlayer bp("Homer");
        Board b(3,2);
		Game g(b, &hp, &bp);
		g.play();
}



#if __cplusplus == 201103L  // C++11

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

#elif defined(_MSC_VER)  // not C++11, but Windows

#include <windows.h>

class Timer
{
public:
    Timer()
    {
        QueryPerformanceFrequency(&ticksPerSecond);
        start();
    }
    void start()
    {
        QueryPerformanceCounter(&m_time);
    }
    double elapsed() const
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000.0 * (now.QuadPart - m_time.QuadPart)) / ticksPerSecond.QuadPart;
    }
private:
    LARGE_INTEGER m_time;
    LARGE_INTEGER ticksPerSecond;
};

#else // not C++11 or Windows, so C++98

#include <ctime>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::clock();
    }
    double elapsed() const
    {
        return (1000.0 * (std::clock() - m_time)) / CLOCKS_PER_SEC;
    }
private:
    std::clock_t m_time;
};

#endif

#include <iostream>
using namespace std;

Timer timer;

void f(bool& startWrappingItUp)
{
    if (!startWrappingItUp)
    {
        for (int k = 0; k < 1000000; k++)
        {
            if (timer.elapsed() > 4900)  // have 4.9 seconds elapsed?
            {
                startWrappingItUp = true;
                return;
            }
            BadPlayer hp("Marge");
            SmartPlayer bp("Homer");
            Board b(3,2);
            bp.chooseMove(b, SOUTH);
        }
    }
}

int main()
{
    cout << "You should see something in close to 5 seconds...\n";
    bool shouldWrapUp = false;
    timer.start();
    int ncalls;
    for (ncalls = 0; !shouldWrapUp; ncalls++)
        f(shouldWrapUp);
    cout<<timer.elapsed();
    cout << "f was called " << ncalls << " times\n";
}
