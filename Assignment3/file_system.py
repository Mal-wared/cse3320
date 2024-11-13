import os
import struct
import time
import random

class FileSystem:
    BLOCK_SIZE = 256  # Fixed block size in bytes
    FILE_NAME_SIZE = 56  # Fixed file name size in bytes
    USERNAME_SIZE = 40 

    def __init__(self):
        self.fs_name = ""  # name of the file system (disk)
        self.FNT = []  # List of tuples (filename[FILE_NAME_SIZE], inode_pointer)
        self.DABPT = []  # List of (file_size, last_modified_time, block_pointer_table_index, username[USERNAME_SIZE])
        self.block_pointer_table = [] # 
        self.blocks = []
    
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
            # Store filesystem metadata
            self.disk_name = disk_name
            self.num_blocks = num_blocks
            
            # Create the disk file
            with open(disk_name, 'wb') as disk_file:
                # Initialize each block with zeros
                zero_block = bytes([0] * self.BLOCK_SIZE)
                
                # Write the superblock (block 0)
                # Format: magic number (4 bytes), num_blocks (4 bytes), creation_time (8 bytes)
                magic_number = 0x4653535F  # '_FSS' in hex
                creation_time = int(time.time())
                
                superblock = struct.pack('>IIQ', 
                    magic_number,
                    num_blocks,
                    creation_time
                )
                
                # Pad the superblock to BLOCK_SIZE
                superblock_padded = superblock + bytes([0] * (self.BLOCK_SIZE - len(superblock)))
                disk_file.write(superblock_padded)
                
                # Write the remaining blocks initialized to zero
                for _ in range(num_blocks - 1):
                    disk_file.write(zero_block)
                
            print(f"Successfully created filesystem '{disk_name}' with {num_blocks} blocks")
            return True
        except Exception as e:
            print(f"Error creating filesystem: {str(e)}")
            return False
    
    def validate_fs(self, disk_name):
        """
        Validates if a given file is a valid FS filesystem.
        
        Args:
            disk_name (str): Name of the disk file to validate
            
        Returns:
            tuple: (is_valid, num_blocks) or (False, 0) if invalid
        """
        try:
            with open(disk_name, 'rb') as disk_file:
                # Read and verify the superblock
                superblock = disk_file.read(self.BLOCK_SIZE)
                magic_number, num_blocks, creation_time = struct.unpack('>IIQ', superblock[:16])
                
                if magic_number != 0x4653535F:  # '_FSS' in hex
                    return False, 0
                
                # Verify file size matches expected size
                file_size = os.path.getsize(disk_name)
                expected_size = num_blocks * self.BLOCK_SIZE
                
                if file_size != expected_size:
                    return False, 0
                
                return True, num_blocks
                
        except Exception:
            return False, 0
        
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
        
        fs_name = "disk"

        if len(cmd) == 1:
            print("ERROR: Please enter the number of blocks.")
            return True
        elif len(cmd) == 3:
            
        elif len(cmd) > 3:
            print("ERROR: Too many arguments.")
            return True
        # Example usage
        fs = FileSystem()
        
        # Create a filesystem with 100 blocks
        fs.create_fs(100, )
        
        # Validate the created filesystem
        is_valid, num_blocks = fs.validate_fs("disk01")
        if is_valid:
            print(f"Filesystem is valid with {num_blocks} blocks")
        else:
            print("Filesystem validation failed")
        return True
    elif cmd[0] == "fm": # Format the filesystem
        return True
    elif cmd[0] == "save": # Save the filesystem
        return True
    elif cmd[0] == "open": # Open an existing filesystem
        return True
    elif cmd[0] == "ls": # List files in the filesystem
        return True
    elif cmd[0] == "rm": # Remove a file from the filesystem
        return True
    elif cmd[0] == "mv": # Rename a file in the filesystem
        return True
    elif cmd[0] == "store": # Put a file into the filesystem
        return True
    elif cmd[0] == "get": # Get a file from the filesystem
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