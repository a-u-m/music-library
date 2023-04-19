// random
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int len(char arr[])
{
    int res = 0;
    int i = 0;
    while (arr[i] != '\0')
    {
        res++;
        i++;
    }
    return res;
}
struct SongNode
{
    char s;
    struct SongNode *srr[27];
    int terminal;
    char path[100];
};
struct SongTrie
{
    struct SongNode *root;
};
struct SongTrie STrie;
struct SongNode *SongInsert(struct SongNode *root, char song[], int n, char path[])
{
    if (root == NULL)
    {
        root = (struct SongNode *)(malloc(sizeof(struct SongNode)));
        strcpy(root->path, "\0");
        root->s = '\0';
        root->terminal = 0;
        for (int i = 0; i < 26; i++)
        {
            root->srr[i] = NULL;
        }
    }
    struct SongNode *temp = root;
    for (int i = 0; i < n; i++)
    {
        if (temp->srr[song[i] - 'a'] == NULL)
        {
            struct SongNode *new = (struct SongNode *)(malloc(sizeof(struct SongNode)));
            new->terminal = 0;
            new->s = song[i];
            for (int k = 0; k < 26; k++)
            {
                new->srr[k] = NULL;
            }
            strcpy(new->path, "\0");
            temp->srr[song[i] - 'a'] = new;
        }
        temp = temp->srr[song[i] - 'a'];
    }
    temp->terminal = 1;
    strcpy(temp->path, path);
    return root;
}
int SongSearch(struct SongNode *root, char song[], int i, int n)
{
    // base case
    if (i == n)
    {
        printf("The path of the song is: %s", root->path);
        return 1;
    }
    // recursive case
    if (root->srr[song[i] - 'a'] != NULL)
    {
        return SongSearch(root->srr[song[i] - 'a'], song, i + 1, n);
    }
    else
    {
        return 0;
    }
}

int main()
{
    // initializing the roots of all tries here
    STrie.root = NULL;
    // demo of insertion and searching in song tree
    // Insertion of songs
    STrie.root = SongInsert(STrie.root, "solo", sizeof("solo") / sizeof(char) - 1, "./audio/solo.wav");
    STrie.root = SongInsert(STrie.root, "high", sizeof("high") / sizeof(char) - 1, "./audio/high.wav");
    STrie.root = SongInsert(STrie.root, "sunflower", sizeof("sunflower") / sizeof(char) - 1, "./audio/sunflower.wav");
    STrie.root = SongInsert(STrie.root, "circles", sizeof("circles") / sizeof(char) - 1, "./audio/circles.wav");
    printf("Songs Inserted successfully!!\n");
    char song[100];
    // searching (So in C we cannot return an array so we need to store the path in a variable that we will pass in the function, for now i m just printing the path if the song is found)

    printf("enter the song you want to search:");
    scanf("%s", &song);
    int check = SongSearch(STrie.root, song, 0, len(song));
    if (!check)
    {
        printf("song not found");
    }
}