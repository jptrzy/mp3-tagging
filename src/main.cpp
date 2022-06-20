/*
This program automaticly tags music (only ogg) files basing on their names.

# Format
1 <TITLE>.<EXT>
2 <ARTIST>-<TITLE>.<EXT>
3 <ARTIST>-<ALBUM>-<TITLE>.<EXT>
4 <ARTIST>-<ALBUM>-<TRACK>-<TITLE>.<EXT>
5 <ARTIST>-<ALBUM>-<TRACK>-<TRACKTOTAL>-<TITLE>.<EXT>

# Links:
https://wiki.xiph.org/Field_names
*/
#include <stdio.h>
#include <cstring>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>


#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/oggfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/vorbisfile.h>

#define LEN(S) sizeof(S)/sizeof(*S)

std::string MUSIC_PATH;

const wchar_t* cToWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}


void tagOgg(std::string path, std::string name){

    // printf("[D] %s %s\n", path, name);

    char* array[6] = {};
    int lenght = 0;

    { // Need to find diffren method to split and put parts into arrays
        int pos = 0;
        std::string cut_name = name;
        // std::cout << name << std::endl;
        while ((pos = cut_name.find('-')) != std::string::npos) {
            std::string token = cut_name.substr(0, pos);
            array[lenght] = (char *) malloc(token.length());
            token.copy(array[lenght++], token.length());
            cut_name.erase(0, pos + 1);
        }
        array[lenght] = (char *) malloc(cut_name.length());
        cut_name.copy(array[lenght++], cut_name.length());

        // for(int i=0;i<lenght;i++){
        //     std::cout << array[i] << "\n";
        // }
    }

    std::string full_path = path + "/" + name + ".ogg";

    TagLib::Ogg::Vorbis::File vorbis_file(full_path.c_str());
    TagLib::Ogg::XiphComment* vorbis_tag = vorbis_file.tag();
    vorbis_tag->removeAllFields();
    
    switch(lenght){
        case 1 :
            vorbis_tag->setTitle( cToWC(array[0]));
        break; case 2 :
            vorbis_tag->setArtist( cToWC(array[0]));
            vorbis_tag->setTitle( cToWC(array[1]));
        break; case 3 :
            vorbis_tag->setArtist( cToWC(array[0]));
            vorbis_tag->setAlbum( cToWC(array[1]));
            vorbis_tag->setTitle( cToWC(array[2]));
        break; case 4 :
            vorbis_tag->setArtist( cToWC(array[0]));
            vorbis_tag->setAlbum( cToWC(array[1]));
            vorbis_tag->addField("TRACKNUMBER",  cToWC(array[2]), true);
            vorbis_tag->setTitle( cToWC(array[3]));
        break; case 5 :
            vorbis_tag->setArtist( cToWC(array[0]));
            vorbis_tag->setAlbum( cToWC(array[1]));
            vorbis_tag->addField("TRACKNUMBER",  cToWC(array[2]), true);
            vorbis_tag->addField("TRACKTOTAL",  cToWC(array[3]), true);
            vorbis_tag->setTitle( cToWC(array[4]));
        break; default:
            printf("[E] \"%s\" Default switch, this shouldn't heppend.\n", name);
        break;
    }

    vorbis_file.save();
}


int main(int argc, char *argv[]){
    setlocale( LC_ALL, "" );

    MUSIC_PATH = getenv("HOME");
    MUSIC_PATH += "/Music";

    DIR* music_dir = opendir(MUSIC_PATH.c_str());
    if(!music_dir){
        printf("[E] Can't find music dir \"%s\".\n", MUSIC_PATH);
        return 1;
    }
    // chdir(music_dir);

    struct dirent *dirp;
    while((dirp=readdir(music_dir))!=NULL){
        if (dirp->d_type==4){ //FOLDER
            // if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0){
            //     continue;
            // }
            // printf("%s %s\n", "FOLDER", dirp->d_name);
        } else if (strcmp(strrchr(dirp->d_name,'.'), ".ogg") == 0) { //FILE
            size_t strLen = strlen(dirp->d_name);
            dirp->d_name[4 <= strLen ? strLen-4 : 0] = '\0';
            tagOgg(MUSIC_PATH, dirp->d_name);
        }
    }

    closedir(music_dir);

    return 0;
}