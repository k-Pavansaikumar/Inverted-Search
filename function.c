#include "inverted.h"

// -------- VALIDATION --------
int read_and_validation(int argc, char *argv[], file_list **head)
{
    if (argc == 1)                                  // no input files passed
    {
        printf("ERROR: No arguments passed.\n");
        return FAILURE;
    }

    for (int i = 1; i < argc; i++)                  // loop through all input files
    {
        if (strstr(argv[i], ".txt") != NULL)        // check .txt extension
        {
            FILE *fp = fopen(argv[i], "r");         // open file in read mode
            if (fp == NULL)
            {
                printf("ERROR: Cannot open %s\n", argv[i]);
                continue;                           // skip invalid file
            }

            fseek(fp, 0, SEEK_END);                 // move pointer to end
            if (ftell(fp) == 0)                     // check file size
            {
                printf("ERROR: %s is empty\n", argv[i]);
                fclose(fp);
                continue;
            }
            fclose(fp);

            file_list *new = malloc(sizeof(file_list));   // create new node
            strcpy(new->filename, argv[i]);               // store filename
            new->link = NULL;

            if (*head == NULL)                            // first node
                *head = new;
            else
            {
                file_list *temp = *head;
                while (temp->link)                        // go to last
                    temp = temp->link;
                temp->link = new;                         // insert at end
            }
        }
    }
    return SUCCESS;
}

// -------- PRINT FILE LIST --------
void print_list(file_list **head)
{
    file_list *temp = *head;

    printf("Files: ");
    while (temp != NULL)                         // traverse list
    {
        printf("%s -> ", temp->filename);
        temp = temp->link;
    }
    printf("NULL\n");
}

// -------- HASH TABLE INIT --------
int create_hash_table(main_node *hash_table[])
{
    for (int i = 0; i < 27; i++)                 // initialize all indexes
        hash_table[i] = NULL;
    return SUCCESS;
}

// -------- FIND INDEX --------
int find_index(char ch)
{
    if (ch >= 'a' && ch <= 'z')                  // lowercase
        return ch - 'a';
    else if (ch >= 'A' && ch <= 'Z')             // uppercase
        return ch - 'A';
    else
        return 26;                               // special characters
}

// -------- CREATE DATABASE --------
int create_database(file_list **head, main_node *hash_table[])
{
    file_list *f_temp = *head;                   // file traversal pointer

    while (f_temp != NULL)                       // loop through files
    {
        FILE *fp = fopen(f_temp->filename, "r");

        char word[20];

        while (fscanf(fp, "%s", word) != EOF)    // read word by word
        {
            int index = find_index(word[0]);     // find hash index

            main_node *m_temp = hash_table[index];
            main_node *prev_m = NULL;

            if (m_temp == NULL)                 // no word at index
            {
                main_node *new_main = malloc(sizeof(main_node));
                strcpy(new_main->word, word);
                new_main->file_count = 1;
                new_main->main_link = NULL;

                sub_node *new_sub = malloc(sizeof(sub_node));
                strcpy(new_sub->filename, f_temp->filename);
                new_sub->word_count = 1;
                new_sub->sub_link = NULL;

                new_main->sub_link = new_sub;
                hash_table[index] = new_main;   // insert into hash table
            }
            else
            {
                while (m_temp != NULL)          // traverse main nodes
                {
                    if (strcmp(m_temp->word, word) == 0)   // word found
                    {
                        sub_node *s_temp = m_temp->sub_link;
                        sub_node *prev_s = NULL;

                        while (s_temp != NULL)   // traverse sub nodes
                        {
                            if (strcmp(s_temp->filename, f_temp->filename) == 0)
                            {
                                s_temp->word_count++;   // increment count
                                break;
                            }
                            prev_s = s_temp;
                            s_temp = s_temp->sub_link;
                        }

                        if (s_temp == NULL)     // file not found
                        {
                            sub_node *new_sub = malloc(sizeof(sub_node));
                            strcpy(new_sub->filename, f_temp->filename);
                            new_sub->word_count = 1;
                            new_sub->sub_link = NULL;

                            prev_s->sub_link = new_sub;
                            m_temp->file_count++;   // increase file count
                        }
                        break;
                    }
                    prev_m = m_temp;
                    m_temp = m_temp->main_link;
                }

                if (m_temp == NULL)             // word not found
                {
                    main_node *new_main = malloc(sizeof(main_node));
                    strcpy(new_main->word, word);
                    new_main->file_count = 1;
                    new_main->main_link = NULL;

                    sub_node *new_sub = malloc(sizeof(sub_node));
                    strcpy(new_sub->filename, f_temp->filename);
                    new_sub->word_count = 1;
                    new_sub->sub_link = NULL;

                    new_main->sub_link = new_sub;
                    prev_m->main_link = new_main;
                }
            }
        }

        fclose(fp);
        f_temp = f_temp->link;                 // next file
    }

    return SUCCESS;
}

// -------- DISPLAY --------
void display_database(main_node *hash_table[])
{
    for (int i = 0; i < 27; i++)              // loop all indexes
    {
        main_node *m_temp = hash_table[i];

        while (m_temp != NULL)
        {
            printf("\n[%d] %s (%d) -> ", i, m_temp->word, m_temp->file_count);

            sub_node *s_temp = m_temp->sub_link;

            while (s_temp != NULL)
            {
                printf("%s (%d) ", s_temp->filename, s_temp->word_count);
                s_temp = s_temp->sub_link;
            }

            m_temp = m_temp->main_link;
        }
    }
    printf("\n");
}

// -------- SEARCH --------
int search_database(main_node *hash_table[])
{
    char word[20];

    printf("Enter word: ");
    scanf("%s", word);

    int index = find_index(word[0]);       // get index

    main_node *m_temp = hash_table[index];

    while (m_temp != NULL)
    {
        if (strcmp(m_temp->word, word) == 0)
        {
            printf("\nFound: %s\n", word);

            sub_node *s_temp = m_temp->sub_link;

            while (s_temp != NULL)
            {
                printf("%s -> %d\n", s_temp->filename, s_temp->word_count);
                s_temp = s_temp->sub_link;
            }

            return SUCCESS;
        }
        m_temp = m_temp->main_link;
    }

    printf("Word not found\n");
    return FAILURE;
}
int save_database(main_node *hash_table[])
{
    char file[30];                               // to store output filename

    printf("Enter file name to save: ");
    scanf("%s", file);                           // read filename

    if (strstr(file, ".txt") == NULL)            // check .txt extension
    {
        printf("File must be .txt\n");
        return FAILURE;
    }

    FILE *fp = fopen(file, "w");                 // open file in write mode
    if (fp == NULL)
    {
        printf("File not opened\n");
        return FAILURE;
    }

    for (int i = 0; i < 27; i++)                 // loop through hash table
    {
        main_node *m_temp = hash_table[i];       // get main node

        while (m_temp != NULL)                  // traverse main nodes
        {
            fprintf(fp, "# %d;%s;%d;", i, m_temp->word, m_temp->file_count);
                                                    // write index, word, file count

            sub_node *s_temp = m_temp->sub_link;

            while (s_temp != NULL)              // traverse sub nodes
            {
                fprintf(fp, "%s;%d;", s_temp->filename, s_temp->word_count);
                                                    // write file name + count
                s_temp = s_temp->sub_link;
            }

            fprintf(fp, "#\n");                 // end of one word entry
            m_temp = m_temp->main_link;         // next main node
        }
    }

    fclose(fp);                                 // close file
    printf("Database saved successfully\n");
    return SUCCESS;
}
int update_database(main_node *hash_table[], file_list **head)
{
    char file[30];                               // backup file name

    printf("Enter backup file: ");
    scanf("%s", file);                           // read filename

    FILE *fp = fopen(file, "r");                 // open backup file
    if (fp == NULL)
    {
        printf("File not found\n");
        return FAILURE;
    }

    int index, file_count, word_count;           // variables for reading data
    char word[30], fname[30];

    while (fscanf(fp, "# %d;%[^;];%d;", &index, word, &file_count) == 3)
                                                    // read one main node
    {
        main_node *new_main = malloc(sizeof(main_node));   // create main node
        strcpy(new_main->word, word);                     // store word
        new_main->file_count = file_count;                // set file count
        new_main->main_link = NULL;
        new_main->sub_link = NULL;

        sub_node *last = NULL;                            // to link sub nodes

        for (int i = 0; i < file_count; i++)              // read sub nodes
        {
            fscanf(fp, "%[^;];%d;", fname, &word_count);

            sub_node *new_sub = malloc(sizeof(sub_node)); // create sub node
            strcpy(new_sub->filename, fname);             // store filename
            new_sub->word_count = word_count;             // store count
            new_sub->sub_link = NULL;

            if (new_main->sub_link == NULL)               // first sub node
                new_main->sub_link = new_sub;
            else
                last->sub_link = new_sub;                 // link at end

            last = new_sub;
        }

        fscanf(fp, "#\n");                                // skip closing #

        int idx = find_index(word[0]);                    // find index

        if (hash_table[idx] == NULL)                      // insert main node
            hash_table[idx] = new_main;
        else
        {
            main_node *temp = hash_table[idx];
            while (temp->main_link != NULL)               // go to last
                temp = temp->main_link;

            temp->main_link = new_main;                   // insert at end
        }
    }

    fclose(fp);                                          // close file

    create_database(head, hash_table);                   // rebuild remaining data

    printf("Database updated successfully\n");
    return SUCCESS;
}