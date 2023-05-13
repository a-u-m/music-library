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
    int height;
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
    n->height = 1;
    return n;
}


int max(int a, int b){
    if(a >= b){
        return a;
    }
    else{
        return b;
    }
}

int getHeight(struct node *n){
    if(n == NULL){
        return 0;
    }
    return n->height;
}

int getBalanceFactor(struct node *n){
    if(n == NULL){
        return 0;
    }
    return getHeight(n->left) - getHeight(n->right);
}

struct node *rightRotate(struct node *y){
    struct node *x = y->left;
    struct node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->right), getHeight(y->left)) + 1;
    x->height = max(getHeight(x->right), getHeight(x->left)) + 1;

    return x;
}

struct node *leftRotate(struct node *x) {
    struct node *y = x->right;
    struct node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

struct node *insertNodeArtist(struct node *root, char songName[], char artistName[], char genreName[], char path[])
{
    if (root == NULL)
    {
        root = createNode(songName, artistName, genreName, path);
        return root;
    }

    int result = strcmp(artistName, root->artistName);
    if (result <= 0)
    {
        root->left = insertNodeArtist(root->left, songName, artistName, genreName, path);
    }
    else
    {
        root->right = insertNodeArtist(root->right, songName, artistName, genreName, path);
    }

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int bf = getBalanceFactor(root);

    int result1 = (root->left != NULL) ? strcmp(artistName, root->left->artistName) : -1;
    int result2 = (root->right != NULL) ? strcmp(artistName, root->right->artistName) : 1;

    // Left Left Case
    if (bf > 1 && result1 < 0)
    {
        return rightRotate(root);
    }
    // Right Right Case
    if (bf < -1 && result2 > 0)
    {
        return leftRotate(root);
    }
    // Left Right Case
    if (bf > 1 && result1 > 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // Right Left Case
    if (bf < -1 && result2 < 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
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
    if (result <= 0)
    {
        root->left = insertNodeSong(root->left, songName, artistName, genreName, path);
    }
    else
    {
        root->right = insertNodeSong(root->right, songName, artistName, genreName, path);
    }

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int bf = getBalanceFactor(root);

    int result1 = (root->left != NULL) ? strcmp(songName, root->left->songName) : -1;
    int result2 = (root->right != NULL) ? strcmp(songName, root->right->songName) : 1;

    // Left Left Case
    if (bf > 1 && result1 < 0)
    {
        return rightRotate(root);
    }
    // Right Right Case
    if (bf < -1 && result2 > 0)
    {
        return leftRotate(root);
    }
    // Left Right Case
    if (bf > 1 && result1 > 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // Right Left Case
    if (bf < -1 && result2 < 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

struct node *insertNodeGenre(struct node *root, char songName[], char artistName[], char genreName[], char path[])
{
    if (root == NULL)
    {
        root = createNode(songName, artistName, genreName, path);
        return root;
    }
    int result = strcmp(genreName, root->genreName);
    if (result <= 0)
    {
        root->left = insertNodeSong(root->left, songName, artistName, genreName, path);
    }
    else
    {
        root->right = insertNodeSong(root->right, songName, artistName, genreName, path);
    }

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int bf = getBalanceFactor(root);

    int result1 = (root->left != NULL) ? strcmp(genreName, root->left->genreName) : -1;
    int result2 = (root->right != NULL) ? strcmp(genreName, root->right->genreName) : 1;

    // Left Left Case
    if (bf > 1 && result1 < 0)
    {
        return rightRotate(root);
    }
    // Right Right Case
    if (bf < -1 && result2 > 0)
    {
        return leftRotate(root);
    }
    // Left Right Case
    if (bf > 1 && result1 > 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // Right Left Case
    if (bf < -1 && result2 < 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
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

void showGenreSongs(struct node *root, char genreName[])
{
    if (root != NULL)
    {
        showGenreSongs(root->left, genreName);
        int result = strcmp(genreName, root->genreName);
        if (result == 0)
        {
            printf("%s ", root->songName);
            printf("%s ", root->artistName);
            printf("%s ", root->genreName);
            printf("\n");
        }
        showGenreSongs(root->right, genreName);
    }
}

void playSong(struct node *root, char songName[], char* songPath)
{
    if (root != NULL)
    {
        playSong(root->left, songName, songPath);
        int result = strcmp(songName, root->songName);
        if (result == 0)
        {
            printf("Function %s \n", root->songName);
            printf("\n");
            strcpy(songPath, root->path);
            return;
        }
        playSong(root->right, songName, songPath);
    }
}

void inorderSong(struct node *root){
    if(root != NULL){
        inorderSong(root->left);
        printf("%s ", root->songName);
        inorderSong(root->right);
    }
}

void inorderArtist(struct node *root){
    if(root != NULL){
        inorderArtist(root->left);
        printf("%s ", root->artistName);
        inorderArtist(root->right);
    }
}
void inorderGenre(struct node *root){
    if(root != NULL){
        inorderGenre(root->left);
        printf("%s ", root->genreName);
        inorderGenre(root->right);
    }
}

int main()
{
    struct node *rootSong = NULL;
    struct node *rootArtist = NULL;
    struct node *rootGenre = NULL;

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

            rootSong = insertNodeSong(rootSong, dir->d_name, artist, genre, audio_filepath);
            rootArtist = insertNodeArtist(rootArtist, dir->d_name, artist, genre, audio_filepath);
            rootGenre = insertNodeGenre(rootGenre, dir->d_name, artist, genre, audio_filepath);

            sf_close(snd_audiofile);
        }
        closedir(d);
    }


    printf("***** Artist Tree *****\n");
    inorderArtist(rootArtist);
    printf("\n");

    printf("***** Genre Tree *****\n");
    inorderGenre(rootGenre);
    printf("\n");

    printf("***** Song Tree *****\n");
    inorderSong(rootSong);
    printf("\n");

    printf("\n");
    printf("\n");
    printf("\n");


    printf("Press 1 : to play via song name\nPress 2 : to play via aritst name\nPress 3 : to play via genre name\n");
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    printf("\n");
    if(choice == 1){
        printf("\n");
        char songToPlay[1000];
        printf("Enter the song to be played: \n");
        scanf(" %[^\n]%*c", songToPlay);  // Remove the & operator before songToPlay

        printf("%s\n", songToPlay);

        char songPath[1000];
        playSong(rootArtist, songToPlay, songPath);
        printf("\n Song Path = %s", songPath);
    }
    else if (choice == 2) {
        char artistName[1000];
        printf("Enter artist name: \n");
        scanf(" %[^\n]%*c", artistName);  // Remove the & operator before artistName
        printf("%s \n", artistName);
        printf("\n");
        showArtistSongs(rootArtist, artistName);

        printf("\n");
        char songToPlay[1000];
        printf("Enter the song to be played: \n");
        scanf(" %[^\n]%*c", songToPlay);  // Remove the & operator before songToPlay

        printf("%s\n", songToPlay);

        char songPath[1000];
        playSong(rootArtist, songToPlay, songPath);
        printf("\n Song Path = %s", songPath);
    }
    else if(choice == 3){
        char genreName[1000];
        printf("Enter genre name: \n");
        scanf(" %[^\n]%*c", genreName);  // Remove the & operator before artistName
        printf("%s \n", genreName);
        printf("\n");
        showGenreSongs(rootArtist, genreName);

        printf("\n");
        char songToPlay[1000];
        printf("Enter the song to be played: \n");
        scanf(" %[^\n]%*c", songToPlay);  // Remove the & operator before songToPlay

        printf("%s\n", songToPlay);

        char songPath[1000];
        playSong(rootArtist, songToPlay, songPath);
        printf("\n Song Path = %s", songPath);
    }
    
}