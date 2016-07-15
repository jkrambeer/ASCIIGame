/***************************************************
 *
 * Joseph Krambeer
 * This is a header for declaring functions that do
 * not involve any objects defined in the Game namespace
 *
 ***************************************************/
#pragma once

WINDOW*             getDialogWindow();
void                setMapNumber(MapIndex number);
short               YXCordToVectorPosition(short y,short x);
void                updateMapCharacters(std::vector<cchar_t> newChars);
Mode                getGameMode();
MapIndex            getMapNumber();
void                printMap();
void                setGameMode(Mode newMode);
void                applyInput(int input);
void                updateDisplayMode(Mode currentMode);
void                switchDisplays();
void                createDisplay();
void                createMenu();
void                setPlayerYX(short y,short x);
void                applyAction(int input);
void                cordinateDisplayTerrain(short yCord,short xCord);
void                moveDisplay(int input);
void                addColoredTiles(short y,short x,cchar_t tile);
void                cutDownTrees(short y,short x);
void                setWindow(int width, int height);
void                clearMessages();
std::array<short,2> vectorPositionToYXCord(short vecPosition);
std::array<bool,8>  findInAdjacentChars(short y,short x,cchar_t character);
void                applyMovingEvents();
