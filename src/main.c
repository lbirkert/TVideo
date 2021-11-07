#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include <png.h>

#include "os.h"
#include "util.h"

const char* cacheDirectory = ".cache/";

char* str_join(const char* s1, const char* s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}

void fatal_error (const char * message, ...) {
    va_list args;
    va_start (args, message);
    vfprintf (stderr, message, args);
    va_end (args);
    exit (EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    #if defined(_WIN32)
        system("chcp 65001");
    #endif
    system("");

    // * DETECT FFMPEG
    printf("\n\e[38;2;255;100;0mLooking for ffmpeg...\e[0m\n\n");
    if(system("ffmpeg -version") != 32512) {
        printf("\n\e[38;2;0;255;30mFFMPEG found!\e[0m\n\n");
        if(argc == 1) {
        } else if(argc == 2) {
            char* filename = argv[1];
            if( access( filename, F_OK ) == 0 ) {
                system("rm .cache/out*.png");
                system("mkdir .cache");

                int filenameLength = strlen(filename);
                char* cmd = (char*)malloc(sizeof(char)*(39+strlen(filename)));
                memcpy(cmd, "ffmpeg -i ", 10);
                memcpy(cmd+10, filename, filenameLength);
                memcpy(cmd+10+filenameLength, " -vf fps=10 .cache/out%d.png", 28);

                system(cmd);

                int frame = 1;
                
                char* framename = malloc(sizeof(char)*15);
                char* framestr = int_to_str(frame);
                int framestrlen = strlen(framestr);
                memcpy(framename, ".cache/out", 10);
                memcpy(framename+10, framestr, framestrlen);
                memcpy(framename+10+framestrlen, ".png", 5);
                free(framestr);
                while( access( framename, F_OK ) == 0 ) {
                    printf("\e[2J\n");
                    png_structp	png_ptr;
                    png_infop info_ptr;
                    FILE * fp;
                    png_uint_32 width;
                    png_uint_32 height;
                    int bit_depth;
                    int color_type;
                    int interlace_method;
                    int compression_method;
                    int filter_method;
                    int j;
                    png_bytepp rows;
                    fp = fopen (framename, "rb");
                    if (! fp) {
                        fatal_error ("Cannot open '%s': %s\n", framename, strerror (errno));
                    }
                    png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
                    if (! png_ptr) {
                        fatal_error ("Cannot create PNG read structure");
                    }
                    info_ptr = png_create_info_struct (png_ptr);
                    if (! png_ptr) {
                        fatal_error ("Cannot create PNG info structure");
                    }
                    png_init_io (png_ptr, fp);
                    png_read_png (png_ptr, info_ptr, 0, 0);
                    png_get_IHDR (png_ptr, info_ptr, & width, & height, & bit_depth,
                        & color_type, & interlace_method, & compression_method,
                        & filter_method);
                    rows = png_get_rows (png_ptr, info_ptr);
                    int rowbytes;
                    rowbytes = png_get_rowbytes (png_ptr, info_ptr);
                    for (j = 0; j < height; j+=10) {
                        int i;
                        png_bytep row;
                        png_bytep row2;
                        row = rows[j+5];
                        row2 = rows[j];
                        for (i = 0; i < rowbytes; i+=15) {
                            printf("\e[48;2;%d;%d;%dm\e[38;2;%d;%d;%dm▀\e[0m", row[i], row[i+1], row[i+2], row2[i], row2[i+1], row2[i+2]);
                        }
                        printf ("\n");
                    }

                    usleep(100000);

                    frame++;

                    framestr = int_to_str(frame);
                    framestrlen = strlen(framestr);
                    memcpy(framename, ".cache/out", 10);
                    memcpy(framename+10, framestr, framestrlen);
                    memcpy(framename+10+framestrlen, ".png", 5);
                    free(framestr);
                }

                free(framename);

                system("rm .cache/out*.png");
            } else printf("\e[38;2;255;0;0mERROR: File '\e[38;2;255;100;0m%s\e[38;2;255;0;0m' not found! \e[0m\n\n", filename);
        }
    } else printf("\e[38;2;255;0;0mERROR: FFMPEG not found. Install it from here: https://ffmpeg.org/ \e[0m\n\n");

    return 0;
}