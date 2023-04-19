#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sndfile.h>
#include <stdio.h>
#include <SDL2/SDL.h>

void playsong(const char* filename) {
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);

    //SDL_WINDOWPOS_UNDEFINED

    SDL_Window* window = SDL_CreateWindow("music-lib", 0, 0, 300, 30, 0);
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
        playsong(root->path);
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
    while(true){
        char song[100];
        printf("\nenter the song you want to search:");
        scanf("%s", song);
        int check = SongSearch(STrie.root, song, 0, len(song));
        if (!check){
            printf("song not found");
        }
    }
}
