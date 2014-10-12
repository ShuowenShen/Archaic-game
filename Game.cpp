//
//  Game.cpp
//  project 3
//
//  Created by shen sw on 14-5-10.
//  Copyright (c) 2014年 shen sw. All rights reserved.
//

#include "Game.h"
#include "Board.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "Player.h"
using namespace std;


Game::Game(const Board& b, Player* south, Player* north):m_b(b)
{
    m_splayer = south;
    m_nplayer = north;
    last = NORTH;
    m_winner = SOUTH;
    m_hasWinner = false;
}

void Game::display() const
{
    cout<<"      "<< m_nplayer -> name() <<endl;
    cout<<"   ";
    for(int i = 1; i <= m_b.holes(); i++)
    {
        cout<<m_b.beans(NORTH, i)<<"  ";
    }
    cout<<endl;
    cout<<" "<<m_b.beans(NORTH, 0);
    for(int i = 1; i <= m_b.holes(); i++)
    {
        cout<<"   ";
    }
    cout<<m_b.beans(SOUTH, 0)<<endl;
    cout<<"   ";
    for(int i = 1; i <= m_b.holes(); i++)
    {
        cout<<m_b.beans(SOUTH, i)<<"  ";
    }
    cout<<endl;
     cout<<"      "<< m_splayer -> name() <<endl;
}
//Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board.
void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    if(m_b.beansInPlay(SOUTH) != 0 && m_b.beansInPlay(NORTH) != 0)  //if the game is not over
    {
        over = false;
    }
    else
    {
        over = true;
        if((m_b.beansInPlay(SOUTH)+m_b.beans(SOUTH, 0)) == (m_b.beansInPlay(NORTH)+m_b.beans(NORTH, 0))) //if there is no winner
            hasWinner = false;
        else
        {
            hasWinner = true;
            winner = (m_b.beansInPlay(SOUTH)+m_b.beans(SOUTH, 0))>(m_b.beansInPlay(NORTH)+m_b.beans(NORTH, 0))? SOUTH:NORTH;
        }
    }
}
//If the game isn't over (i.e., more moves are possible), set over to false and do not change anything else. Otherwise, set over to true and hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
bool Game::move()
{
    bool m_over;
    Side m_endSide;
    int m_endHole = 1;
    status(m_over, m_hasWinner, m_winner);
    if(m_over == true)
        return false;
    if(last == NORTH)
    {
    for(;;)
    {
        int a;
        if(m_splayer->isInteractive() == true)  //if it is a humanplayer
        {
            
              a = m_splayer->chooseMove(m_b, SOUTH);
              m_b.sow(SOUTH, a, m_endSide, m_endHole);
        }
        else
        {
            int b = m_splayer->chooseMove(m_b, SOUTH);
            m_b.sow(SOUTH, b, m_endSide, m_endHole);
            cout<<m_splayer->name()<<" chooses hole "<< b <<endl;
        }
    if(m_endHole == 0)
    {
        display();
        status(m_over, m_hasWinner, m_winner);
        if(m_over == true)  // if the game is over
        {
            if(m_b.beansInPlay(SOUTH) == 0 && m_b.beansInPlay(NORTH) != 0)
            {
                cout<<"Sweeping remaining beans into "<<m_nplayer->name()<<"'s pot."<<endl;

            for(int i =1; i<=m_b.holes();i++)  // move the beans into the pot
            {
                m_b.moveToPot(NORTH, i, NORTH);
            }
            display();
            }
            return false;
        }
        cout<<m_splayer->name()<<" gets another turn."<<endl;
        continue;
    } // if there is a capture
    else if(m_endSide == SOUTH && m_endHole != 0 && m_b.beans(m_endSide, m_endHole) == 1 && m_b.beans(NORTH, m_endHole) != 0)
    {
        m_b.moveToPot(NORTH, m_endHole, SOUTH);
        m_b.moveToPot(SOUTH, m_endHole, SOUTH);
        display();
        status(m_over, m_hasWinner, m_winner);
        if(m_over == true)  // if the game is over
        {
            if(m_b.beansInPlay(SOUTH) == 0 && m_b.beansInPlay(NORTH) != 0)
            {
                cout<<"Sweeping remaining beans into "<<m_nplayer->name()<<"'s pot."<<endl;
                
                for(int i =1; i<=m_b.holes();i++)
                {
                    m_b.moveToPot(NORTH, i, NORTH);
                }
                display();
            }

            return false;
        }
        last = opponent(last);
        return true;
    }
    
    else  //other conditions
    {
        status(m_over, m_hasWinner, m_winner);
        if(m_over == true)  // if the game is over
        {
            display();
            if(m_b.beansInPlay(SOUTH) == 0 && m_b.beansInPlay(NORTH) != 0)
            {
                cout<<"Sweeping remaining beans into "<<m_nplayer->name()<<"'s pot."<<endl;
            for(int i =1; i<=m_b.holes();i++)
            {
                m_b.moveToPot(NORTH, i, NORTH);
            }
            display();
            }
            return false;
        }
        display();
        last = opponent(last);
        return true;
    }
    }
    }
    else if(last == SOUTH) // if the last one is the south
    {
        for(;;)
        {
            int a;
            int b;
            if(m_nplayer->isInteractive() == true)  // if it is a humanplayer
            {
                
                a = m_nplayer->chooseMove(m_b, NORTH);
                m_b.sow(NORTH, a, m_endSide, m_endHole);
            }
            else  // if it is not a humanplayer
            {
                b = m_nplayer->chooseMove(m_b, NORTH);
                m_b.sow(NORTH, b, m_endSide, m_endHole);
                cout<<m_nplayer->name()<<" chooses hole "<< b <<endl;
            }
            if(m_endHole == 0)
            {
                status(m_over, m_hasWinner, m_winner);
                if(m_over == true)  // if hte game is over
                {
                    if(m_b.beansInPlay(NORTH) == 0 && m_b.beansInPlay(SOUTH) != 0)
                    {
                        cout<<"Sweeping remaining beans into "<<m_splayer->name()<<"'s pot."<<endl;

                    for(int i =1; i<=m_b.holes();i++)
                    {
                        m_b.moveToPot(SOUTH, i, SOUTH);
                    }
                    display();
                    }
                    return false;
                }
                display();
                cout<<m_nplayer->name()<<" gets another turn."<<endl;
                continue;
            }
            else if(m_endSide == NORTH && m_endHole != 0 && m_b.beans(m_endSide, m_endHole) == 1 && m_b.beans(SOUTH, m_endHole) != 0)  // if there is a capture
            {
                m_b.moveToPot(SOUTH, m_endHole, NORTH);
                m_b.moveToPot(NORTH, m_endHole, NORTH);
                display();
                status(m_over, m_hasWinner, m_winner);
                if(m_over == true)
                {
                    if(m_b.beansInPlay(NORTH) == 0 && m_b.beansInPlay(SOUTH) != 0)
                    {
                        cout<<"Sweeping remaining beans into "<<m_splayer->name()<<"'s pot."<<endl;
                        
                        for(int i =1; i<=m_b.holes();i++)
                        {
                            m_b.moveToPot(SOUTH, i, SOUTH);
                        }
                        display();
                    }

                    return false;
                }
                last = opponent(last);
                return true;
            }
            else
            {
                status(m_over, m_hasWinner, m_winner);
                if(m_over == true)  // if the game is over
                {
                    display();
                    if(m_b.beansInPlay(NORTH) == 0 && m_b.beansInPlay(SOUTH) != 0)  // if there is remaining beans
                    {
                        cout<<"Sweeping remaining beans into "<<m_splayer->name()<<"'s pot."<<endl;
                    for(int i =1; i<=m_b.holes();i++)
                    {
                        m_b.moveToPot(SOUTH, i, SOUTH);
                    }
                    display();
                    }
                    return false;
                }
                display();
                last = opponent(last);
                return true;
            }
        }

    }
    return false;
}
//If the game is over, return false. Otherwise, make a complete move for the player whose turn it is (so that it becomes the other player's turn) and return true. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. If the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening.
void Game::play()
{
    display();
    for(;;)
    {
    if(m_splayer ->isInteractive() ==false && m_nplayer->isInteractive() == false)  // if there is no humanplayer
    {
       if(move() == true)
       {
           cout<<"Press ENTER to continue";
           cin.ignore(10000, '\n');
       }
       else
       {
          
           if(m_b.beansInPlay(NORTH) == 0 && m_b.beansInPlay(SOUTH) != 0)  //if the game is over
           {
               cout<<"Sweeping remaining beans into "<<m_splayer->name()<<"'s pot."<<endl;
               for(int i =1; i<=m_b.holes();i++)
               {
                   m_b.moveToPot(SOUTH, i, SOUTH);
               }
               display();
           }
           if(m_b.beansInPlay(SOUTH) == 0 && m_b.beansInPlay(NORTH) != 0)  // if the game is over
           {
               cout<<"Sweeping remaining beans into "<<m_nplayer->name()<<"'s pot."<<endl;
               for(int i =1; i<=m_b.holes();i++)
               {
                   m_b.moveToPot(NORTH, i, NORTH);
               }
               display();
           }

           cout<<"Press ENTER to continue";
           cin.ignore(10000, '\n');
           break;
       }
    }
    else
    {
    if(move() == false)  // if the move() returns false
    {
       
        if(m_b.beansInPlay(NORTH) == 0 && m_b.beansInPlay(SOUTH) != 0) // if the game is over
        {
            cout<<"Sweeping remaining beans into "<<m_splayer->name()<<"'s pot."<<endl;
            for(int i =1; i<=m_b.holes();i++)
            {
                m_b.moveToPot(SOUTH, i, SOUTH);
            }
            display();
        }
        if(m_b.beansInPlay(SOUTH) == 0 && m_b.beansInPlay(NORTH) != 0)  // if the game is over
        {
            cout<<"Sweeping remaining beans into "<<m_nplayer->name()<<"'s pot."<<endl;
            for(int i =1; i<=m_b.holes();i++)
            {
                m_b.moveToPot(NORTH, i, NORTH);
            }
            display();
        }

        break;
    }
    }
    }
    if(m_hasWinner == false)   // if there is no winner
        cout<<"The game resulted in a tie."<<endl;
    else{
    if(m_winner == SOUTH)
        cout<<"The winner is "<<m_splayer->name()<<endl;
    else
        cout<<"The winner is "<<m_nplayer->name()<<endl;
      }
}


//Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function.

int Game::beans(Side s, int hole) const
{
    if(hole < 0 || hole >m_b.holes())  // if the hole is not valid
    {
        return -1;
    }
    return m_b.beans(s, hole);
}
 
//Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
