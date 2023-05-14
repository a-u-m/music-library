#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sndfile.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// Music Player DSA CP
//  ................

// bst

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
        root->left = insertNodeGenre(root->left, songName, artistName, genreName, path);
    }
    else
    {
        root->right = insertNodeGenre(root->right, songName, artistName, genreName, path);
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

void showArtistSongs(struct node *root, char artistName[], int* i,char artistSongs[100][100])
{
    if (root != NULL)
    {
        showArtistSongs(root->left, artistName,i,artistSongs);
        int result = strcmp(artistName, root->artistName);
        if (result == 0)
        {
            // printf("%s ", root->songName);
            // printf("%s ", root->artistName);
            // printf("%s ", root->genreName);
            // printf("\n");
            strcpy(artistSongs[(*i)++],root->songName);
        }
        showArtistSongs(root->right, artistName, i, artistSongs);
    }
}

void showGenreSongs(struct node *root, char genreName[],int* i,char genreSongs[100][100])
{
    if (root != NULL)
    {
        showGenreSongs(root->left, genreName,i,genreSongs);
        int result = strcmp(genreName, root->genreName);
        if (result == 0)
        {
            // printf("%s ", root->songName);
            // printf("%s ", root->artistName);
            // printf("%s ", root->genreName);
            // printf("\n");
            strcpy(genreSongs[(*i)++],root->songName);
        }
        showGenreSongs(root->right, genreName,i,genreSongs);
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

int strind = 0;
void inorderSong(struct node *root,char songList[100][100]){
    if(root != NULL){
        inorderSong(root->left,songList);
        strcpy(songList[strind++],root->songName);
        inorderSong(root->right,songList);
    }
}

int artind = 0;
void inorderArtist(struct node *root, char artistList[100][100]){
    if(root != NULL){
        inorderArtist(root->left,artistList);
        strcpy(artistList[artind++],root->artistName);
        inorderArtist(root->right,artistList);
    }
}

int gnrind = 0;
void inorderGenre(struct node *root, char genreList[100][100]){
    if(root != NULL){
        inorderGenre(root->left,genreList);
        strcpy(genreList[gnrind++],root->genreName);
        inorderGenre(root->right,genreList);
    }
}

//

void displaysong(char* song){
    int strl = 39-strlen(song);
    printf("|   %s",song);
    while(strl--){
        printf(" ");
    }
    printf("|    |\n");
}

void displayartist(char* artist){
    int strl = 29-strlen(artist);
    printf("|    |   %s",artist);
    while(strl--){
        printf(" ");
    }
    printf("|  ");
}

void displayGenre(char* genre){
    int strl = 36-strlen(genre);
    printf("   %s",genre);
    while(strl--){
        printf(" ");
    }
    printf("");
}

void displayCurrentSong(char* cs){
    int strl = 75-strlen(cs);
    printf("|    |   %s",cs);
    while(strl--){
        printf(" ");
    }
    printf("|    |\n");
}

void displayMainTitle(char mainTitle[100]){
    int strl = 38-strlen(mainTitle);
    printf("+-- %s ",mainTitle);
    while(strl--){
        printf("-");
    }
    printf("+    |\n");
}

void displayer(char songList[100][100],char artistList[100][100],char genreList[100][100],char currentsong[100],char mainType[100]){
    system("clear");
    printf("\n+------------------------------------- MUSIC PLAYER -------------------------------------+\n");
    printf("|                                                                                        |\n");
    printf("|    +-- Options ---------------------+  ");
    displayMainTitle(mainType);
    printf("|    |                                |  |                                          |    |\n");
    printf("|    |   1  Play [Songname]           |  ");
    displaysong(songList[0]);
    printf("|    |   2  Artist                    |  ");
    displaysong(songList[1]);
    printf("|    |   3  Genre                     |  ");
    displaysong(songList[2]);
    printf("|    |   4  Show Playlists            |  ");
    displaysong(songList[3]);
    printf("|    |   5  Create Playlists          |  ");
    displaysong(songList[4]);
    printf("|    |   6  Top Recommendations       |  ");
    displaysong(songList[5]);
    printf("|    |                                |  ");
    displaysong(songList[6]);
    printf("|    +-- Artists ---------------------+  ");
    displaysong(songList[7]);
    printf("|    |                                |  ");
    displaysong(songList[8]);
    displayartist(artistList[0]);
    displaysong(songList[9]);
    displayartist(artistList[1]);
    displaysong(songList[10]);
    displayartist(artistList[2]);
    displaysong(songList[11]);
    displayartist(artistList[3]);
    displaysong(songList[12]);
    displayartist(artistList[4]);
    displaysong(songList[13]);
    printf("|    |                                |  |                                          |    |\n");
    printf("|    +--------------------------------+  +------------------------------------------+    |\n");
    //printf("|                                                                                        |\n");
    printf("|    +-- Genre ---------------------------------------------------------------------+    |\n");
    printf("|    |                                                                              |    |\n");
    printf("|    |");
    displayGenre(genreList[0]);
    displayGenre(genreList[1]);
    printf("|    |\n");
    printf("|    |");
    displayGenre(genreList[2]);
    displayGenre(genreList[3]);
    printf("|    |\n");
    printf("|    |                                                                              |    |\n");
    printf("|    +------------------------------------------------------------------------------+    |\n");
    printf("|                                                                                        |\n");
    printf("|    +-- Currently Playing ---------------------------------------------------------+    |\n");
    printf("|    |                                                                              |    |\n");
    displayCurrentSong(currentsong);
    printf("|    |                                                                              |    |\n");
    printf("|    +------------------------------------------------------------------------------+    |\n");
    printf("|                                                                                        |\n");
    printf("+----------------------------------------------------------------------------------------+\n");

}

// Playlist

struct playlist{
    char playlistName[100];
    char songName[100];
    char artistName[100];
    char genreName[100];
    char path[100];
    struct playlist* prev;
    struct playlist* next;
};

struct playlist* insertPlaylistNode(struct playlist* head, const char* playlistName, const char* songName, const char* artistName, const char* genreName, const char* path) {
    struct playlist* newNode = (struct playlist*)malloc(sizeof(struct playlist));
    if (newNode == NULL) {
        printf("Memory allocation failed.");
        return NULL;
    }

    strcpy(newNode->playlistName, playlistName);
    strcpy(newNode->songName, songName);
    strcpy(newNode->artistName, artistName);
    strcpy(newNode->genreName, genreName);
    strcpy(newNode->path, path);

    newNode->prev = NULL;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } 
    else {
        struct playlist* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
    return head;
}

void addPlaylistToCSV(struct playlist* playlist, FILE* file) {
    fprintf(file, "%s,%s\n", playlist->playlistName, playlist->songName);
}

struct playlist* searchPlaylist(struct playlist** playlists, char givenPlaylist[100]){
    int i = 0;
    while(i < 100 && playlists[i] != NULL){
        if(!strcmp(playlists[i]->playlistName,givenPlaylist))return playlists[i];
        i++;
    }
    return NULL;
}

void playlistSongs(struct playlist* p, char playlistSongs[100][100]){
    int i = 0;
    while(p != NULL){
        strcpy(playlistSongs[i++],p->songName);
        p = p->next;
    }
}

void initiatePlaylists(struct playlist* p[100],struct node* songroot) {
    int i = 0,j = 0;
    FILE* file = fopen("playlist.csv", "r");
    if (file == NULL) {
        printf("Failed to open the CSV file.\n");
        return;
    }

    char line[100];
    while (fgets(line, 100, file) != NULL) {
        char* token = strtok(line, ",");
        char values[6][100];
        while (token != NULL) {
            //printf("%s\n", token);
            strcpy(values[j++],token);
            token = strtok(NULL, ",");
        }
        j=0;
        if(!strcmp(values[0],"-")){
            i++;
            continue;
        }
        struct node* temp = searchSong(songroot,values[1]);
        p[i] = insertPlaylistNode(p[i],values[0],values[1],temp->artistName,temp->genreName,temp->path);
    }

    fclose(file);
}

// --------


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

int main()
{
    struct node *rootSong = NULL;
    struct node *rootArtist = NULL;
    struct node *rootGenre = NULL;
    struct playlist* playlists[100] = {NULL};

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

    initiatePlaylists(playlists,rootSong);

    char songToPlay[100] = {""};

    char songList[100][100];
    for(int i = 0;i < 100;i++)strcpy(songList[i]," ");
    inorderSong(rootSong,songList);

    char artistList[100][100];
    for(int i = 0;i < 100;i++)strcpy(artistList[i]," ");
    inorderArtist(rootArtist,artistList);

    char genreList[100][100];
    for(int i = 0;i < 100;i++)strcpy(genreList[i]," ");
    inorderGenre(rootGenre,genreList);

    while(1){
        // printf("***** Artist Tree *****\n");
        // inorderArtist(rootArtist);
        // printf("\n");

        // printf("***** Genre Tree *****\n");
        // inorderGenre(rootGenre);
        // printf("\n");

        // printf("***** Song Tree *****\n");
        // inorderSong(rootSong);
        // printf("\n");

        // printf("\n");
        // printf("\n");
        // printf("\n");
        system("clear");
        if(!strcmp(songToPlay,""))displayer(songList,artistList,genreList,"NO SONG PLAYING!","Songs");
        else displayer(songList,artistList,genreList,songToPlay,"Songs");

        //printf("Press 1 : to play via song name\nPress 2 : to play via aritst name\nPress 3 : to play via genre name\n");
        int choice;
        printf("\nSelect Option: ");
        scanf("%d", &choice);
        printf("\n");
        if(choice == 1){
            printf("Enter the song to be played: ");
            scanf(" %[^\n]%*c", songToPlay);  // Remove the & operator before songToPlay

            printf("%s\n", songToPlay);
            char songPath[1000] = {""};
            playSong(rootArtist, songToPlay, songPath);
            if(!strcmp(songPath,"")){
                strcpy(songToPlay,"");
                displayer(songList,artistList,genreList,"NO SONG PLAYING!","Songs");
                printf("Song Not Available!");
            }
            else{
                displayer(songList,artistList,genreList,songToPlay,"Songs");
                playsong(songPath);
                strcpy(songToPlay,"");
            }
        }
        else if (choice == 2) {
            char artistName[100], title[100] = {""};

            printf("Enter artist name: ");
            scanf(" %[^\n]%*c", artistName);  // Remove the & operator before artistName

            strcat(title,artistName);
            strcat(title,"'s");
            strcat(title," Songs");

            char givenArtistSongs[100][100];
            for(int i = 0;i < 100;i++)strcpy(givenArtistSongs[i]," ");
            int i = 0;
            showArtistSongs(rootArtist, artistName,&i,givenArtistSongs);
            system("clear");
            displayer(givenArtistSongs,artistList,genreList,"NO SONG PLAYING!",title);

            printf("Enter the song to be played: ");
            scanf(" %[^\n]%*c", songToPlay);  // Remove the & operator before songToPlay

            printf("%s\n", songToPlay);

            char songPath[1000] = {""};
            playSong(rootArtist, songToPlay, songPath);
            if(!strcmp(songPath,"")){
                strcpy(songToPlay,"");
                displayer(givenArtistSongs,artistList,genreList,"NO SONG PLAYING!",title);
                printf("Song Not Available!");
            }
            else{
                displayer(givenArtistSongs,artistList,genreList,songToPlay,title);
                playsong(songPath);
                strcpy(songToPlay,"");
            }
            //printf("\n Song Path = %s", songPath);
        }
        else if(choice == 3){
            char genreName[100], title[100] = {""};

            printf("Enter Genre: ");
            scanf(" %[^\n]%*c", genreName);  // Remove the & operator before artistName

            strcat(title,genreName);
            strcat(title," Songs");

            char givenGenreSongs[100][100];
            for(int i = 0;i < 100;i++)strcpy(givenGenreSongs[i]," ");
            int i = 0;
            showGenreSongs(rootGenre, genreName,&i,givenGenreSongs);
            system("clear");
            displayer(givenGenreSongs,artistList,genreList,"NO SONG PLAYING!",title);

            printf("Enter the song to be played: ");
            scanf(" %[^\n]%*c", songToPlay);  // Remove the & operator before songToPlay

            printf("%s\n", songToPlay);

            char songPath[1000] = {""};
            playSong(rootGenre, songToPlay, songPath);
            if(!strcmp(songPath,"")){
                strcpy(songToPlay,"");
                displayer(givenGenreSongs,artistList,genreList,"NO SONG PLAYING!",title);
                printf("Song Not Available!");
            }
            else{
                displayer(givenGenreSongs,artistList,genreList,songToPlay,title);
                playsong(songPath);
                strcpy(songToPlay,"");
            }
        }
        else if(choice == 4){
            char playlistNames[100][100],selectedPlaylistSongs[100][100], a;
            struct playlist* selectedPlaylist = NULL;
            for(int i = 0;i < 100;i++){
                if(playlists[i] == NULL)strcpy(playlistNames[i]," ");
                else strcpy(playlistNames[i],playlists[i]->playlistName);
                strcpy(selectedPlaylistSongs[i]," ");
            }
            system("clear");
            displayer(playlistNames,artistList,genreList,"NO SONG PLAYING!","Your Playlists");
            
            char playlistToPlay[100];
            printf("Select Playlist: ");
            scanf(" %[^\n]%*c", playlistToPlay);  // Remove the & operator before songToPlay
            if(!strcmp(playlistToPlay,"-1")){
                printf("End\n");
            }
            else{
                selectedPlaylist = searchPlaylist(playlists,playlistToPlay);
                if(selectedPlaylist == NULL){
                    printf("NO SUCH PLAYLIST!\n");
                }
                else {
                    playlistSongs(selectedPlaylist,selectedPlaylistSongs);
                    displayer(selectedPlaylistSongs,artistList,genreList,"NO SONG PLAYING!",selectedPlaylist->playlistName);
                    char pause[100];
                    printf("Write Any Character to Break the Pause: ");
                    scanf("%s",pause);
                }
            }
        }
        else if(choice == 5){
            int i = 0;
            char playlistName[100],temp[100];
            struct node* songTemp;
            while(playlists[i] != NULL)i++;
            printf("Enter Name of Playlist: ");
            scanf(" %[^\n]%*c", playlistName);
            
            printf("---- Select Songs For %s ----\n",playlistName);

            while(1){
                printf("Song Name: ");
                scanf(" %[^\n]%*c", temp);
                if(!strcmp(temp,"-1")){
                    FILE* csvFile = fopen("playlist.csv", "a");
                    if (csvFile == NULL) {
                        printf("Failed to open the CSV file.");
                        return 1;
                    }
                    struct playlist* temp = playlists[i];
                    while(temp != NULL){    
                        addPlaylistToCSV(temp, csvFile);
                        temp = temp->next;
                    }
                    fprintf(csvFile, "%s,%s\n", "-", "-");
                    fclose(csvFile);
                    break;
                }
                songTemp = searchSong(rootSong,temp);
                if(songTemp == NULL)printf("Song Not Available!\n");
                else playlists[i] = insertPlaylistNode(playlists[i],playlistName,songTemp->songName,songTemp->artistName,songTemp->genreName,songTemp->path);
            }
        }

    }
}