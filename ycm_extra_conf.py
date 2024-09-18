import os
import ycm_core

# Flags based on your project requirements
flags = [
    '-Wall -Wextra -Werror',
    '-std=c99',           # C standard provided by the user
    '-I./include -I/usr/local/include',          # Project include directories
    '-I/include',
    '-I/usr/local/include',
]

compilation_database_folder = ''

def DirectoryOfThisScript():
    return os.path.dirname(os.path.abspath(__file__))

def MakeRelativePathsAbsolute(flags, working_directory):
    if not working_directory:
        return list(flags)
    new_flags = []
    for flag in flags:
        new_flag = flag
        if flag.startswith('-I'):
            path = flag[2:]
            if not os.path.isabs(path):
                new_flag = '-I' + os.path.join(working_directory, path)
        new_flags.append(new_flag)
    return new_flags

def FlagsForFile(filename):
    return {
        'flags': MakeRelativePathsAbsolute(flags, DirectoryOfThisScript()),
        'do_cache': True
    }
