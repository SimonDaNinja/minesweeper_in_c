#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 10

#define MOD(X,Y) ((X) - (((X)/(Y))*(Y)))


static size_t LENGTH = WIDTH*HEIGHT;

int GetUniformRandomNumber(int lower, int upper);
void InitializeState(char* state, size_t length, size_t nMines);
void PrintState(char* state, size_t width, size_t height);


void InitializeState(char* state, size_t length, size_t nMines) {
    for (int i=0; i<LENGTH; i++)
        state[i] = 0;
    for (size_t placedMines = 0; placedMines<nMines;)
    {
        int minePos = GetUniformRandomNumber(0, length-1);
        if (!state[minePos]){
            placedMines += 1;
            state[minePos] = 1;
        }
    }
}

void PrintState(char* state, size_t width, size_t height) {
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j ++) {
            char symbol = (state[i*width + j])?'X':' ';
            printf(" %c",symbol);
        }
        printf("\n");
    }
}

int GetUniformRandomNumber(int lower, int upper) {
    unsigned int nVals = upper-lower+1;
    unsigned int nIntervals = RAND_MAX/nVals;
    unsigned int r;
    for (r = rand(); r > nVals*nIntervals; );
    r = MOD(r,nVals);
    return r + lower;
}

void DisplayRandomNumbers(size_t n, int lower, int upper) {
    int r;
    for (unsigned int i=0; i < n; i++){
        r = GetUniformRandomNumber(lower,upper);
        printf("random number %d: %d\n", i, r);
    }
}

int main() {
    // seed the PRNG
    srand(time(NULL));

    char state[LENGTH];
    InitializeState(state, LENGTH, 15);
    PrintState(state, WIDTH, HEIGHT);

    return 0;
}
