/***************************************************
 *
 * Joseph Krambeer
 * This is for holding y/x cords and the corresponding
 * action that happen when the player is on the tile
 *
 ***************************************************/
#pragma once

Game::GameMap getCurrentGameMap();

//! ADD A WAY TO SPAWN IN ENTITIES FROM EVENTS AFTER ENTIES ARE IMPLEMENTED
//! It should be a new event type

namespace Game
{
    #define noMapChange -1

    std::string replaceUnderscore(std::string input);
    void        printEventMessage(const char* message);
    void        updateMapTitle(MapIndex mapNumber);


/*-----MAP EVENT-----*/

    class MapEvent
    {
        private:
            std::string  messageText;
            short        yCord;
            short        xCord;
            short        yTarget;
            short        xTarget;

        public:
            MapEvent();
            MapEvent(short y,short x,short y2,short x2,std::string message);

            short        getYCord();
            short        getXCord();
            short        getYTarget();
            short        getXTarget();
            std::string  getMessage();
            virtual void applyEvent()=0;//virtual method so this is abstract
    };//class MapEvent

    MapEvent::MapEvent(){}//default constructor

    MapEvent::MapEvent(short y,short x,short y2,short x2,std::string message)
    {
        this->messageText = message;
        this->yCord       = y;
        this->xCord       = x;
        this->yTarget     = y2;
        this->xTarget     = x2;
    }//MapEvent constructor

    short MapEvent::getYCord()
    {
        return this->yCord;
    }//getYCord

    short MapEvent::getXCord()
    {
        return this->xCord;
    }//getXCord

    short MapEvent::getYTarget()
    {
        return this->yTarget;
    }

    short MapEvent::getXTarget()
    {
        return this->xTarget;
    }

    std::string MapEvent::getMessage()
    {
        return this->messageText;
    }//getMessage

/*-----TILE EVENT-----*/

    class TileEvent: public MapEvent
    {
        private:
            short       damage;
            cchar_t     mapTile;

        public:
            TileEvent();
            TileEvent(short y,short x, std::string message, short newY, short newX, short dmg, cchar_t tile);

            short       getDamage();
            cchar_t     getMapTile();
            void        applyEvent() override;//override the parent class's virtual method

    };//class TileEvent

    TileEvent::TileEvent()
    {
        //default constructor
    }

    TileEvent::TileEvent(short y,short x, std::string message, short newY, short newX, short dmg, cchar_t tile):MapEvent(y,x,newY,newX,message)//this invokes the superclass constructor
    {
        this->damage  = dmg;
        this->mapTile = tile;
    }//TileEvent constructor

    void TileEvent::applyEvent() //this is gonna need to be fixed sometime with all these gets to global variables
    {
        std::vector<cchar_t> mapChars;
        const char*          message;

        if(getMapTile()!= -1)
        {
            mapChars = getCurrentGameMap().getMapChars();
            mapChars.at( YXCordToVectorPosition( getYTarget(),getXTarget() ) ) = getMapTile();
            updateMapCharacters( mapChars );
        }

        //damage stuff here

        message = replaceUnderscore(getMessage() ).c_str();
        printEventMessage(message);
    }//applyEvent

    short TileEvent::getDamage()
    {
        return this->damage;
    }//getDamage

    cchar_t TileEvent::getMapTile()
    {
        return this->mapTile;
    }//getMapTile

/*-----MOVE EVENT-----*/

    class MoveEvent: public MapEvent
    {
        private:
            MapIndex mapIndex;

        public:
            MoveEvent();
            MoveEvent(short y, short x, std::string message, short newY, short newX, MapIndex mapNum );

            MapIndex getMapIndex();
            void     applyEvent() override;//override the parent class's virtual method

    };//class MoveEvent

    MoveEvent::MoveEvent()
    {
        //default constructor
    }

    MoveEvent::MoveEvent(short y, short x, std::string message, short newY, short newX, MapIndex mapNum ):MapEvent(y,x,newY,newX,message)
    {
        this->mapIndex = mapNum;
    }//moveEvent constructor

    void MoveEvent::applyEvent()
    {
        const char* message;

        if( getMapIndex() == noMapChange )
        {
            setPlayerYX(getYTarget(),getXTarget() );
            cordinateDisplayTerrain( getYTarget(),getXTarget() );
        }//is -1
        else
        {
            setMapNumber( getMapIndex() );
            setPlayerYX(getYTarget(),getXTarget() );
            cordinateDisplayTerrain( getYTarget(),getXTarget() );
            updateMapTitle( getMapIndex() );
        }//isnt -1

        message = replaceUnderscore(getMessage() ).c_str();
        printEventMessage(message);
    }//applyEvent

    MapIndex MoveEvent::getMapIndex()
    {
        return this->mapIndex;
    }//getMapIndex

/*-----HELPER FUNCTIONS-----*/

    std::string replaceUnderscore(std::string input)
    {
        std::string newString;

        newString = input;
        for(short i=0;i<newString.size();i++)
        {
            if(newString.at(i)=='_'){newString.replace(i,1," ");}
        }
        return newString;
    }//replaceUnderscore

    void printEventMessage(const char* message)
    {
        WINDOW* dialogWindow;

        dialogWindow = getDialogWindow();
        mvwprintw(dialogWindow,3,10,message);
        wrefresh(dialogWindow);
    }//printEventMessage

    void updateMapTitle(MapIndex mapNumber)
    {
        WINDOW* dialogWindow;
        char    stringBuff[100];

        dialogWindow = getDialogWindow();
        sprintf(stringBuff,"Map %d", mapNumber );

        wattron(dialogWindow,A_BOLD);
        mvwprintw(dialogWindow,1,2,stringBuff);
        wattroff(dialogWindow,A_BOLD);
    }//updateMapTitle

}//namespace Game
