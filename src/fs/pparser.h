#ifndef PATHPARSER_H
#define PATHPARSER_H


/** @brief describes path contains disk id part of the path */
struct path_root
{
    int drive_no;
    struct path_part* first;
};

/** @brief describes path contains folder and file names part of the path */
struct path_part
{
    const char* part;
    struct path_part* next;
};

struct path_root* pathparser_parse(const char* path, const char* current_directory_path);
void pathparser_free(struct path_root* root);

#endif