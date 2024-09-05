
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
  /* Start curses mode */
  initscr();

  // Get current directory
  move(0, 0);
  char current_directory[256];
  getcwd(current_directory, 200);
  printw("Current Working Dir: %s", current_directory);

  // Get current time
  move(1, 0);
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  printw("It is now: %s", asctime(timeinfo));

  // Get files
  DIR * curr_directory;
  struct dirent* directory_entry;
  char command[16];
  int count = 0;
  int file_place = 3;
  move(file_place, 0);
  curr_directory = opendir(".");
  count = 0;

  while ((directory_entry = readdir(curr_directory)) && count != 5)
  {
    if (((directory_entry->d_type) & DT_REG) && count != 0) {
      printw("%d. %s", count++, directory_entry->d_name);
      file_place++;
      move(file_place, 15);
    } else if (((directory_entry->d_type) & DT_REG)) {
      printw("Files:");
      move(file_place, 15);
      printw("%d. %s", count++, directory_entry->d_name);
      file_place++; 
      move(file_place, 15);
    }
      
    if (count == 5)
    {
      file_place++;
      move(file_place, 0);
    }
  }
  rewinddir(curr_directory);

  // Get directories
  count = 0;
  
  while ((directory_entry = readdir(curr_directory)))
  {
    if (((directory_entry->d_type) & DT_DIR) && count != 0) {
      printw("%d. %s", count++, directory_entry->d_name);
      file_place++;
      move(file_place, 15);
    } else if ((directory_entry->d_type) & DT_DIR) {
      printw("Directories: ");
      move(file_place, 15);
      printw("%d. %s", count++, directory_entry->d_name);
      file_place++;
      move(file_place, 15);
      count++;
    }
  }

  closedir(curr_directory);

  // Display available operations
  file_place ++;
  move(file_place, 0);  
  printw("Operations: ");
  move(file_place++, 15);
  printw("D Display");
  move(file_place++, 15);
  printw("E Edit");
  move(file_place++, 15);
  printw("R Run");
  move(file_place++, 15);
  printw("S Sort Directory listing");
  
  
  while(strcmp(command, "Q"))
  {
    // Get user input
    move(++file_place, 15);
    getnstr(command, 16);
    switch(command[0]){
      case 'D': {
        clear();

        // Print working directory
        move(0, 0);
        getcwd(current_directory, 200);
        printw("Current Working Dir: %s", current_directory);

        // Get current time
        move(1, 0);
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        printw("It is now: %s", asctime(timeinfo));

        // Get Files
        count = 0;
        file_place = 3;
        move(file_place, 0);
        curr_directory = opendir(".");
        count = 0;

        while ((directory_entry = readdir(curr_directory)) && count != 5)
        {
          if (((directory_entry->d_type) & DT_REG) && count != 0) {
            printw("%d. %s", count++, directory_entry->d_name);
            file_place++;
            move(file_place, 15);
          } else if (((directory_entry->d_type) & DT_REG)) {
            printw("Files:");
            move(file_place, 15);
            printw("%d. %s", count++, directory_entry->d_name);
            file_place++; 
            move(file_place, 15);
          }
            
          if (count == 5)
          {
            file_place++;
            move(file_place, 0);
          }
        }
        rewinddir(curr_directory);
        
        // Get directories
        count = 0;
        
        while ((directory_entry = readdir(curr_directory)))
        {
          if (((directory_entry->d_type) & DT_DIR) && count != 0) {
            printw("%d. %s", count++, directory_entry->d_name);
            file_place++;
            move(file_place, 15);
          } else if ((directory_entry->d_type) & DT_DIR) {
            printw("Directories: ");
            move(file_place, 15);
            printw("%d. %s", count++, directory_entry->d_name);
            file_place++;
            move(file_place, 15);
            count++;
          }
        }

        closedir(curr_directory);

        // Display available operations
        file_place ++;
        move(file_place, 0);  
        printw("Operations: ");
        move(file_place++, 15);
        printw("D Display");
        move(file_place++, 15);
        printw("E Edit");
        move(file_place++, 15);
        printw("R Run");
        move(file_place++, 15);
        printw("S Sort Directory listing");
        break;
      }
      case 'E': {
        break;
      }
      case 'R': {
        break;
      }
      case 'S': {
        break;
      }
    }

    refresh();
  }
  endwin(); /* End curses mode */

  return 0;
}
