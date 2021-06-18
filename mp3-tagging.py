#!/bin/python


import mp3_tagging, sys
import argparse


if( __name__ == "__main__" ): 
    
    parser = argparse.ArgumentParser()
    parser.add_argument(
                        "-p", "--path", 
                        )
    parser.add_argument(
                        "-d", "--debug", 
                        action="store_true"
                        )
    args = parser.parse_args()

    sys.exit(mp3_tagging.main(args))
