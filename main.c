/*
Name: K PAVAN SAI KUMAR
Date: 23/04/2026
Description: An inverted index is an index data structure storing a mapping from content, such as words or
numbers, to its locations in a database file, or in a document or a set of documents. The purpose of
an inverted index is to allow fast full text searches, at the cost of increased processing when a
document is added to the database. The inverted file may be the database file itself, rather than its
index. It is the most popular data structure used in document retrieval systems, used on a large
scale for example in search engines.
*/
#include "inverted.h"

int main(int argc, char *argv[])
{
    file_list *head = NULL;                 // head of file linked list
    main_node *hash_table[27] = {NULL};     // hash table (0–25 alphabets, 26 special chars)

    // -------- VALIDATION --------
    if (read_and_validation(argc, argv, &head) == FAILURE)
    {
        printf("ERROR: validation failed.\n");
        return FAILURE;                     // stop if validation fails
    }

    print_list(&head);                      // display list of valid input files

    create_hash_table(hash_table);          // initialize all hash table indexes to NULL

    // -------- MENU LOOP --------
    while (1)
    {
        printf("\nSelect option:\n");
        printf("1.Create database\n");
        printf("2.Display database\n");
        printf("3.Search database\n");
        printf("4.Save database\n");
        printf("5.Update database\n");
        printf("6.Exit\n");

        int option;
        scanf("%d", &option);               // read user choice

        switch (option)
        {
            case 1:
                printf("Creating database...\n");

                create_database(&head, hash_table);   // read files and build hash table

                printf("Database created successfully\n");
                break;

            case 2:
                display_database(hash_table);         // print full database
                break;

            case 3:
                search_database(hash_table);          // search a word in database
                break;

            case 4:
                save_database(hash_table);            // save database to file
                break;

            case 5:
                update_database(hash_table, &head);   // load database from backup file
                break;

            case 6:
                return 0;                             // exit program
        }
    }
}