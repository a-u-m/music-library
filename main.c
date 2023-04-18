#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sndfile.h>
#include <SDL2/SDL.h>

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
struct SongNode *SongInsert(struct SongNode *root, char* song, int n, char* path)
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

int main(){
    char* audio_dir = "./audio/";
    DIR *d;
    struct dirent *dir;
    d = opendir(audio_dir);
    STrie.root = NULL;

    if (d){
        while ((dir = readdir(d)) != NULL){
            if(dir->d_type != DT_REG) continue;
            char audio_filepath[50];
            strcpy(audio_filepath,audio_dir);
            strcat(audio_filepath,dir->d_name);

            SNDFILE *snd_audiofile;
            SF_INFO info;
            snd_audiofile = sf_open(audio_filepath, SFM_READ, &info);

            const char *artist = sf_get_string(snd_audiofile, SF_STR_ARTIST);
            const char *ablum = sf_get_string(snd_audiofile, SF_STR_ALBUM);
            const char *genre = sf_get_string(snd_audiofile, SF_STR_GENRE);

            for(int i = 0;i < len(dir->d_name);i++){
                if(dir->d_name[i]=='.'){
                    dir->d_name[i] = '\0';
                    break;
                }
            }
    
            printf("\nPath: %s\n", audio_filepath);
            printf("Song: %s\n", dir->d_name);
            printf("Artist: %s\n", artist);
            printf("Album: %s\n", ablum);
            printf("Genre: %s\n\n", genre);

            STrie.root = SongInsert(STrie.root, dir->d_name, len(dir->d_name), audio_filepath);
        

            sf_close(snd_audiofile);
        }
        closedir(d);
    }

    char song[100];
    printf("enter the song you want to search:");
    scanf("%s", song);
    int check = SongSearch(STrie.root, song, 0, len(song));
    if (!check){
        printf("song not found");
    }
}
