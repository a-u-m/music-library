#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sndfile.h>
#include <stdio.h>
#include <SDL2/SDL.h>

void playsong(const char* filename) {
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);

    //SDL_WINDOWPOS_UNDEFINED

    SDL_Window* window = SDL_CreateWindow("music-lib", 0, 0, 300, 70, 0);
    if (window == NULL) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        return;
    }

    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    if (SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength) == NULL) {
        fprintf(stderr, "Could not load %s: %s\n", filename, SDL_GetError());
        return;
    }

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (deviceId == 0) {
        fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
        SDL_FreeWAV(wavBuffer);
        return;
    }

    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    if (success < 0) {
        fprintf(stderr, "Failed to queue audio: %s\n", SDL_GetError());
        SDL_CloseAudioDevice(deviceId);
        SDL_FreeWAV(wavBuffer);
        return;
    }

    SDL_PauseAudioDevice(deviceId, 0);

    int isPlaying = 1;
    int isPaused = 0;
    while (isPlaying) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isPlaying = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        isPlaying = 0;
                    } else if (event.key.keysym.sym == SDLK_SPACE) {
                        if (isPaused) {
                            SDL_PauseAudioDevice(deviceId, 0);
                            isPaused = 0;
                        } else {
                            SDL_PauseAudioDevice(deviceId, 1);
                            isPaused = 1;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) {
            isPlaying = 0;
        }
        if (SDL_GetQueuedAudioSize(deviceId) == 0) {
            isPlaying = 0;
        }
        SDL_Delay(100);
    }

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

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


struct node
{
    char songName[1000];
    char artistName[1000];
    char genreName[1000];
    char path[1000];
    struct node *left;
    struct node *right;
};

struct node *createNode(char songName[], char artistName[], char genreName[], char path[])
{
    struct node *n;
    n = (struct node *)malloc(sizeof(struct node));
    strcpy(n->songName, songName);
    strcpy(n->artistName, artistName);
    strcpy(n->genreName, genreName);
    strcpy(n->path, path);
    n->left = NULL;
    n->right = NULL;
    return n;
}

struct node *insertNode(struct node *root, char songName[], char artistName[], char genreName[], char path[])
{
    if (root == NULL)
    {
        root = createNode(songName, artistName, genreName, path);
        return root;
    }
    int result = strcmp(artistName, root->artistName);
    // printf("\n");
    // printf("%d\n", result);
    // printf("artistName: %s  and  root->artistName: %s   and  result: %d\n", artistName, root->artistName, result);
    // printf("%s and %s result is %d: \n", name, root->name, result);
    if (result <= 0)
    {
        root->left = insertNode(root->left, songName, artistName, genreName, path);
    }
    else
    {
        root->right = insertNode(root->right, songName, artistName, genreName, path);
    }
    return root;
}

struct node *insertNodeSong(struct node *root, char songName[], char artistName[], char genreName[], char path[])
{
    if (root == NULL)
    {
        root = createNode(songName, artistName, genreName, path);
        return root;
    }
    int result = strcmp(songName, root->songName);
    // printf("\n");
    // printf("%d\n", result);
    // printf("artistName: %s  and  root->artistName: %s   and  result: %d\n", artistName, root->artistName, result);
    // printf("%s and %s result is %d: \n", name, root->name, result);
    if (result <= 0)
    {
        root->left = insertNode(root->left, songName, artistName, genreName, path);
    }
    else
    {
        root->right = insertNode(root->right, songName, artistName, genreName, path);
    }
    return root;
}

struct node *searchArtist(struct node *root, char artistName[])
{
    if (root == NULL)
    {
        return NULL;
    }
    int result = strcmp(artistName, root->artistName);
    if (result == 0)
    {
        return root;
    }
    else if (result < 0)
    {
        return searchArtist(root->left, artistName);
    }
    else
    {
        return searchArtist(root->right, artistName);
    }
}

struct node *searchSong(struct node *root, char songName[])
{
    if (root == NULL)
    {
        return NULL;
    }
    int result = strcmp(songName, root->songName);
    if (result == 0)
    {
        return root;
    }
    else if (result < 0)
    {
        return searchSong(root->left, songName);
    }
    else
    {
        return searchSong(root->right, songName);
    }
}

void inorder(struct node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%s ", root->songName);
        printf("%s ", root->artistName);
        printf("%s ", root->genreName);
        printf("%s ", root->path);
        printf("\n");
        inorder(root->right);
    }
}

void showArtistSongs(struct node *root, char artistName[])
{
    if (root != NULL)
    {
        showArtistSongs(root->left, artistName);
        int result = strcmp(artistName, root->artistName);
        if (result == 0)
        {
            printf("%s ", root->songName);
            printf("%s ", root->artistName);
            printf("%s ", root->genreName);
            printf("\n");
        }
        showArtistSongs(root->right, artistName);
    }
}

void playSong(struct node *root, char songName[], char *songPath)
{
    // char *songPath[1000];
    if (root != NULL)
    {
        playSong(root->left, songName, songPath);
        int result = strcmp(songName, root->songName);
        // printf("\nStringOne = %s and StringTwo = %s and result = %d\n", songName, root->songName, result);
        // printf("\n%d ", result);
        if (result == 0)
        {
            strcpy(songPath, root->path);
            return;
        }
        playSong(root->right, songName, songPath);
    }
    // return "songPath";
}

int main()
{
    struct node *root;
    root = NULL;
    char* audio_dir = "./audio/";
    DIR *d;
    struct dirent *dir;
    d = opendir(audio_dir);

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
    
            // printf("\nPath: %s\n", audio_filepath);
            // printf("Song: %s\n", dir->d_name);
            // printf("Artist: %s\n", artist);
            // printf("Album: %s\n", ablum);
            // printf("Genre: %s\n\n", genre);

            root = insertNode(root, dir->d_name, artist, genre, audio_filepath);
        

            sf_close(snd_audiofile);
        }
        closedir(d);
    }
    // // root = insertNode(root, "Jeet", "Ritviz", "Electronic", "path");
    // root = insertNode(root, "Kesariya", "Arijit Singh", "Romantic", "Kesariyapath");
    // root = insertNode(root, "Khali Salam Dua", "Himensh rashamiya", "Romantic", "path");
    // root = insertNode(root, "Tere Liye", "Atif Aslam", "Romantic", "path");
    // // root = insertNode(root, "Baaraat", "Ritviz", "Electronic", "path");
    // root = insertNode(root, "Tere Pyar Mein", "Arijit Singh", "Romantic", "path");

    inorder(root);

    printf("\n");
    char songToPlay[1000];
    printf("Enter the song to be played: \n");
    scanf("%s", songToPlay);

    char songPath[1000];
    playSong(root, songToPlay, songPath);
    playsong(songPath);
}