
#include <sys/types.h>
#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

int main()
{
  int i;
  char command[16];
  initscr(); /* Start curses mode */
  for (i = 0; i < 1000; i+=5)
  {
    // Keep track of entered character
    move(0, 0);
    printw("Previous command: ");

    // Keep track of entered character
    move(1, 0);
    printw("Enter a command: ");

    // Print the current number and the number + 5
    move(4, 8);
    printw("%d %d", i,i+5);

    // Print onto screen
    refresh();

    // Get user input
    move(1, 17);
    getnstr(command, 16);
    refresh();

    // Erase user's input for cleanliness and replace previous input for tracking
    move(1, 0);
    clrtoeol();
    printw("Enter a command: ");
    move(0, 0);
    clrtoeol();
    printw("Previous command: %s", command);
    refresh();
  }
  getch();  /* Wait for user input */
  endwin(); /* End curses mode                */

  return 0;
}
