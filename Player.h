/***************************************************
 *
 * Joseph Krambeer
 * This is for creating the singleton player object
 * that will be used to represent the character controlled
 * by the player
 *
 ***************************************************/
#pragma once

namespace Game
{
    class Player : public Entity
    {
        private:
            Player();//singleton pattern, ensures control of creation of the player
            Player(Player const&) = delete;//copy constructor is not needed so it's deleted
            void operator =(Player const&) = delete;//assignment operator is not needed so it's deleted

            void switchDisplays();
            void exitGame();
        public:
            static Player& getInstance()//this will get and create the one and only player object
            {
                static Player playerInstance;//this is not deleted between calls, rather it is initialized on the 1st call then
                                             //the same object is returned by this function every subsequent call
                return playerInstance;
            }//getInstance

            void setHealth(short newHealth);
            void getPlayerAction();//this will read inputs from the player and act on the accordingly
            void moveEntity(int direction) override;
    };

    Player::Player() : Entity(ThePlayer,NoStatus,10,10,50,12,50,std::string("Player"),L'☺')//is made only be calling the entity constructor with certain set parameters
    {
        //constructor ( which should only be used once )
    }

    void Player::setHealth(short newHealth) //test
    {
        if(newHealth > 0){ this->setHealth(newHealth); }
        else             { exitGame(); }
    }//updateHealth

    void Player::moveEntity(int direction)//call this with getPlayerAction or an event
    {
        moveDisplay(direction);//needs to call a function defined below to have access to the objects needed to check the map chars
    }//moveEntity

    void Player::getPlayerAction()
    {
        int   input;
        Mode  currentMode;
        Mode  newMode;
        Byte  modeNumber;
        bool  directionalCheck;

        input            = getch();
        currentMode      = getGameMode();
        directionalCheck = (input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN);

        if(currentMode == display) //probably break input interpreting into another function
        {
            if     (directionalCheck)  { moveDisplay(input ); }
            else if(input == KEY_ESC)  { switchDisplays();    }
            else if(input == KEY_F(2) ){ clearMessages();     }
            else if(input == KEY_F(1) ){ exitGame();          }
            else if(input == 'c')      { cutDownTrees( this->getYCord(), this->getXCord() ); }
        }
        else //probably break screen handling into another function
        {
            modeNumber = (Byte)currentMode;
            if( input == KEY_DOWN )
            {
                if(modeNumber == 3){modeNumber = 1;}
                else               {modeNumber = modeNumber+1;}
                newMode = (Mode)modeNumber;
                updateDisplayMode( newMode );
            }//key down
            else if( input == KEY_UP)
            {
                if(modeNumber == 1){modeNumber = 3;}
                else               {modeNumber = modeNumber-1;}
                newMode = (Mode)modeNumber;
                updateDisplayMode( newMode );
            }//key up
            else if( input == KEY_ESC)
            {
                switchDisplays();
            }
        }//not display
    }//getPlayerAction

//-----Other Methods-----

    void Player::switchDisplays()
    {
        Mode currentMode;

        currentMode = getGameMode();
        if( currentMode == display){ updateDisplayMode(playerMenu);}
        else                       { updateDisplayMode(display);   }
    }//switchDisplays

    void Player::exitGame()
    {
        clear();//curses command to clear everything on the screen (thanks namespace)
        mvprintw(1,1,"Game Ended, press any key to exit");//tell player it's game over
        //getch();//pause until they hit a key
        endwin();
        exit(0);//exit the program
    }

}//namespace game
