/*
 * Parker Riebs
 * 4/26/2016
 *
 * Shows the amount of page faults that will occur in memory managment.
 * The given file is the data read/written to memory, and output
 * is shown for First-In-First-Out (FIFO), Optimal (Opt.),
 * Least-Recently-Used (LRU), Second-Chance LRU (SC),
 * Enhanced Second-Chance LRU (ESC) algorithms that handle pages in memory.
 * How many frames can also be specified for the algorthims, the default is
 * to show all frames (1-7).
 *
 * CS 441/541: Page Fault Algorithm Simulator (Project 5)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/******************************
 * Defines
 ******************************/


/******************************
 * Structures
 ******************************/
// Struct to hold page data from files.
struct page {
    int data;
    char wr;
};
typedef struct page page;

// Struct to hold frame information for different algorithms.
struct frame_info {
    int num_frame;
    int opt;
    int fifo;
    int lru;
    int sc;
    int esc;
};
typedef struct frame_info frame_info;

// Special struct for enhanced second chance algorithm to hold pages in frames
// with extra reference bit and dirty bit.
struct esc {
    int page_num;
    int ref_bit;
    int dirty_bit;
};
typedef struct esc esc;

/******************************
 * Global Variables
 ******************************/


/******************************
 * Function declarations
 ******************************/

 /*
 *  Runs all algorithms for the set amount of frames, to update data in frame
 *  information struct.
 *
 *  Parameters:
 *   pages: The page information given from files.
 *   num_pages: The number of pages.
 *   frame: The frame information struct to add algorithm data to.
 */
void calculate_page_faults(page * pages, int num_pages, frame_info * frame);

/*
*  Runs first in first out algorithm for the set amount of frames, to update 
*  data in frame information struct.
*
*  Parameters:
*   pages: The page information given from files.
*   num_pages: The number of pages.
*   frame_info: The frame information struct to add algorithm data to.
*/
void first_in_first_out(page * pages, int num_pages, frame_info * frame_info);

/*
*  Runs least recently used algorithm for the set amount of frames, to update
*  data in frame information struct.
*
*  Parameters:
*   pages: The page information given from files.
*   num_pages: The number of pages.
*   frame_info: The frame information struct to add algorithm data to.
*/
void least_recently_used(page * pages, int num_pages, frame_info * frame_info);

/*
*  Runs optimal algorithm for the set amount of frames, to update
*  data in frame information struct.
*
*  Parameters:
*   pages: The page information given from files.
*   num_pages: The number of pages.
*   frame_info: The frame information struct to add algorithm data to.
*/
void optimal(page * pages, int num_pages, frame_info * frame_info);

/*
*  Runs second chance least recently used algorithm for the set amount of 
*  frames, to update data in frame information struct.
*
*  Parameters:
*   pages: The page information given from files.
*   num_pages: The number of pages.
*   frame_info: The frame information struct to add algorithm data to.
*/
void second_chance_lru(page * pages, int num_pages, frame_info * frame_info);

/*
*  Runs enhanced second chance algorithm for the set amount of
*  frames, to update data in frame information struct.
*
*  Parameters:
*   pages: The page information given from files.
*   num_pages: The number of pages.
*   frame_info: The frame information struct to add algorithm data to.
*/
void enhanced_sc(page * pages, int num_pages, frame_info * frame_info);

/*
*  Prints page infromation to the user.
*
*  Parameters:
*   pages: The page information given from files.
*   num_pages: The number of pages.
*/
void print_page_info(page * pages, int num_pages);

/*
*  Prints frame algorithm data to the user.
*
*  Parameters:
*   frames: The frame information struct that data was added to.
*   num_frames: The number of frames.
*/
void print_frame_info(frame_info * frames, int num_frames);