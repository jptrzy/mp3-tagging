#!/bin/python

import sys
from os.path import expanduser
from os import scandir, walk

#from mutagen.easyid3 import EasyID3
#from mutagen.id3 import ID3,TRCK,TIT2,TALB,TPE1,APIC,TDRC,COMM,TPOS,USLT



#import stagger
#import stagger.id3

import music_tag

def retag_mp3(name, path):

    argv = name.split(" - ")
    n = len(argv)


    tag = music_tag.load_file(path)
    #tag.clear()

    if(n == 1):
        tag["title"] = argv[0]
    elif(n == 2):
        tag["artist"] = argv[0]
        tag["title"] = argv[1]
    elif(n == 3):
        tag["artist"] = argv[0]
        tag["album"] = argv[1]
        tag["title"] = argv[2]
    elif(n == 4):
        tag["artist"] = argv[0]
        tag["album"] = argv[1]
        trackNum = argv[2].split("|")
        tag["tracknumber"] = trackNum[0]
        tag["totaltracks"] = trackNum[1]
        tag["title"] = argv[3]
    else:
        print("\033[5;41m[E]\033[0m\033[31m Mp3 name have {}(n) \" - \" !!!\033[0m".format(n))

    tag.save()

# def retag_mp3_by_id3(name, path):
#     #print(name)
#     argv = name.split(" - ")
#     n = len(argv)
#     id3 = ID3()
#     if(n == 1):
#         id3.add(TIT2(encoding=3, text= argv[0] ))
#     elif(n == 2):
#         id3.add(TPE1(encoding=3, text= argv[0] ))
#         id3.add(TIT2(encoding=3, text= argv[1] ))
#     elif(n == 3):
#         id3.add(TPE1(encoding=3, text= argv[0] ))
#         id3.add(TALB(encoding=3, text= argv[1] ))
#         id3.add(TIT2(encoding=3, text= argv[2] ))
#     elif(n == 4):
#         id3.add(TPE1(encoding=3, text= argv[0] ))
#         id3.add(TALB(encoding=3, text= argv[1] ))
#         id3.add(TRCK(encoding=3, text= argv[2].replace("|", "/") ))
#         id3.add(TIT2(encoding=3, text= argv[3] ))
#     else:
#         print("\033[5;41m[E]\033[0m\033[31m Mp3 name have {}(n) \" - \" !!!\033[0m".format(n))
#     id3.save(path)



# def retag_mp3_by_easyid3(name, path):
#     audio = EasyID3(path)
#     print(name)
#     #s = int(MP3(path).info.length)
#     #m = int(s/60)
#     #s = s%60
#     #print("{:>2}:{:<2}".format(m, s))
#     #print( audio["tracknumber"] )
#     argv = name.split(" - ")
#     n = len(argv)  
#     if(n == 1):
#         audio["title"] = argv[0]
#     elif(n == 2):
#         audio["artist"] = argv[0]
#         audio["title"]  = argv[1]
#     elif(n == 3):
#         audio["artist"] = argv[0]
#         audio["album"]  = argv[1]
#         audio["title"]  = argv[2]
#     elif(n == 4):
#         audio["artist"]         = argv[0]
#         audio["album"]          = argv[1]
#         audio["tracknumber"]    = argv[2].replace("|", "/")
#         audio["title"]          = argv[3]
#     else:
#         print("\033[5;41m[E]\033[0m\033[31m Mp3 name have {}(n) \" - \" !!!\033[0m".format(n))
#     audio.save()



def main(argv):

    path = argv.path
    if( path == None ):
        path=expanduser("~")+"/Music"

    #TEST
    #path="example"
    for entry in scandir(path):
        name = entry.name
        if(entry.is_dir() == 0 and name.endswith(".ogg") ):
            argv.debug and print(name)
            retag_mp3(name[0:-4], path+"/"+name)
        else:
            argv.debug and print("Wtf", path+name)


    
    return(0)
    