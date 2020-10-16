#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

/*
*   Conway's Game of Life (in C)
*   ----------------------------
*   Implements John Conway's game of life simulation, with the caveat that the grid is finite and limited to a size WIDTHxHEIGHT.
*   In other words, infinite growth off the grid is not allowed (unlike some other simulations).
*
*/

//  Defaults, no reason to change
int WIDTH = 20, HEIGHT = 20, ITERATIONS = 100, WAIT_TIME_MS = 100;


void sleep_ms(int milliseconds){ 
    // cross-platform sleep function
    // borrowed from: https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if (milliseconds >= 1000)
      sleep(milliseconds / 1000);
    usleep((milliseconds % 1000) * 1000);
#endif
}
void print_array(int (*array)[HEIGHT][WIDTH]){
    for(size_t y = 0; y < HEIGHT; y++){
        printf("\n\t");
        for(size_t x = 0; x < WIDTH; x++){
            printf("%d ", (*array)[y][x]);
        }
    }
    printf("\n");
}

int neighbor_count(int pos_x, int pos_y, int (*array)[HEIGHT][WIDTH]){
    size_t count = 0;
    for(int y = -1; y < 2; y++){
        for(int x = -1; x < 2; x++){
            if(!(y == 0 && x == 0) && y+pos_y >= 0 && x+pos_x >= 0){
                if((*array)[pos_y+y][pos_x+x] == 1) count++;
            }
        }
    }
    return count;
}
void copy_array(int (*src)[HEIGHT][WIDTH], int (*dst)[HEIGHT][WIDTH]){
    for(size_t y = 0; y < HEIGHT; y++){
        for(size_t x = 0; x < WIDTH; x++){
            (*dst)[y][x] = (*src)[y][x];
        }
    }
}

void gol_step(int (*array)[HEIGHT][WIDTH]){
    int cells_to_kill[HEIGHT*WIDTH][2];
    int cells_to_birth[HEIGHT*WIDTH][2];
    int read_only_array[HEIGHT][WIDTH];
    int ncount;
    copy_array(array,&read_only_array);
    for(size_t y = 0; y < HEIGHT; y++){
        for(size_t x = 0; x < WIDTH; x++){
            ncount = neighbor_count(x,y,&read_only_array);
            //  Live cell
            if(read_only_array[y][x] == 1){
                //  Cell death
                if(ncount != 2 && ncount != 3){
                    (*array)[y][x] = 0;
                }
            }
            //  Dead cell
            else{
                //  Cell birth
                if(ncount == 3){
                    (*array)[y][x] = 1;
                }
            }
        }
    }
}

void play_the_gol(int iterations, int (*game_array)[HEIGHT][WIDTH], size_t wait_time_ms){
    printf("Starting board:");
    print_array(game_array);
    sleep_ms(3000);
    for(int i = 1; i <= iterations; i++){
        system("clear");
        gol_step(game_array);
        printf("Iteration: %d", i);
        print_array(game_array);
        sleep_ms(wait_time_ms);
    }
}

void initial_config(){
    char buffer[256];
    printf("Enter grid width: ");
    if(fgets(buffer, sizeof(buffer), stdin)){
        if(buffer[0] == '\n'){
            printf("Defaulting to WIDTH=%d\n",WIDTH);
        }
        else{
            buffer[strlen(buffer) - 1] = 0;
            WIDTH = strtol(buffer, NULL, 10);
        }
    }
    else{
        printf("Defaulting to WIDTH=%d\n",WIDTH);
    }
    printf("Enter grid height: ");
    if(fgets(buffer, sizeof(buffer), stdin)){
        if(buffer[0] == '\n'){
            printf("Defaulting to HEIGHT=%d\n",HEIGHT);
        }
        else{
            buffer[strlen(buffer) - 1] = 0;
            HEIGHT = strtol(buffer, NULL, 10);
        }
    }
    else{
        printf("Defaulting to HEIGHT=%d\n",HEIGHT);
    }
    printf("Enter iterations to perform: ");
    if(fgets(buffer, sizeof(buffer), stdin)){
        if(buffer[0] == '\n'){
            printf("Defaulting to ITERATIONS=%d\n",ITERATIONS);
        }
        else{
            buffer[strlen(buffer) - 1] = 0;
            ITERATIONS = strtol(buffer, NULL, 10);
        }
    }
    else{
        printf("Defaulting to ITERATIONS=%d\n",ITERATIONS);
    }
    printf("Enter wait time (in milliseconds): ");
    if(fgets(buffer, sizeof(buffer), stdin)){
        if(buffer[0] == '\n'){
            printf("Defaulting to WAIT_TIME_MS=%d\n",WAIT_TIME_MS);
        }
        else{
            buffer[strlen(buffer) - 1] = 0;
            WAIT_TIME_MS = strtol(buffer, NULL, 10);
        }
    }
    else{
        printf("Defaulting to ITERATIONS=%d\n",ITERATIONS);
    }
}

void generate_board_randomly(int (*out_array)[HEIGHT][WIDTH]){
    srand(time(NULL));
    for(size_t y = 0; y < HEIGHT; y++){
        for(size_t x = 0; x < WIDTH; x++){
            (*out_array)[y][x] = rand() % 2;
        }
    }
}

int main(){
    initial_config();
    int game_array[HEIGHT][WIDTH];
    memset(game_array, 0, HEIGHT*WIDTH*sizeof(int));
    generate_board_randomly(&game_array);
    play_the_gol(ITERATIONS, &game_array, WAIT_TIME_MS);
}