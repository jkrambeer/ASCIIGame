/***************************************************
 *
 * Joseph Krambeer
 * This is for making the map class which holds
 * map info that the game uses to display the terrain.
 *
 ***************************************************/
#pragma once

struct EventPair
{
   std::vector<Game::TileEvent> tileEvents;
   std::vector<Game::MoveEvent> moveEvents;
};

namespace Game
{
    std::vector<EventPair>   getMapEvents(const char* searchDirectory);
    std::vector<std::string> getFileNamesInDirectory(const char* directory);
    std::vector<GameMap>     getGameMaps(const char* searchDirectory);

    class GameArea
    {
        private:
            MapIndex  mapIndex;
            GameMap   gameMap;
            EventPair events;

        public:
            GameArea();
            GameArea(MapIndex index, GameMap newMap, EventPair newEvents);

            MapIndex  getMapIndex();
            GameMap   getGameMap();
            EventPair getEvents();
            void      setMapChars(std::vector<cchar_t> newChars);
    };

    GameArea::GameArea()
    {
        //default constructor
    }

    GameArea::GameArea(MapIndex index, GameMap newMap, EventPair newEvents)
    {
        this->mapIndex = index;
        this->gameMap  = newMap;
        this->events   = newEvents;
    }//GameArea constructor

    MapIndex GameArea::getMapIndex()
    {
        return this->mapIndex;
    }//getMapIndex

    GameMap GameArea::getGameMap()
    {
        return this->gameMap;
    }//getGameMap

    EventPair GameArea::getEvents()
    {
        return this->events;
    }//getEvents

    void GameArea::setMapChars(std::vector<cchar_t> newChars)
    {
        gameMap.setMapChars(newChars);
    }//setMapsChars


/*-----OTHER FUNCTIONS-----*/

    std::vector<GameArea> createAllGameAreas()
    {
        std::vector<GameArea>  gameAreas;
        std::vector<EventPair> events;
        std::vector<GameMap>   maps;

        events = getMapEvents("MapEvents\\");
        maps   = getGameMaps("Maps\\");

        if(events.size()!=maps.size() )
        {
            printf("Events size is %d and maps size is %d. These two values need to be equal, please fix this.",events.size(),maps.size() );
            system("pause");
            abort();
        }//if not equal in size

        for(int i=0;i<maps.size();i++)
        {
            GameArea newArea(i,maps.at(i),events.at(i) );
            gameAreas.push_back(newArea);
        }
        return gameAreas;
    }

    std::vector<EventPair> getMapEvents(const char* searchDirectory)
    {
        std::vector<EventPair> eventPairs;
        std::vector<TileEvent> tileEvents;
        std::vector<MoveEvent> moveEvents;
        std::vector<std::string>    fileNames;

        FILE*             file;
        EventPair         eventPair;
        cchar_t           character;

        int               y,x,newY,newX,damage,charNum,mapIndex;
        const char*       messageString;
        char              fileNameBuf[200];
        char              eventMessage[100];
        char              eventType[10];

        fileNames = getFileNamesInDirectory(searchDirectory);
        for(std::string fileName : fileNames)
        {
            tileEvents.clear();
            moveEvents.clear();

            sprintf(fileNameBuf,"%s%s",searchDirectory,fileName.c_str() );//concatenating the search directory and the current file name
            printf("%s\n",fileNameBuf);
            file = fopen(fileNameBuf,"r");//opening the file with the c string path

            if(!file){puts("Failed to read file"); continue;}//if file doesn't open it moves to next fileName iteration
            else     {puts("Read file\n");}

            for(Byte x=0;x<3;x++){fgetwc(file);}//getting rid of utf-8 encoding chars

            do //while eventType!="End"
            {
                fscanf(file,"%s",eventType);//reading tile:,move:,end
                if(strcmp("Tile:",eventType)==0 )
                {
                    if( fscanf(file,"%d %d %s %d %d %d %d",&y,&x,eventMessage,&newY,&newX,&damage,&charNum)==7 )
                    {
                        character = charNum;
                        TileEvent eventTile(y,x,std::string(eventMessage),newY,newX,damage,character);
                        tileEvents.push_back(eventTile);
                    }//tileEvent successful read
                }//tileEvent event
                else if(strcmp("Move:",eventType)==0 )
                {
                    if ( fscanf(file,"%d %d %s %d %d %d",&y,&x,eventMessage,&newY,&newX,&mapIndex)==6 )
                    {
                        MoveEvent eventMove(y,x,std::string(eventMessage),newY,newX,mapIndex);
                        moveEvents.push_back(eventMove);
                    }//moveEvent successful read
                }//moveEvent event
            } while(strcmp("End",eventType)!=0);//do while (this will infinite loop if file not formatted correctly <.< )

            eventPair.tileEvents = tileEvents; //making the tileEvents of the new eventPair
            eventPair.moveEvents = moveEvents; //making the moveEvents of the new eventPair
            eventPairs.push_back(eventPair);   //adding the eventPair to the vector
        }//for strings

        return eventPairs;
    }//initializeMapEvents

    std::vector<GameMap> getGameMaps(const char* searchDirectory)
    {
        std::vector<GameMap>      readGameMaps;
        std::vector<std::string>  fileNames;
        char                      fileNameBuf[200];//the file path shouldnt be over 100 chars long
        FILE                     *file;

        fileNames = getFileNamesInDirectory(searchDirectory);//getting the string vector

        for(std::string fileName : fileNames)
        {
            sprintf(fileNameBuf,"%s%s",searchDirectory,fileName.c_str() );//adding on the directory name to the filename to make the path

            printf("\n%s\n",fileNameBuf);
            file = fopen(fileNameBuf,"r");//opening the file with the c string path

            if(!file){ puts("Failed to read file"); continue;/*Add something to exit program here*/ }
            else
            {
                GameMap gameMap(file);//making the GameMap object with the opened file

                puts("Read file\n");
                readGameMaps.push_back(gameMap);
            }
        }//for fileNames

        return readGameMaps;
    }//initializeGameMapVector

    std::vector<std::string> getFileNamesInDirectory(const char* directory)
    {
        std::vector<std::string>  files; //Will be added to List
        char                      searchPath[200];
        WIN32_FIND_DATA           findData;
        HANDLE                    handle;

        sprintf(searchPath, "%s*.*", directory);//the handle needs to be a c string
        handle = FindFirstFile(searchPath, &findData);

        if(handle!= INVALID_HANDLE_VALUE)
        {
            do
            {
                // read all files in current folder
                if(! (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
                {
                    files.push_back(findData.cFileName);
                }
                //if you need other things like folders add an else here
            }
            while(FindNextFile(handle, &findData));
            FindClose(handle);
        }//if valid handle
        std::sort(files.begin(), files.end());

        return files;
    }//getFileNamesInDirectory

}//namespace Game
