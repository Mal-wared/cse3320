#include <sys/types.h>
#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

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
  int current_directory_count, current_de_count = 0;
  bool need_display_reset = false;
  char sort_mode[32];

  DIR *curr_directory;
  curr_directory = opendir(".");
  struct dirent *directory_entry;
  char directory_entry_names[100][64];
  long int directory_entry_sizes[100];
  char directory_entry_first_five[5][64];
  struct stat file_info;
  int den_count = 0;


  while ((directory_entry = readdir(curr_directory)))
  {
    if (((directory_entry->d_type) & DT_REG))
    {
      // Grabs directory entry names and puts into list
      strcpy(directory_entry_names[den_count], directory_entry->d_name);

      // Grabs directory entry sizes and puts into list
      char file_path[500];
      snprintf(file_path, sizeof(file_path), "./%s", directory_entry->d_name);
      if (stat(file_path, &file_info) == 0)
      {
        directory_entry_sizes[den_count] = file_info.st_size;
      }
      den_count++;
    }
  }
  
  rewinddir(curr_directory);

  int directory_count = 0;
  char directory_names[100][64];

  while ((directory_entry = readdir(curr_directory)))
  {
    if (((directory_entry->d_type) & DT_DIR))
    {
      strcpy(directory_names[directory_count++], directory_entry->d_name);
    }
  }
  closedir(curr_directory);

  while (strcmp(command, "Q") != 0)
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

      move(file_place, 0);

      int de_max = 5;
      if ((current_de_count + 5) > den_count)
      {
        de_max = den_count - current_de_count;
      }

      if (strcmp(sort_mode, "SL"))
      {
        
      }
      for (int i = current_de_count; i < current_de_count + de_max; i++) 
      {
        char directory_entry_size_as_string[64];
        sprintf(directory_entry_size_as_string, "%ld", directory_entry_sizes[i]);
        strcpy(directory_entry_first_five[i%5], directory_entry_size_as_string);
      }

      for (int i = current_de_count; i < current_de_count + de_max; i++) 
      {
        if (i % 5 != 0)
        {
          printw("%d. %ld bytes\t %s", i, directory_entry_sizes[i], directory_entry_names[i]);
          file_place++;
          move(file_place, 15);
        }
        else
        {
          printw("Files:");
          move(file_place, 15);
          printw("%d. %ld bytes\t %s", i, directory_entry_sizes[i], directory_entry_names[i]);
          file_place++;
          move(file_place, 15);
        }

        if (i == (current_de_count + de_max - 1))
        {
          move(file_place, 10);
          printw("Press NF for next files | Press PF for previous files");
          file_place++;
          move(file_place, 10);
          printw("Press SL to sort large to small | Press SS to sort small to large");
          file_place++;
          move(file_place, 10);
          printw("Press SN to sort newest to oldest | Press SO to sort oldest to newest");
          file_place+=2;
          move(file_place, 0);
        }
      }

      // Get directories

      int directory_max = 5;
      if ((current_directory_count + 5) > directory_count)
      {
        directory_max = directory_count - current_directory_count;
      }

      for (int i = current_directory_count; i < current_directory_count + directory_max; i++) 
      {
        if (i % 5 != 0)
        {
          printw("%d. %s", i, directory_names[i]);
          file_place++;
          move(file_place, 15);
        }
        else
        {
          printw("Directories:");
          move(file_place, 15);
          printw("%d. %s", i, directory_names[i]);
          file_place++;
          move(file_place, 15);
        }

        if (i == (current_directory_count + directory_max - 1))
        {
          move(file_place, 10);
          printw("Press ND for next directories | Press PD for previous directories");
          file_place++;
          move(file_place, 0);
        }
      }

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
      if (strcmp(command, "SL") == 0)
      {
        strcpy(sort_mode, "SL");
        need_display_reset = true;
      }
      else if (strcmp(command, "SS") == 0)
      {
        strcpy(sort_mode, "SS");
        need_display_reset = true;
      }
      else if (strcmp(command, "SN") == 0)
      {
        strcpy(sort_mode, "SN");
        need_display_reset = true;
      }
      else if (strcmp(command, "SO") == 0)
      {
        strcpy(sort_mode, "SO");
        need_display_reset = true;
      }
      break;
    }
    case 'N':
    {
      if (strcmp(command, "NF") == 0)
      {
        if ((current_de_count + 5) < den_count)
        {
          current_de_count += 5;
          need_display_reset = true;
        }
      }
      else if (strcmp(command, "ND") == 0)
      {
        if ((current_directory_count + 5) < directory_count)
        {
          current_directory_count += 5;
          need_display_reset = true;
        }
      }
      break;
    }
    case 'P':
    {
      if (strcmp(command, "PF") == 0)
      {
        if (current_de_count != 0)
        {
          current_de_count -= 5;
          need_display_reset = true;
        }
      }
      else if (strcmp(command, "PD") == 0)
      {
        if (current_directory_count != 0)
        {
          current_directory_count -= 5;
          need_display_reset = true;
        }
      }
      break;
    }
    }

    move(file_place, 15);
    if (need_display_reset == false)
    {
      getnstr(command, 32);
    }
    else
    {
      strcpy(command, "D");
      need_display_reset = false;
    }
    

    refresh();
  }
  endwin(); /* End curses mode */

  return 0;
}
