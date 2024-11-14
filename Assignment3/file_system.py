import os
import struct
import time
from datetime import datetime
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
        self.block_pointer_table = [] # List of block pointers
        self.num_blocks = 0 # Number of maximum blocks in the filesystem
        self.data_blocks = [] # List of data blocks

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
        
    def format_fs(self, FNT_size, DABPT_size):
        """
        Format the filesystem with the specified sizes for the FNT and DABPT.
        
        Args:
            FNT_size (int): Size of the File Name Table
            DABPT_size (int): Size of the Data Block Pointer Table
        
        Returns:
            bool: True if successful, False otherwise
        """
        try:
            self.FNT = [None] * FNT_size
            self.DABPT = [None] * DABPT_size
            return True
        except:
            return False
        
    def save_fs(self, disk_name):
        """
        Save the filesystem to a disk file.
        
        Args:
            disk_name (str): Name of the disk file to save the filesystem to
        
        Returns:
            bool: True if successful, False otherwise
        """
        try:
            with open(disk_name, 'wb') as disk_file:
                disk_file.write(bytes(str(self.FNT), 'utf-8') + b'\n')
                disk_file.write(bytes(str(self.DABPT), 'utf-8') + b'\n')
                disk_file.write(bytes(str(self.block_pointer_table), 'utf-8') + b'\n')
                disk_file.write(bytes(str(self.num_blocks), 'utf-8') + b'\n')
                disk_file.write(bytes(str(self.data_blocks), 'utf-8') + b'\n')
            
            return True
        except:
            return False

    def open_fs(self, disk_name):
        """
        Open an existing filesystem from a disk file.
        
        Args:
            disk_name (str): Name of the disk file to open
        
        Returns:
            bool: True if successful, False otherwise
        """
        try:
            with open(disk_name, 'rb') as disk_file:
                self.disk_name = disk_name
                self.FNT = eval(disk_file.readline())
                self.DABPT = eval(disk_file.readline())
                self.block_pointer_table = eval(disk_file.readline())
                self.num_blocks = eval(disk_file.readline())
                self.data_blocks = eval(disk_file.readline())
            
            return True
        except:
            return False
        
    def list(self):
        print("Disk Name: " + self.disk_name)
        print("Max Number of Blocks: " + str(self.num_blocks))
        print("FNT: " + str(self.FNT))
        print("DABPT: " + str(self.DABPT))
        print("BPT: " + str(self.block_pointer_table))
        print("Data Blocks: " + str(self.data_blocks))

    def remove(self, file_name):
        """
        Remove a file from the filesystem.
        
        Args:
            file_name (str): Name of the file to remove from the filesystem
        
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
        
        if found:
            self.FNT.pop(row_index)
            self.DABPT.pop(row_index)
            if len(self.block_pointer_table) > row_index + 1:
                number_of_pops = self.block_pointer_table[row_index + 1] - self.block_pointer_table[row_index]
            elif len(self.block_pointer_table) == 1:
                number_of_pops = len(self.data_blocks)
            else:
                number_of_pops = len(self.data_blocks) - self.block_pointer_table[row_index]
            for i in range(number_of_pops):
                self.data_blocks.pop(self.block_pointer_table[row_index])
            self.block_pointer_table.pop(row_index)
            return True
        else:
            print("ERROR: File not found.")
            return True

    def rename(self, old_name, new_name):
        found = False
        row_index = 0
        for row in self.FNT:
            if old_name == row[0]:
                found = True
                break
            row_index += 1
                
        if found:
            self.FNT[row_index][0] = new_name
            os.rename(old_name, new_name)
        else: 
            print("ERROR: Could not find file \"{old_name}\"")


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
            
            found = False
            for row in self.FNT:
                if file_name == row[0]:
                    found = True
                    break
                    
            if found:
                print("ERROR: File already exists.")
                return True
            
            blocks = []
            for block in read_file(file_name, self.BLOCK_SIZE):
                blocks.append(block)
            
            total_blocks = len(blocks) + len(self.data_blocks)
            if total_blocks > self.num_blocks:
                print("ERROR: Not enough space in the filesystem.")
                return True

            self.FNT.append([file_name, None])
            self.DABPT.append([os.path.getsize(file_name), datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S'), len(self.block_pointer_table), "username-placeholder"])
            self.block_pointer_table.append(len(self.data_blocks))
            for block in blocks:
                self.data_blocks.append(block)
            
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

        with open(file_name, 'wb') as file:    # 'w' overwrites file
            if len(self.data_blocks) > 1:
                start_index = self.block_pointer_table[row_index]
                if len(self.block_pointer_table) > row_index + 1:
                    end_index = self.block_pointer_table[row_index + 1]
                    for i in range(start_index, end_index):
                        file.write(self.data_blocks[i])
                elif len(self.block_pointer_table) == 1:
                    for i in range(len(self.data_blocks)):
                        file.write(self.data_blocks[i]) 
                else:
                    end_index = len(self.data_blocks)
                    for i in range(start_index, end_index):
                        file.write(self.data_blocks[i])

    def link(self):
        for i in range(len(self.FNT)):
            self.FNT[i][1] = i

    def unlink(self):
        for i in range(len(self.FNT)):
            self.FNT[i][1] = None
        
    def user(self, user_name):
        user_files = []
        row_index = 0
        for row in self.DABPT:
            if user_name == row[3]:
                user_files.append(self.FNT[row_index][0])
            row_index += 1
        print("The user {user_name} owns these files: " + str(user_files))

current_fs = FileSystem()

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
        current_fs.create_fs(int(cmd[1]), fs_name)
        return True
    elif cmd[0] == "fm": # Format the filesystem
        return True
    elif cmd[0] == "save": # Save the filesystem
        if len(cmd) == 1:
            print("hello")
            try:
                current_fs.save_fs(current_fs.get_fs_name())
            except Exception as e:
                print(e)
            return True
        if len(cmd) == 2:
            current_fs.save_fs(cmd[1])
            return True
        else:
            print("ERROR: Too many arguments.")
        return True
    elif cmd[0] == "open": # Open an existing filesystem
        if len(cmd) == 2:
            current_fs.open_fs(cmd[1])
            return True
        else:
            print("ERROR: Invalid arguments.")
        return True
    elif cmd[0] == "ls": # List files in the filesystem
        current_fs.list()
        return True
    elif cmd[0] == "rm": # Remove a file from the filesystem
        if len(cmd) == 2:
            current_fs.remove(cmd[1])
            return True
        else:
            print("ERROR: Invalid arguments.")
        return True
    elif cmd[0] == "mv": # Rename a file in the filesystem
        if len(cmd) == 3:
            current_fs.rename(cmd[1], cmd[2])
        else:
            print("ERROR: Invalid arguments.")
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
        if len(cmd) == 2:
            current_fs.get(cmd[1])
        else:
            print("ERROR: Invalid arguments.")
        return True
    elif cmd[0] == "user": # Link a file in the filesystem to Unix
        current_fs.user(cmd[1])
        return True
    elif cmd[0] == "link": # Link a file in the filesystem to Unix
        current_fs.link()
        return True
    elif cmd[0] == "unlink": # Link a file in the filesystem to Unix
        current_fs.unlink()
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
        print("new {num of blocks} {fs name} - Create a new filesystem")
        print("fm {filenames} {DABPTentries} - Format the filesystem - current n/a")
        print("save {disk name} - Save the filesystem")
        print("open {disk name} - Open an existing filesystem")
        print("ls - List files in the filesystem")
        print("rm {file name}- Remove a file from the filesystem")
        print("mv {old file name} {new file name} - Rename a file in the filesystem")
        print("put {file name} - Put a file into the filesystem")
        print("get {file name} - Get a file from the filesystem")
        print("user {username}- Link a file in the filesystem to Unix")
        print("link - Link a file in the filesystem to Unix")
        print("unlink - Unlink a file in the filesystem to Unix")
        print("q - Quit the program")
        command = input("Enter a command: ").lower()
        fs_running = run_command(command)

if __name__ == "__main__":
    main()