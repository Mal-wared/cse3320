import os
import struct
import time
import random
import getpass


def read_file(filename, block_size):
  """Reads a file in chunks of the specified size.

  Args:
    filename: The name of the file to read.
    chunk_size: The size of each chunk to read, in bytes.

  Yields:
    A chunk of data from the file.
  """

  with open(filename, 'rb') as f:
    while True:
      chunk = f.read(block_size)
      if not chunk:
        break
      yield chunk

class FileSystem:
    BLOCK_SIZE = 256  # Fixed block size in bytes
    FILE_NAME_SIZE = 56  # Fixed file name size in bytes
    USERNAME_SIZE = 40 

    def __init__(self):
        self.fs_name = ""  # name of the file system (disk)
        self.FNT = []  # List of tuples (filename[FILE_NAME_SIZE], inode_pointer)
        self.DABPT = []  # List of (file_size, last_modified_time, block_pointer_table_index, username[USERNAME_SIZE])
        self.block_pointer_table = [] #  
        self.data_blocks = []
    
    def create_fs(self, num_blocks, disk_name="disk01"):
        """
        Creates a new filesystem with the specified number of blocks.
        Each block is 256 bytes in size.
        
        Args:
            num_blocks (int): Number of blocks to allocate
            disk_name (str): Name of the disk file to create
        
        Returns:
            bool: True if successful, False otherwise
        """

        try:
            self.disk_name = disk_name
            self.num_blocks = num_blocks
            return True
        except:
            return False
        
    def open_fs(self, disk_name):
        """
        Open an existing filesystem.
        
        Args:
            disk_name (str): Name of the disk file to open
        
        Returns:
            bool: True if successful, False otherwise
        """
        try:
            self.disk_name = disk_name
            return True
        except:
            return False
        
    def put(self, file_name):
        """
        Put a file into the filesystem.
        
        Args:
            file_name (str): Name of the file to put into the filesystem
        
        Returns:
            bool: True if successful, False otherwise
        """
        try:
            if len(file_name) > 56:
                print("ERROR: File name is too large.")
                return True
            
            self.FNT.append([file_name, len(self.DABPT)])
            self.DABPT.append([os.path.getsize(file_name), time.time(), len(self.block_pointer_table), "username placeholder"])
            self.block_pointer_table.append(len(self.data_blocks))

            print("hello")

            for block in read_file(file_name, self.BLOCK_SIZE):
                self.data_blocks.append(block)
            print("hello1")
            print("Disk Name: " + self.disk_name)
            print("hello2")
            print("FNT: " + str(self.FNT))
            print("hello3")
            print("DABPT: " + str(self.DABPT))
            print("BPT: " + str(self.block_pointer_table))
            print("Data Blocks: " + str(self.data_blocks))
            return True
        except:
            return False

    def get(self, file_name):
        """
        Get a file from the filesystem.
        
        Args:
            file_name (str): Name of the file to get from the filesystem
        
        Returns:
            bool: True if successful, False otherwise
        """
        found = False
        row_index = 0
        for row in self.FNT:
            if file_name == row[0]:
                found = True
                break
            row_index += 1
        
        if not found:
            print("ERROR: File not found.")
            return True

        with open(file_name, 'w') as file:    # 'w' overwrites file
            if len(self.data_blocks) > 1:
                start_index = self.block_pointer_table[row_index]
                end_index = self.block_pointer_table[row_index + 1]
                for i in range(start_index, end_index):
                    file.write(self.data_blocks[i].decode('utf-8'))

        

current_fs = FileSystem()

def show_disk(disk_name):
    """
    Display the contents of a disk file.
    
    Args:
        disk_name (str): Name of the disk file to display
    """
    try:
        with open(disk_name, 'rb') as disk_file:
            # Read and display each block
            for block_num in range(0, num_blocks):
                block = disk_file.read(BLOCK_SIZE)
                print(f"Block {block_num}: {block}")
    except Exception as e:
        print(f"Error reading disk: {str(e)}")

def run_command(command):
    '''
    Createfs: new {num_blocks} {fs_name} - creates a filesystem (disk) with #ofblocks size, each 256 bytes
    Formatfs: fm {FNT_size} {DABPT_size}
    Savefs: save {fs_name} - save the “disk” image in a file “name”
    Openfs: open {fs_name} - use an existing disk image
    List: ls - list files (and other meta-information) in a FS
    Remove: rm {fs_name} - remove named file from fs
    Rename: mv {file_name} {new_file_name} - rename a file stored in the FS
    Put: store {file_name} - put (store) Host OS file into the disk
    Get: get {file_name} - get disk file, copy from “disk” to host OS file system
    User: user {user_name} - this user owns this user’s files
    Link/Unlink: link - Unix style file linking
    '''
    cmd = command.split(" ")
    quit_args = ["exit", "quit", "q", "die", "bye"]
    goodbye_messages = ["Later alligator", "Bye nerd", "Goodbye cruel world", "See you later", "DON'T LEAVE ME"]
    

    if cmd[0] == "new": # Create a new filesystem
        fs_name = ""

        if len(cmd) == 1:
            print("ERROR: Please enter the number of blocks.")
            return True
        elif len(cmd) == 2:
            disk_names = []
            current_dir = os.getcwd()
            print(f"Current working directory: {current_dir}")

            for file in os.listdir(current_dir):
                file_path = os.path.join(current_dir, file)
                if os.path.isfile(file_path):
                    if file.startswith("disk"):
                        disk_names.append(file)
            
            disk_names.sort()

            n = 1
            index = "01"
            for disk_name in disk_names:
                if disk_name == f"disk{index}":
                    n += 1
                    index = f"0{n}"
                else:
                    break

            fs_name = f"disk{index}"
        elif len(cmd) == 3:
            fs_name = cmd[2]
        else:
            print("ERROR: Too many arguments.")
            return True
        
        # Create a filesystem with a specified # of blocks and name
        current_fs.create_fs(cmd[1], fs_name)
        return True
    elif cmd[0] == "fm": # Format the filesystem
        return True
    elif cmd[0] == "save": # Save the filesystem
        if len(cmd) == 1:
            print("ERROR: Please enter the name of the file to save.")
            return True
        if len(cmd) == 2:
            current_fs.save_fs(cmd[1])
            return True
        else:
            print("ERROR: Too many arguments.")
            return True
    elif cmd[0] == "open": # Open an existing filesystem
        return True
    elif cmd[0] == "ls": # List files in the filesystem
        return True
    elif cmd[0] == "rm": # Remove a file from the filesystem
        return True
    elif cmd[0] == "mv": # Rename a file in the filesystem
        return True
    elif cmd[0] == "put": # Put a file into the filesystem
        if len(cmd) == 1:
            print("ERROR: Please enter the name of the external file to put into the file system.")
            return True
        if len(cmd) == 2:
            current_fs.put(cmd[1])
            return True
        else:
            print("ERROR: Too many arguments.")
            return True
    elif cmd[0] == "get": # Get a file from the filesystem
        current_fs.get(cmd[1])
        return True
    elif cmd[0] == "user": # Get the files owned by a user
        return True
    elif cmd[0] == "link": # Link a file in the filesystem to Unix
        return True
    elif cmd[0] in quit_args: # Quit the program
        print(random.choice(goodbye_messages))
        return False
    else:
        print("Invalid command")
        return True


def main():

    fs_running = True
    while (fs_running):
        command = input("Enter a command: ").lower()
        fs_running = run_command(command)

if __name__ == "__main__":
    main()