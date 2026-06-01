#ifndef INVERTED_H
#define INVERTED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE 1

// File list
typedef struct file_node
{
    char filename[20];
    struct file_node *link;
} file_list;

// Main node
typedef struct main_node
{
    char word[20];
    int file_count;
    struct sub_node *sub_link;
    struct main_node *main_link;
} main_node;

// Sub node
typedef struct sub_node
{
    char filename[20];
    int word_count;
    struct sub_node *sub_link;
} sub_node;

// Functions
int read_and_validation(int argc, char *argv[], file_list **head);
void print_list(file_list **head);

int create_hash_table(main_node *hash_table[]);
int find_index(char ch);
int create_database(file_list **head, main_node *hash_table[]);

void display_database(main_node *hash_table[]);
int search_database(main_node *hash_table[]);
int save_database(main_node *hash_table[]);
int update_database(main_node *hash_table[], file_list **head);
#endif