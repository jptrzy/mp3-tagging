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
#include <string>
#include <codecvt>
#include <locale>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/oggfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/vorbisfile.h>

#define LEN(S) sizeof(S)/sizeof(*S)


// OPTIONS
bool DEBUG = false;
std::string MUSIC_PATH = getenv("HOME") + std::string("/Music");



std::wstring sTW(std::string s){
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(s);
}

void tagOgg(std::string path, std::string name){

    if(DEBUG){
        printf("[D] `%s` `%s`\n", path.c_str(), name.c_str());
    } 

    std::wstring array[6] = {};
    int lenght = 0;

    {
        int pos = 0;
        std::wstring token, cut_name = sTW(name);
        // std::cout << name << std::endl;
        while ((pos = cut_name.find('-')) != std::wstring::npos) {
            if(lenght > 4){
                printf("[E] `%s` have more segments then is allowed.\n", path.c_str(), name.c_str());
                return;
            }
            token = cut_name.substr(0, pos);
            array[lenght++] = token;
            cut_name.erase(0, pos + 1);
        }
        array[lenght++] = cut_name;;
    }


    std::string full_path = path + "/" + name + ".ogg";

    TagLib::Ogg::Vorbis::File vorbis_file(full_path.c_str());
    TagLib::Ogg::XiphComment* vorbis_tag = vorbis_file.tag();
    vorbis_tag->removeAllFields();
    
    switch(lenght){
        case 1 :
            vorbis_tag->setTitle( array[0].c_str() );
        break; case 2 :
            vorbis_tag->setArtist( array[0].c_str() );
            vorbis_tag->setTitle( array[1].c_str() );
        break; case 3 :
            vorbis_tag->setArtist( array[0].c_str() );
            vorbis_tag->setAlbum( array[1].c_str() );
            vorbis_tag->setTitle( array[2].c_str() );
        break; case 4 :
            vorbis_tag->setArtist( array[0].c_str() );
            vorbis_tag->setAlbum( array[1].c_str() );
            vorbis_tag->addField("TRACKNUMBER",  array[2].c_str() , true);
            vorbis_tag->setTitle( array[3].c_str() );
        break; case 5 :
            vorbis_tag->setArtist( array[0].c_str() );
            vorbis_tag->setAlbum( array[1].c_str() );
            vorbis_tag->addField("TRACKNUMBER",  array[2].c_str() , true);
            vorbis_tag->addField("TRACKTOTAL",  array[3].c_str() , true);
            vorbis_tag->setTitle( array[4].c_str() );
        break; default:
            printf("[E] \"%s\" Default switch, this shouldn't heppend.\n", name);
        break;
    }

    vorbis_file.save();
}

int main(int argc, char *argv[]){
    setlocale( LC_ALL, "" );



    { /* Interpretate run options */
        if(DEBUG){
            printf("[D] Start reading run options\n");
        }

        for(int i=0; i < argc; i++){
            if ( strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help")==0 ) {
                printf(R"(Usage: ogg-tagging [OPTION...]

-h, --help      prints this help message
-p [PATH]       set music folder path to PATH
-d, --debug     emit additional debugging messages

)");
                return 0;
            }else if (strcmp(argv[i], "-d")==0 || strcmp(argv[i], "---debug")==0) {
                DEBUG = true;
            }else if (strcmp(argv[i], "-p")==0) {
                if (++i < argc) {
                    printf("%s\n", argv[i]);
                    MUSIC_PATH=argv[i];
                } else {
                    printf("[W] You forget about [PATH] after -p flag.\n");
                }
            }
        }

        if(DEBUG){
            printf("[D] End reading run options\n");
        }
    }


    DIR* music_dir = opendir(MUSIC_PATH.c_str());
    if(!music_dir){
        printf("[E] Can't find music dir `%s`.\n", MUSIC_PATH.c_str());
        return 1;
    }

    struct dirent *dirp;
    while((dirp=readdir(music_dir))!=NULL){

        // if (dirp->d_type==4){ //FOLDER
        //     // if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0){
        //     //     continue;
        //     // }
        //     // printf("%s %s\n", "FOLDER", dirp->d_name);
        // }  
        if (strrchr(dirp->d_name,'.') && strcmp(strrchr(dirp->d_name,'.'), ".ogg") == 0) { //FILE
            size_t strLen = strlen(dirp->d_name);
            dirp->d_name[4 <= strLen ? strLen-4 : 0] = '\0';
            tagOgg(MUSIC_PATH, dirp->d_name);
        }
    }

    closedir(music_dir);

    return 0;
}