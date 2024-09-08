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

void swap(long int *arr, char dir_entries[][256], char directory_entry_readable_dates[][256], int i, int j)
{
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;

  char temp_str[256];
  strcpy(temp_str, dir_entries[i]);
  strcpy(dir_entries[i], dir_entries[j]);
  strcpy(dir_entries[j], temp_str);

  char date_str[256];
  strcpy(date_str, directory_entry_readable_dates[i]);
  strcpy(directory_entry_readable_dates[i], directory_entry_readable_dates[j]);
  strcpy(directory_entry_readable_dates[j], date_str);

  
}

void bubble_sort_asc_size(long int sizes[], char directory_entries[][256], char directory_entry_readable_dates[][256], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    for (int j = 0; j < n - 1; j++)
    {
      if (sizes[j] > sizes[j + 1])
      {
        swap(sizes, directory_entries, directory_entry_readable_dates, j, j + 1);
      }
    }
  }
}

void bubble_sort_desc_size(long int sizes[], char directory_entries[][256], char directory_entry_readable_dates[][256], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    for (int j = 0; j < n - 1; j++)
    {
      if (sizes[j] < sizes[j + 1])
      {
        swap(sizes, directory_entries, directory_entry_readable_dates, j, j + 1);
      }
    }
  }
}
void get_cwd_and_time(char current_directory[256], char sort_mode[32])
{
  // Get current directory
  move(0, 0);
  printw("Current Working Dir: %s", current_directory);

  // Get current time
  move(1, 0);
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  printw("It is now: %s", asctime(timeinfo));

  // Get sorting mode
  move(2, 0);
  printw("Sorting Mode: %s", sort_mode);
}

int main()
{
  /* Start curses mode */
  initscr();

  // Start shell in display mode
  char command[100] = "D";
  int file_place = 0;

  // Variables that control display
  bool need_display_reset = false;
  char sort_mode[32] = "Default";
  bool is_new_directory = true;

  // Initializing current directory as the folder that runs this shell
  DIR *curr_directory;
  char current_directory[256];
  getcwd(current_directory, 200);
  char directory_to_enter[256] = ".";

  // Directory entry and name logs initialized
  struct dirent *directory_entry;
  char directory_entry_names[1024][256];
  long int directory_entry_sizes[1024];
  long int directory_entry_dates[1024];
  char directory_entry_readable_dates[1024][256];
  int current_directory_count, current_de_count, den_count = 0;
  char de_first_five[5][256];
  long int de_first_sizes[5];
  char de_first_readable_dates[5][256];
  long int de_first_dates[5];
  int directory_count = 0;
  char directory_names[100][64];

  // Create stat object to get directory entry sizes
  struct stat file_info;

  // Shell loop
  while (strcmp(command, "Q") != 0)
  {
    switch (command[0])
    {
    case 'D':
    {
      if (is_new_directory)
      {
        for (int i = 0; i < den_count; i++)
        {
          memset(directory_entry_names[i], '\0', sizeof(directory_entry_names[i]));
          directory_entry_sizes[i] = 0;
        }
        den_count = 0;

        for (int i = 0; i < directory_count; i++)
        {
          memset(directory_names[i], '\0', sizeof(directory_names[i]));
        }
        directory_count = 0;

        curr_directory = opendir(directory_to_enter);
        chdir(directory_to_enter);
        getcwd(current_directory, 256);

        // Get directory entries and entry sizes
        while ((directory_entry = readdir(curr_directory)))
        {
          if (((directory_entry->d_type) & DT_REG))
          {
            // Grabs directory entry names and puts into list
            strcpy(directory_entry_names[den_count], directory_entry->d_name);

            char file_path[500];
            char readable_time[100];
            time_t mod_time = file_info.st_mtime;
            struct tm *time_info = localtime(&mod_time); // Convert to local time structure
            if (strftime(readable_time, sizeof(readable_time), "%Y-%m-%d %H:%M:%S", time_info) == 0)
            {
              fprintf(stderr, "strftime failed\n");
              return EXIT_FAILURE;
            }
            snprintf(file_path, sizeof(file_path), "./%s", directory_entry->d_name);
            if (stat(file_path, &file_info) == 0)
            {
              directory_entry_sizes[den_count] = file_info.st_size;
              directory_entry_dates[den_count] = (long int)mod_time;
              strcpy(directory_entry_readable_dates[den_count], readable_time);
            }

            // Format the time

            den_count++;
          }
        }

        rewinddir(curr_directory);

        // Get directories

        while ((directory_entry = readdir(curr_directory)))
        {
          if (((directory_entry->d_type) & DT_DIR))
          {
            strcpy(directory_names[directory_count++], directory_entry->d_name);
          }
        }
        rewinddir(curr_directory);

        closedir(curr_directory);
        is_new_directory = false;
      }

      // file_place determines where files, directories, and operations are printed
      file_place = 3;

      clear();

      // Get current directory and current time
      get_cwd_and_time(current_directory, sort_mode);

      // Get files
      move(file_place, 0);

      // Check if the current count + 5 is less than the total to get max
      int de_max = 5;
      if ((current_de_count + 5) > den_count)
      {
        de_max = den_count - current_de_count;
      }

      // Get max number (0-5) of files and their sizes
      for (int i = current_de_count; i < current_de_count + de_max; i++)
      {
        strcpy(de_first_five[i % 5], directory_entry_names[i]);
        de_first_sizes[i % 5] = directory_entry_sizes[i];
        strcpy(de_first_readable_dates[i % 5], directory_entry_readable_dates[i]);
        de_first_dates[i % 5] = directory_entry_dates[i];
      }

      int index_list[5];

      if (strcmp(sort_mode, "SL") == 0)
      {
        bubble_sort_desc_size(de_first_sizes, de_first_five, de_first_readable_dates, de_max);
      }
      if (strcmp(sort_mode, "SS") == 0)
      {
        bubble_sort_asc_size(de_first_sizes, de_first_five, de_first_readable_dates, de_max);
      }
      if (strcmp(sort_mode, "SN") == 0)
      {
        bubble_sort_desc_size(de_first_dates, de_first_five, de_first_readable_dates, de_max);
      }
      if (strcmp(sort_mode, "SO") == 0)
      {
        bubble_sort_asc_size(de_first_dates, de_first_five, de_first_readable_dates, de_max);
      }

      for (int i = 0; i < de_max; i++)
      {
        if (i != 0)
        {
          printw("%d. %ld bytes\t %s \t%s", current_de_count + i, de_first_sizes[i], de_first_readable_dates[i], de_first_five[i]);
          file_place++;
          move(file_place, 15);
        }
        else
        {
          printw("Files:");
          move(file_place, 15);
          printw("%d. %ld bytes\t %s \t%s", current_de_count + i, de_first_sizes[i], de_first_readable_dates[i], de_first_five[i]);
          file_place++;
          move(file_place, 15);
        }

        if (i == (de_max - 1))
        {
          move(file_place, 10);
          printw("Press NF for next files | Press PF for previous files");
          file_place++;
          move(file_place, 10);
          printw("Press SL to sort large to small | Press SS to sort small to large");
          file_place++;
          move(file_place, 10);
          printw("Press SN to sort newest to oldest | Press SO to sort oldest to newest");
          file_place += 2;
          move(file_place, 0);
        }
      }

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
      printw("C Change");
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
    case 'C':
    {
      char destination[200];
      strncpy(destination, command + 2, 200);
      strcpy(directory_to_enter, destination);
      is_new_directory = true;
      need_display_reset = true;
      current_de_count = 0;
      current_directory_count = 0;

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
      else
      {
        strcpy(sort_mode, "Default");
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
