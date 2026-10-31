#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

typedef enum FileErr
{
    FILENAME_DOESNT_EXIST_ERR,
    READ_FILE_ERR,
    UNKNOWN_ERR
} file_err_t;

/// @brief `FileHandler` is a struct that will do all file operations on the input file/files.
/// If you want to add an operation to this file, create a function declaration, and add it as a function pointer
/// this way we can simulate methods.
typedef struct FileHandler
{
    file_err_t (*read_file)(struct FileHandler *self, char filename[]);
} file_handler_t;

file_err_t read_file(file_handler_t* fh, char filename[]);

#endif
