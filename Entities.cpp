/******************
 *
 * Joseph Krambeer
 * A text based game
 *
 ******************/

/*
 * Since this game is pretty much dead, I've decided to leave
 * the current format until
 *
 */

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500 //used so windows can correctly resize the console

/*
 *   -----Variable Types Definitions-------
 */
typedef unsigned char Byte; //equivalent of 1 byte
typedef char          MapIndex;
enum Mode : Byte {display=0, playerMenu=1, inventoryMenu=2, mapMenu=3};

/*
 *   -----Header Files-------
 */
#include <panel.h>  //includes curse.h and stdio.h
#include <windows.h>//to resize the console
#include <exception>
#include <vector>
#include <array>
#include <string>
#include <algorithm>

#include "Functions.h"
#include "CursesRedefines.h"
#include "Entity.h"
#include "Player.h"
#include "GameMap.h"
#include "MapEvent.h"
#include "GameArea.h"

/*
 *   -----Namespace stuff-------
 */
using std::string;
using std::wstring;
using std::vector;
using std::array;
using std::exception;
using Game::GameMap;
using Game::TileEvent;
using Game::MoveEvent;
using Game::GameArea;
using Game::Player;

/*
 *   --------State-----------
 */
Mode             gameMode; //tells game what to display on the screen
WINDOW          *displayWindow,*playerInfoWindow,*dialogWindow;//for the display
PANEL           *displayPanel ,*playerInfoPanel ,*dialogPanel; //for the display
WINDOW          *menuBarWindow,*playerMenuWindow,*inventoryMenuWindow,*mapMenuWindow;//for menu
PANEL           *menuBarPanel ,*playerMenuPanel ,*inventoryMenuPanel ,*mapMenuPanel; //for menu
vector<GameArea> gameAreas; //vector containing all maps and their events/attributes
MapIndex         mapNumber;//used to note the index of gameAreas that is current being used

/*
 *   --------Main-----------
 */
int main()
{
    FILE*  mapFile;
    bool   inputCheck;
    int    input;

    setWindow(80,27);//width,height which matched to COLS,LINES

    //system("title ayylmao");
    gameAreas = Game::createAllGameAreas();//this initializes all the gameMaps in the gameAreas vector
    setMapNumber(0);

    printf("\nHeight : %d, Width: %d\n",gameAreas.at(mapNumber).getGameMap().getHeight(), gameAreas.at(mapNumber).getGameMap().getWidth() );
    system("pause");

/* ---Curses Mode--- */
    initscr();
    start_color();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    init_color(Blue, 0, 0, 1000);//redefining the intensity of the color blue
    init_pair(1, Brown, Black);//1st is text color, 2nd is background color
    init_pair(2, Green, Black);
    init_pair(3, Cyan , Black);
    init_pair(4, White, Blue );

    refresh();//need to refresh before making window apparently

    setPlayerYX(12,50);

    createDisplay();
    createMenu();
    printMap();
    setGameMode(display);
    updateDisplayMode( getGameMode() );

    cordinateDisplayTerrain( Player::getInstance().getYCord(),Player::getInstance().getXCord() );
    while(true)
    {
        Player::getInstance().getPlayerAction();
    }
    refresh();
    endwin();

}//main

/*
 *   --------Functions-----------
 */

 vector<GameArea> getGameAreas()
 {
     return gameAreas;
 }//getGameAreas

void applyInput(int input)
{
    Mode  currentMode;
    Mode  newMode;
    Byte  modeNumber;
    bool  directionalCheck;

    currentMode      = getGameMode();
    directionalCheck = (input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN);

    if(currentMode == display)
    {
        if     (directionalCheck)  { moveDisplay(input ); }
        else if(input == KEY_ESC)  { switchDisplays();    }
        else if(input == KEY_F(2) ){ clearMessages();     }
        else if(input == 'c')      { cutDownTrees( Player::getInstance().getYCord(),Player::getInstance().getXCord() ); }
    }
    else
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
    }//not display
}//applyInput

void printMap()
{
    const static int mapWidth  = 54;
    const static int mapHeight = 20;
    const static cchar_t period    = '.';

    for(int y=0;y<mapHeight;y++)
    {
        for(int x=0;x<mapWidth;x++)
        {
            mvwadd_wch(mapMenuWindow,y+4,x+6,&period);
        }//for x
    }//for y
}//printMap

void switchDisplays()
{
    Mode currentMode;

    currentMode = getGameMode();
    if( currentMode == display){ updateDisplayMode(playerMenu);}
    else                       { updateDisplayMode(display);   }
}//switchDisplays

Mode getGameMode()
{
    return gameMode;
}//getGameMode

void setGameMode(Mode newMode)
{
    gameMode = newMode;
}//setGameMode

void updateDisplayMode(Mode mode)
{
    //this hides panel depending on the current mode and highlights the mode text if the player is in the menu
    setGameMode(mode);
    if(mode == display )
    {
        show_panel(displayPanel);
        show_panel(playerInfoPanel);
        show_panel(dialogPanel);
        hide_panel(menuBarPanel);
        hide_panel(playerMenuPanel);
        hide_panel(inventoryMenuPanel);
        hide_panel(mapMenuPanel);
    }//display mode
    else if(mode == playerMenu )
    {
        show_panel(menuBarPanel);
        show_panel(playerMenuPanel);
        hide_panel(displayPanel);
        hide_panel(playerInfoPanel);
        hide_panel(dialogPanel);
        hide_panel(inventoryMenuPanel);
        hide_panel(mapMenuPanel);

        wattron(menuBarWindow,COLOR_PAIR(4) );
        mvwprintw(menuBarWindow,3 ,3," Player ");
        mvwprintw(menuBarWindow,4 ,3,"  Info  ");
        wattroff(menuBarWindow,COLOR_PAIR(4) );
        mvwprintw(menuBarWindow,11 ,2,"   View  ");
        mvwprintw(menuBarWindow,12,2,"Inventory");
        mvwprintw(menuBarWindow,19,3," World ");
        mvwprintw(menuBarWindow,20,3,"  Map  ");
    }//player menu mode
    else if(mode == inventoryMenu )
    {
        show_panel(menuBarPanel);
        show_panel(inventoryMenuPanel);
        hide_panel(displayPanel);
        hide_panel(playerInfoPanel);
        hide_panel(dialogPanel);
        hide_panel(playerMenuPanel);
        hide_panel(mapMenuPanel);

        mvwprintw(menuBarWindow,3 ,3," Player ");
        mvwprintw(menuBarWindow,4 ,3,"  Info  ");
        wattron(menuBarWindow,COLOR_PAIR(4) );
        mvwprintw(menuBarWindow,11 ,2,"   View  ");
        mvwprintw(menuBarWindow,12,2,"Inventory");
        wattroff(menuBarWindow,COLOR_PAIR(4) );
        mvwprintw(menuBarWindow,19,3," World ");
        mvwprintw(menuBarWindow,20,3,"  Map  ");
    }//inventory menu mode
    else if(mode == mapMenu )
    {
        show_panel(menuBarPanel);
        show_panel(mapMenuPanel);
        hide_panel(displayPanel);
        hide_panel(playerInfoPanel);
        hide_panel(dialogPanel);
        hide_panel(playerMenuPanel);
        hide_panel(inventoryMenuPanel);

        mvwprintw(menuBarWindow,3 ,3," Player ");
        mvwprintw(menuBarWindow,4 ,3,"  Info  ");
        mvwprintw(menuBarWindow,11 ,2,"   View  ");
        mvwprintw(menuBarWindow,12,2,"Inventory");
        wattron(menuBarWindow,COLOR_PAIR(4) );
        mvwprintw(menuBarWindow,19,3," World ");
        mvwprintw(menuBarWindow,20,3,"  Map  ");
        wattroff(menuBarWindow,COLOR_PAIR(4) );
    }//map menu mode
    refresh();
    update_panels();
    doupdate();
}//updateDisplay

void createMenu()
{
    //creating the left side window for the menu
    menuBarWindow = newwin(LINES,14,0,0);
    wborder(menuBarWindow , L'│',L'│',L'─',L'─',L'┌',L'┬',L'└',L'┴');

    //creating right side window for player info
    playerMenuWindow = newwin(LINES,COLS-14,0,14);
    wborder(playerMenuWindow , L'│',L'│',L'─',L'─',L'┬',L'┐',L'┴',L'┘');

    //creating right side window for inventory info
    inventoryMenuWindow = newwin(LINES,COLS-14,0,14);
    wborder(inventoryMenuWindow , L'│',L'│',L'─',L'─',L'┬',L'┐',L'┴',L'┘');

    //creating right side window for displaying the map
    mapMenuWindow = newwin(LINES,COLS-14,0,14);
    wborder(mapMenuWindow , L'│',L'│',L'─',L'─',L'┬',L'┐',L'┴',L'┘');

    //creating labels for menuBar
    mvwprintw(menuBarWindow,3 ,3," Player ");
    mvwprintw(menuBarWindow,4 ,3,"  Info  ");
    mvwprintw(menuBarWindow,11 ,2,"   View  ");
    mvwprintw(menuBarWindow,12,2,"Inventory");
    mvwprintw(menuBarWindow,19,3," World ");
    mvwprintw(menuBarWindow,20,3,"  Map  ");

    //crating labels inside the other windows
    mvwprintw(playerMenuWindow,2,2,"Player Menu");      //change
    mvwprintw(inventoryMenuWindow,2,2,"Inventory Menu");//change
    mvwprintw(mapMenuWindow,2,2,"Map Menu");            //change

    //creating panels
    menuBarPanel       = new_panel(menuBarWindow);
    playerMenuPanel    = new_panel(playerMenuWindow);
    inventoryMenuPanel = new_panel(inventoryMenuWindow);
    mapMenuPanel       = new_panel(mapMenuWindow);

    //hiding the panels
    hide_panel(menuBarPanel);
    hide_panel(playerMenuPanel);
    hide_panel(inventoryMenuPanel);
    hide_panel(mapMenuPanel);

    refresh();
    update_panels();
    doupdate();
}//createMenu

WINDOW* getPlayerInfoWindow()
{
    return playerInfoWindow;
}//getPlayerInfoWindow

WINDOW* getDisplayWindow()
{
    return displayWindow;
}//getDisplayWindow

WINDOW* getDialogWindow()
{
     return dialogWindow;
}//getDialogWindow

void createDisplay()
{
    //creating the display window
	displayWindow = newwin(19, COLS, 0, 0);
    wborder(displayWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');

    //creating the playerInfo window
    playerInfoWindow = newwin(8, 24, 19, 0);
    wborder(playerInfoWindow, L'│',L'│',L'─',L'─',L'┌',L'┬',L'└',L'┴');

    //creating the dialog window
    dialogWindow = newwin(8, COLS-23, 19, 23);
    wborder(dialogWindow, L'│',L'│',L'─',L'─',L'┬',L'┐',L'┴',L'┘');

    //creating labels in playerInfo
    wattron(playerInfoWindow,A_BOLD);
    mvwprintw(playerInfoWindow ,1,1,"Player Name");
    wattroff(playerInfoWindow,A_BOLD);

    mvwprintw(playerInfoWindow ,3,1,"Health:  64/100");//remove these extra placeholder values after you have that system in place
    mvwprintw(playerInfoWindow ,4,1,"Magic :  12/50");
    mvwprintw(playerInfoWindow ,5,1,"Weapon:  Iron Sword");
    mvwprintw(playerInfoWindow ,6,1,"Armor :  Leather");
    wrefresh(playerInfoWindow );

    //Creating labels in dialog window
    wattron(dialogWindow,A_BOLD);
    mvwprintw(dialogWindow,1,2,"Map 0");
    wattroff(dialogWindow,A_BOLD);

    mvwprintw(dialogWindow,3,2,"Events:");
    wrefresh(dialogWindow);

    //creating panels of the created windows
    dialogPanel     = new_panel(dialogWindow);
    playerInfoPanel = new_panel(playerInfoWindow);
    displayPanel    = new_panel(displayWindow);

}//createDisplay


void cordinateDisplayTerrain(short yCord,short xCord)//fix spelling
{
    vector<cchar_t> mapVector;
    cchar_t         tile;
    cchar_t         playerIcon;
    short           offset;
    short           mapWidth;
    Byte            position;//0 for left, 1 for center, 2 for right

    mapVector = gameAreas.at(mapNumber).getGameMap().getMapChars(); //good
    mapWidth  = gameAreas.at(mapNumber).getGameMap().getWidth();

    offset    = 0;
    position  = 1;
    playerIcon = Player::getInstance().getIcon();

    //explain all these magic numbers
    if(xCord<39)
    {
        offset = 38-xCord;
        position = 0;
    }
    else if(xCord>mapWidth-39)
    {
        offset = xCord-(mapWidth-39);
        position = 2;
    }
    for(int i=0;i<17;i++)
    {
        for(int j=0;j<76;j++)
        {
            try
            {
                if     (position==1){tile = mapVector.at( YXCordToVectorPosition(yCord+i-8,xCord+j-38) );}//! explain why 8 and 32 are used
                else if(position==0)
                {
                    if(j<offset){tile = ' ';}
                    else        {tile = mapVector.at( YXCordToVectorPosition(yCord+i-8,xCord+j-38) );}
                }//if position 0
                else if(position==2)
                {
                    if(76-j<offset){tile = ' ';}
                    else           {tile = mapVector.at( YXCordToVectorPosition(yCord+i-8,xCord+j-38) );}
                }//if position 2
                addColoredTiles(i+1,j+2,tile);
            }//try
            catch (exception &e)
            {
                tile = ' ';
                addColoredTiles(i+1,j+2,tile);
            }//catch
        }//for j
    }//for i
    mvwadd_wch(displayWindow, 9 ,COLS/2, &playerIcon );
    wrefresh(displayWindow);
}//coordinateDisplayTerrain2

void moveDisplay(int input)
{
    wstring         illegalMoveChars  = L"☺☻♣♠≈║╗╝╚╔═╣╩╦╠╬│─┐└┘┌┤┴┬├┼";//update with the other connectors
    vector<cchar_t> mapChars;
    short           mapWidth;
    short           mapHeight;
    short           yCord;
    short           xCord;

    mapChars  = gameAreas.at(mapNumber).getGameMap().getMapChars();
    mapWidth  = gameAreas.at(mapNumber).getGameMap().getWidth();
    mapHeight = gameAreas.at(mapNumber).getGameMap().getHeight();
    yCord = Player::getInstance().getYCord();//need to be gotten after applying moving events as they
    xCord = Player::getInstance().getXCord();//can update the players coordinated

    try
    {
        if(input == KEY_LEFT)
        {
            if( xCord%mapWidth==0 ){return;}
            else
            {
                if( illegalMoveChars.find(mapChars.at( YXCordToVectorPosition(yCord,xCord-1)) )!= string::npos ){return;}
                xCord--;
                Player::getInstance().setCords( yCord, xCord );
            }
        }//key left
        else if(input == KEY_RIGHT)
        {
            if( xCord%mapWidth==mapWidth-1 ){return;}
            else
            {
                if( illegalMoveChars.find(mapChars.at( YXCordToVectorPosition(yCord,xCord+1)) )!= string::npos ){return;}
                xCord++;
                Player::getInstance().setCords( yCord, xCord );
            }
        }//key right
        else if(input == KEY_UP)
        {
            if(yCord<=0){return;}
            else
            {
                if( illegalMoveChars.find(mapChars.at( YXCordToVectorPosition(yCord-1,xCord)) )!= string::npos ){return;}
                yCord--;
                Player::getInstance().setCords( yCord, xCord );;
            }
        }//key up
        else if(input == KEY_DOWN)
        {
            if(yCord>=mapHeight-2){return;}
            else
            {
                if( illegalMoveChars.find(mapChars.at( YXCordToVectorPosition(yCord+1,xCord)) )!= string::npos ){return;}
                yCord++;
                Player::getInstance().setCords( yCord, xCord );
            }
        }//key down
    }//try
    catch(exception &e){/* This would result from a vector out of bounds, error checking should be added later */}
    clearMessages();
    applyMovingEvents();

    mvwprintw(dialogWindow,3,10,"%d %d",yCord,xCord);
    wrefresh(dialogWindow);

    cordinateDisplayTerrain( Player::getInstance().getYCord(),Player::getInstance().getXCord() );//mapVector
}

void addColoredTiles(short y,short x,cchar_t tile) //!****CHANGE TO READ THE COLORS OFF A FILE****
{
    if     (tile==L'≈')              {wattron(displayWindow,COLOR_PAIR(4)  );  }
    else if(tile==L'♣' || tile==L'♠'){wattron(displayWindow,COLOR_PAIR(2)  );  }
    else if(tile=='.')               {wattron(displayWindow,COLOR_PAIR(1)  );  }

    mvwadd_wch(displayWindow,y,x,&tile);

    if     (tile==L'≈')              {wattroff(displayWindow,COLOR_PAIR(4)); }
    else if(tile==L'♣' || tile==L'♠'){wattroff(displayWindow,COLOR_PAIR(2)); }
    else if(tile=='.')               {wattroff(displayWindow,COLOR_PAIR(1)); }
}//addColoredTiles

void setMapNumber(MapIndex number)
{
    mapNumber = number;
}//setMapNumber

void updateMapCharacters( vector<cchar_t> newChars)
{
    short yCord;
    short xCord;

    yCord = Player::getInstance().getYCord();
    xCord = Player::getInstance().getXCord();

    gameAreas.at(mapNumber).setMapChars(newChars);
    cordinateDisplayTerrain(yCord,xCord);
}//updateMapCharacters

MapIndex getMapNumber()
{
    return mapNumber;
}//getMapNumber

void setPlayerYX(short y,short x)//change to switch state on player object
{
    Player::getInstance().setCords(y,x);
}//setPlayerYX

void applyMovingEvents()
{
    EventPair events;
    short     yCord;
    short     xCord;

    yCord  = Player::getInstance().getYCord();
    xCord  = Player::getInstance().getXCord();
    events = gameAreas.at(mapNumber).getEvents();
    for(TileEvent currentEvent : events.tileEvents)
    {
        if(currentEvent.getYCord() == yCord && currentEvent.getXCord() == xCord  ){currentEvent.applyEvent();}
    }
    for(MoveEvent currentEvent : events.moveEvents)
    {
        if(currentEvent.getYCord() == yCord && currentEvent.getXCord() == xCord  ){currentEvent.applyEvent();}
    }
}//applyMovingEvents

GameMap getCurrentGameMap()
{
    return gameAreas.at(mapNumber).getGameMap();
}//getCurrentMap

void clearMessages()
{
    const char* blank = "                                          ";
    mvwprintw(dialogWindow,3,10,blank);
    wrefresh(dialogWindow);
}//clearMessages

void cutDownTrees(short y,short x)
{
    vector<cchar_t> mapChars;
    array<bool,8>   indexClub;
    array<bool,8>   indexSpade;
    short           yCord;
    short           xCord;

    yCord      = Player::getInstance().getYCord();
    xCord      = Player::getInstance().getXCord();
    mapChars   = gameAreas.at(mapNumber).getGameMap().getMapChars();
    indexClub  = findInAdjacentChars(y,x,L'♣');
    indexSpade = findInAdjacentChars(y,x,L'♠');

    if(indexClub[0] || indexSpade[0]){mapChars.at(YXCordToVectorPosition(y-1,x-1))='x';}
    if(indexClub[1] || indexSpade[1]){mapChars.at(YXCordToVectorPosition(y-1,x  ))='x';}
    if(indexClub[2] || indexSpade[2]){mapChars.at(YXCordToVectorPosition(y-1,x+1))='x';}
    if(indexClub[3] || indexSpade[3]){mapChars.at(YXCordToVectorPosition(y,  x-1))='x';}
    if(indexClub[4] || indexSpade[4]){mapChars.at(YXCordToVectorPosition(y,  x+1))='x';}
    if(indexClub[5] || indexSpade[5]){mapChars.at(YXCordToVectorPosition(y+1,x-1))='x';}
    if(indexClub[6] || indexSpade[6]){mapChars.at(YXCordToVectorPosition(y+1,x  ))='x';}
    if(indexClub[7] || indexSpade[7]){mapChars.at(YXCordToVectorPosition(y+1,x+1))='x';}

    gameAreas.at(mapNumber).setMapChars(mapChars);
    cordinateDisplayTerrain(yCord,xCord);
}//cutDownTrees

array<bool,8> findInAdjacentChars(short y,short x,cchar_t character)
{
    array<bool,8>   index;
    vector<cchar_t> mapChars;
    short           mapWidth;
    short           mapHeight;

    mapChars  = gameAreas.at(mapNumber).getGameMap().getMapChars();
    mapWidth  = gameAreas.at(mapNumber).getGameMap().getWidth();
    mapHeight = gameAreas.at(mapNumber).getGameMap().getHeight();

    index.fill(false);//setting all values to false
    /* --Map Check Positions--
               012
               3☺4
               567             */

    if(y>0 && x>0)                     { if(mapChars.at(YXCordToVectorPosition(y-1,x-1))==character){index[0]=true;} }
    if(y>0)                            { if(mapChars.at(YXCordToVectorPosition(y-1,x  ))==character){index[1]=true;} }
    if(y>0 && x!=mapWidth-1)           { if(mapChars.at(YXCordToVectorPosition(y-1,x+1))==character){index[2]=true;} }
    if(x>0)                            { if(mapChars.at(YXCordToVectorPosition(y  ,x-1))==character){index[3]=true;} }
    if(x!=mapWidth-1)                  { if(mapChars.at(YXCordToVectorPosition(y  ,x+1))==character){index[4]=true;} }
    if(y!=mapHeight-2 && x>0)          { if(mapChars.at(YXCordToVectorPosition(y+1,x-1))==character){index[5]=true;} }
    if(y!=mapHeight-2)                 { if(mapChars.at(YXCordToVectorPosition(y+1,x  ))==character){index[6]=true;} }
    if(y!=mapHeight-2 && x!=mapWidth-1){ if(mapChars.at(YXCordToVectorPosition(y+1,x+1))==character){index[7]=true;} }

    return index;
}//findInAdjacentChars

array<short,2> vectorPositionToYXCord(short vecPosition)
{
    array<short,2> yx;
    short          mapWidth;

    mapWidth = gameAreas.at(mapNumber).getGameMap().getWidth();

    yx[0]= vecPosition/mapWidth;//y
    yx[1]= vecPosition%mapWidth;//x

    return yx;
}//vectorPositionToYXCord

short YXCordToVectorPosition(short y,short x)
{
    short mapWidth;

    mapWidth = gameAreas.at(mapNumber).getGameMap().getWidth();

    return (y*mapWidth+x);
}//YXCordToVectorPosition

void setWindow(int width, int height)
{
    HANDLE      handle;
    _SMALL_RECT rect;
    _COORD      coord;

    coord.X = width;
    coord.Y = height;

    rect.Top    = 0;
    rect.Left   = 0;
    rect.Bottom = height - 1;
    rect.Right  = width - 1;

    handle = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleScreenBufferSize(handle, coord);//Setting the window buf
    SetConsoleWindowInfo(handle, TRUE, &rect);//Setting the window size
}//setWindow
