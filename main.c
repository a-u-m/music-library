#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sndfile.h>
#include <SDL2/SDL.h>

int main(){
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

            printf("\nPath: %s\n", audio_filepath);
            printf("Song: %s\n", dir->d_name);
            printf("Artist: %s\n", artist);
            printf("Album: %s\n", ablum);
            printf("Genre: %s\n\n", genre);

            sf_close(snd_audiofile);
        }
        closedir(d);
    }
}
