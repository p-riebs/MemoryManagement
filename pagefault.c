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
#include "pagefault.h"


int main(int argc, char **argv) 
{
    FILE * file = NULL;
    char line[256];
    int i;
    char * filename = NULL;
    char correct_letter = NULL;
    
    page * pages;
    int num_pages;
    // Keep track of frame information for different algorithms.
    frame_info * frames;
    int num_frames = 7;

    // Used if a user specifies a frame to be used.
    int select_frame = -1;

    // Process out of order parameters
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0 && argc > i + 1
            && atoi(argv[i + 1]) > 0 && atoi(argv[i + 1]) < 8)
        {
            select_frame = atoi(argv[i + 1]);
            num_frames = 1;
            i++;
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            printf("Error: Must supply an integer argument between 1 and 7 for -f option\n");
            exit(0);
        }
        else
        {
            filename = argv[i];
        }
    }

    // Argument error check.
    if (filename == NULL)
    {
        printf("Error: All the arguments were not specified.\n");
        exit(0);
    }

    if (select_frame == -1)
    {
        printf("Num. Frames : All\n");
    }
    else
    {
        printf("Ref. File   : %s\n", filename);
    }

    // Open the file to be read.
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file %s\n", filename);
        exit(0);
    }

    fgets(line, 256, file);
    // Find if the number of the pages from file.
    num_pages = atoi(line);
    fgets(line, 256, file);

    // Create struct array for each line in file.
    pages = malloc(num_pages * sizeof(page));
    for (i = 0; i < num_pages; i++)
    {
        // Add file data to pages array.
        pages[i].data = atoi(strtok(strdup(line), " "));
        correct_letter = strtok(NULL, " ")[0];
        if (correct_letter == 'w')
        {
            pages[i].wr = 'W';
        }
        else if (correct_letter == 'r')
        {
            pages[i].wr = 'R';
        }
        else
        {
            pages[i].wr = correct_letter;
        }

        fgets(line, 256, file);
    }

    // Keep track of how many different numbers of frames user wants to see.
    frames = malloc(num_frames * sizeof(frame_info));

    printf("-------------------------------\n");
    printf("Reference String:\n");
    print_page_info(pages, num_pages);
    printf("-------------------------------\n");

    // Populate frame data.
    if (select_frame == -1)
    {
        // User wants to see all data for all algorithms.
        for (i = 0; i < num_frames; i++)
        {
            frames[i].num_frame = i + 1;
            calculate_page_faults(pages, num_pages, (&frames[i]));
        }
    }
    else
    {
        // User wants to see just frame count of all algorithms.
        frames[0].num_frame = select_frame;
        calculate_page_faults(pages, num_pages, (&frames[0]));
    }

    // Display data.
    printf("####################################################\n");
    printf("#     Frames    Opt.    FIFO     LRU      SC     ESC\n");
    print_frame_info(frames, num_frames);
    printf("####################################################\n");

    // Clean up
    free(pages);
    pages = NULL;
    free(frames);
    frames = NULL;

    return 0;
}

void calculate_page_faults(page * pages, int num_pages, frame_info * frame)
{

    // Compile all algorthims to add data to frame information structs.
    first_in_first_out(pages, num_pages, frame);
    least_recently_used(pages, num_pages, frame);
    second_chance_lru(pages, num_pages, frame);
    optimal(pages, num_pages, frame);
    enhanced_sc(pages, num_pages, frame);
}

void first_in_first_out(page * pages, int num_pages, frame_info * frame_info)
{
    int * frames = malloc(frame_info->num_frame * sizeof(int));
    int i;
    int j;
    // Keep track of page fault count.
    int page_fault_cnt = 0;
    int page_in_frames = 0;
    // Keep track of the oldest page.
    int replace_idx = 0;

    // Fill frames with -1's.
    for (i = 0; i < frame_info->num_frame; i++)
    {
        frames[i] = -1;
    }

    for (i = 0; i < num_pages; i++)
    {
        page_in_frames = 0;

        // First check if page is in frames.
        for (j = 0; j < frame_info->num_frame; j++)
        {
            if (pages[i].data == frames[j])
            {
                page_in_frames = 1;
            }
        }

        // If page is not in frame, add it to frames, give it an order_cnt
        if (page_in_frames == 0)
        {
            page_fault_cnt++;

            // Put data where the oldest page is.
            frames[replace_idx] = pages[i].data;
            replace_idx++;
            if (replace_idx >= frame_info->num_frame)
            {
                replace_idx = 0;
            }
        }
    }
        
    frame_info->fifo = page_fault_cnt;
        
    // Clean up
    free(frames);
    frames = NULL;
}

void least_recently_used(page * pages, int num_pages, frame_info * frame_info)
{
    int * frames = malloc(frame_info->num_frame * sizeof(int));
    // Keeps track of which frame was the least recently used.
    int * order = malloc(frame_info->num_frame * sizeof(int));
    int order_cnt = 1;
    int i;
    int j;
    // Keep track of page fault count.
    int page_fault_cnt = 0;
    int page_in_frames = 0;
    int replace_idx = 0;
    int lowest_cnt;

    // Fill frames with empty data.
    for (i = 0; i < frame_info->num_frame; i++)
    {
        frames[i] = -1;
        order[i] = 0;
    }

    for (i = 0; i < num_pages; i++)
    {
        page_in_frames = 0;

        // First check if page is in frames.
        for (j = 0; j < frame_info->num_frame; j++)
        {
            if (pages[i].data == frames[j])
            {
                page_in_frames = 1;
                order[j] = order_cnt;
                order_cnt++;
            }
        }

        // If page is not in frame, add it to frames, give it an order_cnt
        if (page_in_frames == 0)
        {
            page_fault_cnt++;

            lowest_cnt = order_cnt;
            for (j = 0; j < frame_info->num_frame; j++)
            {
                // Find the lowest order to replace in frames.
                if (lowest_cnt > order[j])
                {
                    lowest_cnt = order[j];
                    replace_idx = j;
                }
            }

            // Replace lowest order in frames.
            frames[replace_idx] = pages[i].data;
            order[replace_idx] = order_cnt;

            order_cnt++;
        }
    }
    frame_info->lru = page_fault_cnt;

    // Clean up
    free(frames);
    frames = NULL;
    free(order);
    order = NULL;
}

void optimal(page * pages, int num_pages, frame_info * frame_info)
{
    int * frames = malloc(frame_info->num_frame * sizeof(int));
    // Keeps track of when pages will be used in the futre.
    int * look_ahead = malloc(frame_info->num_frame * sizeof(int));
    int i;
    int j;
    int k;
    int l;
    int page_in_frames = 0;
    // Keep track of page fault count.
    int page_fault_cnt = 0;
    // Keeps track of what index of frames to replace.
    int replace_idx = -1;
    int lowest_cnt = 0;

    // Fill frames with empty data.
    for (i = 0; i < frame_info->num_frame; i++)
    {
        frames[i] = -1;
        look_ahead[i] = -1;
    }

    for (i = 0; i < num_pages; i++)
    {
        page_in_frames = 0;

        // First check if page is in frames.
        for (j = 0; j < frame_info->num_frame; j++)
        {
            if (pages[i].data == frames[j])
            {
                page_in_frames = 1;
            }
        }

        // If the page is not in the frame, see if there is an empty frame to
        // fill.
        if (page_in_frames == 0)
        {

            // If a page has not be filled, fill it.
            for (j = 0; j < frame_info->num_frame; j++)
            {
                if (frames[j] == -1)
                {
                    page_fault_cnt++;
                    frames[j] = pages[i].data;
                    page_in_frames = 1;
                    break;
                }
            }
        }

        // If page is not in frames, add it to frame.
        if (page_in_frames == 0)
        {
            page_fault_cnt++;

            // First find when each frame will be used in the future.
            for (k = i; k < num_pages; k++)
            {
                for (l = 0; l < frame_info->num_frame; l++)
                {
                    if (pages[k].data == frames[l] && look_ahead[l] == -1)
                    {
                        look_ahead[l] = k;
                    }
                }
            }

            lowest_cnt = 0;
            // Second find the highest indexed frame or -1;
            for (k = 0; k < frame_info->num_frame; k++)
            {
                if (look_ahead[k] > lowest_cnt)
                {
                    lowest_cnt = look_ahead[k];
                    replace_idx = k;
                }
                // If the page is never used again always replace it.
                else if (look_ahead[k] == -1)
                {
                    replace_idx = k;
                    break;
                }
            }

            // Replace the least recent page.
            frames[replace_idx] = pages[i].data;

            // Reset look ahead;
            for (j = 0; j < frame_info->num_frame; j++)
            {
                look_ahead[j] = -1;
            }
        }
    }

    frame_info->opt = page_fault_cnt;

    // Clean up
    free(frames);
    frames = NULL;
    free(look_ahead);
    look_ahead = NULL;

}

void second_chance_lru(page * pages, int num_pages, frame_info * frame_info)
{
    int * frames = malloc(frame_info->num_frame * sizeof(int));
    // Keeps track of the reference bits for each frame.
    int * second_chance = malloc(frame_info->num_frame * sizeof(int));
    int i;
    int j;
    // Keep track of page fault count.
    int page_fault_cnt = 0;
    int page_in_frames = 0;
    int check_frame = 0;
    int replaced_frame = 0;

    // Fill frames with empty data.
    for (i = 0; i < frame_info->num_frame; i++)
    {
        frames[i] = -1;
        second_chance[i] = 0;
    }

    for (i = 0; i < num_pages; i++)
    {
        page_in_frames = 0;

        // First check if the page is in the frames already.
        for (j = 0; j < frame_info->num_frame; j++)
        {
            if (pages[i].data == frames[j])
            {
                page_in_frames = 1;
                second_chance[j] = 1;
            }
        }

        // If the page is not in the frames, look to replace a frame.
        if (page_in_frames == 0)
        {
            page_fault_cnt++;
            replaced_frame = 0;

            // Will loop until a frame is replaced.
            while(replaced_frame == 0)
            {
                // If the reference bit is 0, replace the frame.
                if (second_chance[check_frame] == 0)
                {
                    frames[check_frame] = pages[i].data;
                    second_chance[check_frame] = 1;

                    replaced_frame = 1;
                }
                // Set the reference bit to 0.
                else
                {
                    second_chance[check_frame] = 0;
                }

                // If at the end of frames start at beginning.
                if (check_frame + 1 >= frame_info->num_frame)
                {
                    check_frame = 0;
                }
                else
                {
                    check_frame++;
                }
            }
        }
    }

    frame_info->sc = page_fault_cnt;

    // Clean up
    free(frames);
    frames = NULL;
    free(second_chance);
    second_chance = NULL;
}

void enhanced_sc(page * pages, int num_pages, frame_info * frame_info)
{
    esc * frames = malloc(frame_info->num_frame * sizeof(esc));
    // Keep track of page fault count.
    int page_fault_cnt = 0;
    int i;
    int j;
    // Keeps track of what frame to check to replace next.
    int check_frame = 0;
    // Helper for check_frame to see how many frames have been checked already.
    int pages_checked = 0;
    int page_in_frames = 0;
    // Tells the loop to check the dirty bit of the frames.
    int check_dirty_bit = 0;

    // Fill frames with empty data.
    for (i = 0; i < frame_info->num_frame; i++)
    {
        frames[i].page_num = -1;
        frames[i].ref_bit = 0;
        frames[i].dirty_bit = 0;
    }

    for (i = 0; i < num_pages; i++)
    {
        page_in_frames = 0;

        // Check if page is in frames.
        for (j = 0; j < frame_info->num_frame; j++)
        {
            if (frames[j].page_num == pages[i].data)
            {
                page_in_frames = 1;
                frames[j].ref_bit = 1;
                // Only change the dirty bit of the already added frame, if 
                // the frame was Read, but is now Written to.
                if (pages[i].wr == 'W' && frames[j].dirty_bit == 0)
                {
                    frames[j].dirty_bit = 1;
                }
            }
        }

        // If the page is not in the frames, increment page fault count.
        if (page_in_frames == 0)
        {
            page_fault_cnt++;
        }

        check_dirty_bit = 0;
        // Loop until the page is put into the frames.
        while (page_in_frames == 0)
        {
            pages_checked = 0;
            // While the page is not in the frames and there is still a loop
            // of the frames to be checked, stay in loop.
            while (pages_checked < frame_info->num_frame && page_in_frames == 0)
            {
                // Check the reference bit of the frame, and if in the correct
                // loop also check the dirty bit. If the page meeets the 
                // criteria replace the page.
                if (frames[check_frame].ref_bit == 0
                    && frames[check_frame].dirty_bit == check_dirty_bit)
                {
                    frames[check_frame].page_num = pages[i].data;
                    frames[check_frame].ref_bit = 1;
                    if (pages[i].wr == 'W')
                    {
                        frames[check_frame].dirty_bit = 1;
                    }
                    else
                    {
                        frames[check_frame].dirty_bit = 0;
                    }
                    // The page is now in the frames.
                    page_in_frames = 1;
                }
                // If in loop for checking the dirty bit, set the reference
                // bit of the checked frame to 0.
                else if (check_dirty_bit == 1)
                {
                    frames[check_frame].ref_bit = 0;
                }

                // Keep check_frame in the available indexes of the frames.
                if (check_frame + 1 >= frame_info->num_frame)
                {
                    check_frame = 0;
                }
                else
                {
                    check_frame++;
                }

                pages_checked++;
            }

            // If the loop was a dirty bit check, change to non-dirty bit check.
            if (check_dirty_bit == 1)
            {
                check_dirty_bit = 0;
            }
            else
            {
                check_dirty_bit++;
            }
        }
    }

    frame_info->esc = page_fault_cnt;

    // Clean up
    free(frames);
    frames = NULL;
}

void print_page_info(page * pages, int num_pages)
{
    int i;

    for (i = 1; i < num_pages + 1; i++)
    {
        // Show page information (don't show end comma)
        if (i == num_pages)
        {
            printf("%2d:%c", pages[i - 1].data, pages[i - 1].wr);
        }
        else
        {
            printf("%2d:%c, ", pages[i - 1].data, pages[i - 1].wr);
        }

        if (i % 10 == 0)
        {
            printf("\n");
        }
    }

    printf("\n");
}

void print_frame_info(frame_info * frames, int num_frames)
{
    int i;

    // Show frame information.
    for (i = 0; i < num_frames; i++)
    {
        printf("#    %7d %7d %7d %7d %7d %7d\n", 
            frames[i].num_frame, frames[i].opt, frames[i].fifo, 
            frames[i].lru, frames[i].sc, frames[i].esc);
    }
}