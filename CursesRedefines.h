/***************************************************
 *
 * Joseph Krambeer
 * This is for redefining the curses.h color values
 *
 ***************************************************/
#pragma once

#ifdef COLOR_BLACK
 #undef COLOR_BLACK
#endif

#ifdef COLOR_BLUE
 #undef COLOR_BLUE
#endif

#ifdef COLOR_GREEN
 #undef COLOR_GREEN
#endif

#ifdef COLOR_RED
 #undef COLOR_RED
#endif

#ifdef COLOR_CYAN
 #undef COLOR_CYAN
#endif

#ifdef COLOR_MAGENTA
 #undef COLOR_MAGENTA
#endif

#ifdef COLOR_YELLOW
 #undef COLOR_YELLOW
#endif

#ifdef COLOR_WHITE
 #undef COLOR_WHITE
#endif

#define Black       0 //console is black background to begin
#define DarkBlue    1
#define Green       2
#define Cyan        3
#define DarkRed     4
#define Purple      5
#define Brown       6
#define LightGray   7 //console is light gray text to begin
#define Gray        8
#define Blue        9
#define LightGreen 10
#define LightBlue  11
#define Red        12
#define Magenta    13
#define Yellow     14
#define White      15

#define KEY_ESC 27 //int representation of esc key
#ifdef  KEY_ENTER //redefining curse's enter key definition
    #undef KEY_ENTER
    #define KEY_ENTER '\n' //I hope and pray that keyboards on windows input newlines first
#endif
