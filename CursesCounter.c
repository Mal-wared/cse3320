
#include <sys/types.h>
#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

void run_program(char *system_call)
{
  endwin();
  system(system_call);
  initscr();
  refresh();
}

int main()
{
  /* Start curses mode */
  initscr();
  char command[32] = "D";
  int file_place = 3;
  while (strcmp(command, "Q"))
  {
    // Get user input
    

    switch (command[0])
    {
    case 'D':
    {
      file_place = 3;
      clear();

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
      DIR *curr_directory;
      struct dirent *directory_entry;

      int count = 0;

      move(file_place, 0);
      curr_directory = opendir(".");
      count = 1;
  
      while ((directory_entry = readdir(curr_directory)) && count % 6 != 0) 
      {
        if (((directory_entry->d_type) & DT_REG) && count % 6 != 1)
        {
          printw("%d. %s", count++, directory_entry->d_name);
          file_place++;
          move(file_place, 15);
        }
        else if (((directory_entry->d_type) & DT_REG))
        {
          printw("Files:");
          move(file_place, 15);
          printw("%d. %s", count++, directory_entry->d_name);
          file_place++;
          move(file_place, 15);
        }

        if (count % 6 == 0)
        {
          move(file_place, 10);
          printw("Press NF for next files | Press PF for previous files");
          file_place++;
          
          file_place++;
          move(file_place, 0);
          count = 0;
        }
      }
      rewinddir(curr_directory);

      // Get directories
      count = 1;

      while ((directory_entry = readdir(curr_directory)) && count % 6 != 0)
      {
        if (((directory_entry->d_type) & DT_DIR) && count % 6 != 1)
        {
          printw("%d. %s", count++, directory_entry->d_name);
          file_place++;
          move(file_place, 15);
        }
        else if ((directory_entry->d_type) & DT_DIR)
        {
          printw("Directories: ");
          move(file_place, 15);
          printw("%d. %s", count++, directory_entry->d_name);
          file_place++;
          move(file_place, 15);
          count++;
        }

        if (count % 6 == 0)
        {
          move(file_place, 10);
          printw("Press NF for next directories | Press PF for previous directories");
          file_place++;
          move(file_place, 0);
          count = 0;
        }
      }

      closedir(curr_directory);

      // Display available operations
      file_place++;
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
      file_place++;
      break;
    }
    case 'E':
    {
      char *space_pos = strchr(command, ' ');

      if (space_pos != NULL)
      {
        space_pos[strcspn(space_pos, "\n")];
      }
      else
      {
        break;
      }

      char system_command[32];
      snprintf(system_command, sizeof(system_command), "nano %s", space_pos + 1);
      run_program(system_command);
      
      break;
    }
    case 'R':
    {
      char *space_pos = strchr(command, ' ');

      if (space_pos != NULL)
      {
        space_pos[strcspn(space_pos, "\n")];
      }
      else
      {
        break;
      }

      char system_command[32];
      snprintf(system_command, sizeof(system_command), "%s", space_pos + 1);
      run_program(system_command);
      
      break;
    }
    case 'S':
    {
      break;
    }
    }
    move(file_place, 15);
    getnstr(command, 32);

    refresh();
  }
  endwin(); /* End curses mode */

  return 0;
}
