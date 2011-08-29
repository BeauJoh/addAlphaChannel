//
//  main.cpp
//  addAlphaChannel
//
//  Created by Beau Johnston on 29/08/11.
//  Copyright 2011 University Of New England. All rights reserved.
//


/*
    To run use following arguments to specify stacks:
    -i $(PROJECT_DIR)/dataResources/Embryo-Stage23-Diff-PNG/.png
    -o $(PROJECT_DIR)/dataResources/output/out.png
*/
#include "FileHandler.h"
#include "RGBAUtilities.h"

#include <getopt.h>
#include <string>
#include <iostream>

using namespace std;

// getopt argument parser variables
string imageFileName;
string outputImageFileName;

static inline int parseCommandLine(int argc , char** argv){
    {
        int c;
        while (true)
        {
            static struct option long_options[] =
            {
                /* These options don't set a flag.
                 We distinguish them by their indices. */
                {"image",  required_argument,       0, 'i'},
                {"output-image", required_argument, 0, 'o'},
                {0, 0, 0, 0}
            };
            /* getopt_long stores the option index here. */
            int option_index = 0;
            
            c = getopt_long (argc, argv, ":i:o:",
                             long_options, &option_index);
            
            /* Detect the end of the options. */
            if (c == -1)
                break;
            
            switch (c)
            {
                case 0:
                    /* If this option set a flag, do nothing else now. */
                    if (long_options[option_index].flag != 0)
                        break;
                    printf ("option %s", long_options[option_index].name);
                    if (optarg)
                        printf (" with arg %s", optarg);
                    printf ("\n");
                    break;
                    
                case 'i':
                    imageFileName = optarg ;
                    break;
                    
                case 'o':
                    outputImageFileName = optarg;
                    break;
                    
                    
                case '?':
                    /* getopt_long already printed an error message. */
                    break;
                    
                default:
                    ;
                    
            }
        }
        
        
        /* Print any remaining command line arguments (not options). */
        if (optind < argc)
        {
            while (optind < argc)
            /*
             printf ("%s ", argv[optind]);
             putchar ('\n');
             */
                optind++;
        }
    }
    return 1;
};

//#define TESTPRINT

int main (int argc, char ** argv)
{
    parseCommandLine(argc, argv);
    
    generateListOfAssociatedFiles((char*)imageFileName.c_str());
    
    int width, height;

    //sortFilesNumerically();
    
    //printFiles();

    // traverse all images
    for (int i = 0; i < numberOfFiles(); i++) {
        //load image into buffer
        read_png_file(getNextFileName());
        width = getImageWidth();
        height = getImageLength();
        
        uint8 *image = new uint8[getImageSize()];
        
        image = getImage();
        
        clearImageBuffer();
        //create new image to store and
        //set it with 4 channels

        uint8 *newImage = convertToFourChannel(image);

        #ifdef TESTPRINT
            if (i==20) {
                cout << "old grayscaleImage Looks like:" << endl;
                printImage(image, getImageSize()/32);
                cout << "\n\n\n\n\n\nnew rgba image Looks like:" << endl;
                printImage(newImage, getImageSize()/8);
            }
        #endif
        
        //write out this image
        string file = outputImageFileName.substr(outputImageFileName.find_last_of('/')+1);
        string path = outputImageFileName.substr(0, outputImageFileName.find_last_of('/')+1);
        
        string cutDownFile = file.substr(0, file.find_last_of('.'));
        string extension = file.substr(file.find_last_of('.'));
        
        
        string newName = cutDownFile;
        char numericalRepresentation[200];
        sprintf(numericalRepresentation, "%d", i+1);
        newName.append(numericalRepresentation);
        newName.append(extension);
        
        newName = path.append(newName);
        
        setImage(newImage);
        write_png_file((char*)newName.c_str());
        
        
    } 
    
    return 0;
}

