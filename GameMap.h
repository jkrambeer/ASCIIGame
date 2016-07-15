/***************************************************
 *
 * Joseph Krambeer
 * This is for making the map class which holds
 * map info that the game uses to display the terrain.
 *
 ***************************************************/
#pragma once

namespace Game
{
    short                getMapHeight(std::vector<cchar_t> mapVector);
    short                getMapWidth(std::vector<cchar_t> mapVector);
    void                 throwAwayEncodingChars(FILE* file);
    std::vector<cchar_t> getMapChars(FILE* file);
    std::vector<cchar_t> removeNewLineChars(std::vector<cchar_t> input);
    std::vector<cchar_t> replaceCharCodes(std::vector<cchar_t> inputVector);

    class GameMap
    {
        private:
            std::vector<cchar_t> charVector;
            short                mapHeight;
            short                mapWidth;

            short                getMapHeight(std::vector<cchar_t> mapVector);
            short                getMapWidth(std::vector<cchar_t> mapVector);
            void                 throwAwayEncodingChars(FILE* file);
            std::vector<cchar_t> getMapChars(FILE* file);
            std::vector<cchar_t> removeNewLineChars(std::vector<cchar_t> input);
            std::vector<cchar_t> replaceCharCodes(std::vector<cchar_t> inputVector);

        public:
            GameMap();
            GameMap(FILE *mapFile);

            short                getWidth();
            short                getHeight();
            std::vector<cchar_t> getMapChars();
            void                 setMapChars(std::vector<cchar_t>);

    };//GameMap class

/* -----GAMEMAP FUNCTIONS----- */
    GameMap::GameMap()
    {
        //default contructor
    }

    GameMap::GameMap(FILE *mapFile)
    {
        throwAwayEncodingChars(mapFile);
        this->charVector = getMapChars(mapFile);
        this->charVector = replaceCharCodes(charVector);

        this->mapHeight  = getMapHeight(charVector);
        this->mapWidth   = getMapWidth(charVector);

        this->charVector = removeNewLineChars(charVector);
    }//constructor

    short GameMap::getWidth()
    {
       return this->mapWidth;
    }//getWidth

    short GameMap::getHeight()
    {
        return this->mapHeight;
    }//getHeight

    std::vector<cchar_t> GameMap::getMapChars()
    {
        return this->charVector;
    }//getMapChars

    void GameMap::setMapChars(std::vector<cchar_t> newChars)
    {
        this->charVector = newChars;
    }//setMapChars



/* -----HELPER FUNCTIONS (private)----- */

    short GameMap::getMapHeight(std::vector<cchar_t> mapVector)
    {
        short height;

        height=0;
        for(cchar_t currentChar: mapVector)
        {
            if(currentChar=='\n'){height++;}//height counting
        }
        return height;
    }//getMapHeight

    short GameMap::getMapWidth(std::vector<cchar_t> mapVector)
    {
        short width;

        width=0;
        for(short i=0;mapVector.at(i)!='\n';i++)
        {
            width++;
        }
        return width;
    }//getMapWidth

    std::vector<cchar_t> GameMap::getMapChars(FILE* file)
    {
        std::vector<cchar_t> readChars;
        cchar_t         currentChar;

        currentChar = fgetwc(file);
        while(currentChar != WEOF)
        {
            readChars.push_back(currentChar);
            currentChar = fgetwc(file);
        }
        return readChars;
    }//getMapChars

    std::vector<cchar_t> GameMap::removeNewLineChars(std::vector<cchar_t> input)
    {
        std::vector<cchar_t> newVector;

        newVector = input;
        for(short i=0;i<newVector.size();i++)
        {
            if(newVector.at(i)=='\n'){newVector.erase(newVector.begin()+i,newVector.begin()+i+1);}//deletes the newline character
        }
        return newVector;
    }//removeNewLineChars

    void GameMap::throwAwayEncodingChars(FILE* file)
    {
        for(Byte i=0;i<3;i++)
        {
           fgetwc(file);
        }
    }//throwAwayEncodingChars

    std::vector<cchar_t> GameMap::replaceCharCodes(std::vector<cchar_t> inputVector)
    {
        std::vector<cchar_t> newVector;
        std::array<int,3>    tempArray;
        std::array< std::array<int,3> ,38> values =
        {
         {
            {226,152,186},//☺
            {226,152,187},//☻
            {226,150,178},//▲
            {226,150,188},//▼
            {226,150,186},//◄
            {226,151,132},//►
            {226,150,160},//■
            {226,153,163},//♣
            {226,153,160},//♠
            {226,153,166},//♦
            {226,128,162},//•
            {226,151,152},//◘
            {226,151,139},//○
            {226,152,188},//☼
            {226,150,172},//▬
            {226,137,136},//≈
            {226,149,145},//║
            {226,149,151},//╗
            {226,149,157},//╝
            {226,149,154},//╚
            {226,149,148},//╔
            {226,149,144},//═
            {226,149,163},//╣
            {226,149,169},//╩
            {226,149,166},//╦
            {226,149,160},//╠
            {226,149,172},//╬
            {226,148,130},//│
            {226,148,128},//─
            {226,148,144},//┐
            {226,148,148},//└
            {226,148,152},//┘
            {226,148,140},//┌
            {226,148,164},//┤
            {226,148,180},//┴
            {226,148,172},//┬
            {226,148,156},//├
            {226,148,188} //┼
         }
        };//needs the extra brackets I guess
        std::array<cchar_t,38> symbols =
        {L'☺',L'☻',L'▲',L'▼',L'◄',L'►',L'■',L'♣',L'♠',L'♦',L'•',L'◘',L'○',L'☼',L'▬',L'≈',
         L'║',L'╗',L'╝',L'╚',L'╔',L'═',L'╣',L'╩',L'╦',L'╠',L'╬',
         L'│',L'─',L'┐',L'└',L'┘',L'┌',L'┤',L'┴',L'┬',L'├',L'┼'  };//corresponding symbols for the values codes

        newVector = inputVector;
        for(short i=0;i<newVector.size()-2;i++)
        {
            tempArray[0]=newVector.at(i);
            tempArray[1]=newVector.at(i+1);
            tempArray[2]=newVector.at(i+2);

            for(short j=0;j<values.size();j++)
            {
                if(tempArray==values[j])
                {
                    newVector.erase(newVector.begin()+i,newVector.begin()+i+3);//deleting the 3 elements
                    newVector.insert(newVector.begin()+i, symbols[j] );
                }//if identical
            }//for j
        }//for i
        return newVector;
    }//replaceCharCodes

}//namespace Game
