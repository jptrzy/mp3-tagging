#!/bin/python

import sys
from os.path import expanduser
from os import scandir, walk

import music_tag

def retag_mp3(name, path):

    argv = name.split(" - ")
    n = len(argv)


    tag = music_tag.load_file(path)
    #tag.clear()

    # title
    # artist - title
    # artist - album - title
    # artist - album - cur_track|max_track - title
    # artist - album - cur_track - max_track - title

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



def main(argv):

    path = argv.path
    if( path == None ):
        path=expanduser("~")+"/Music"

    for entry in scandir(path):
        name = entry.name
        if(entry.is_dir() == 0 and (name.endswith(".ogg") or name.endswith(".mp3")) ):
            argv.debug and print(name)
            retag_mp3(name[0:-4], path+"/"+name)
        else:
            argv.debug and print("Wtf", path+name)


    
    return(0)
    