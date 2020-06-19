#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define WIDTH 10L
#define HEIGHT 10L

#define MOD(X,Y) ((X) - (((X)/(Y))*(Y)))

static size_t LENGTH = WIDTH*HEIGHT;

char MINEVAL = '(';
char UNREVEALED = -1;

int GetUniformRandomNumber(int lower, int upper);
void InitializeState(char* state, size_t width, size_t height, size_t nMines, size_t firstPos);
void PrintState(char* state, size_t width, size_t height);
long MaxL(long x, long y);
long MinL(long x, long y);

long MaxL(long x, long y) {
    return (x>y)?x:y;
}

long MinL(long x, long y) {
    return (x<y)?x:y;
}

void InitializeState(char* state, size_t width, size_t height, size_t nMines, size_t firstPos) {
    size_t length = width*height;
    for (size_t i=0L; i<length; i++)
        state[i] = 0;
    for (size_t placedMines = 0; placedMines<nMines;)
    {
        int minePos = GetUniformRandomNumber(0, length-1);
        if (state[minePos])
            continue;
        if (minePos == firstPos)
            continue;
        placedMines += 1;
        state[minePos] = '(';
    }
    for (size_t i = 0L; i < width; i++) {
        for (size_t j = 0L; j < height; j++) {
            if (state[i*width+j])
                continue;
            char count = 0;
            for (size_t k = MaxL(0L,i-1L); k<MinL(width,i+2L); k++) {
                for (size_t l = MaxL(0L,j-1L); l<MinL(height,j+2L); l++) {
                    if (state[k*width + l]==MINEVAL)
                        count++;
                }
            }
            state[i*width+j] = count;
        }
    }
}

void RevealCell(size_t cell, char* state, char* observation, size_t width, size_t height) {
    observation[cell] = state[cell];
    if (observation[cell] != 0)
        return;
    size_t i = cell/width;
    size_t j = cell-i*width;
    for (size_t k = MaxL(0L,i-1L); k<MinL(width,i+2L); k++) {
        for (size_t l = MaxL(0L,j-1L); l<MinL(height,j+2L); l++) {
            if (observation[k*width + l]==UNREVEALED) {
                RevealCell(k*width + l, state, observation, width, height);
            }
        }
    }
}

void PrintState(char* state, size_t width, size_t height) {
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j ++) {
            char symbol = ('0')+state[i*width + j];
            printf(" %c",symbol);
        }
        printf("\n");
    }
    printf("\n");
}

int GetUniformRandomNumber(int lower, int upper) {
    unsigned int nVals = upper-lower+1;
    // TODO double check what RAND_MAX is (in terms of data type, signedness, etc.)
    // see if you can use size_t (long) instead
    unsigned int nIntervals = RAND_MAX/nVals;
    unsigned int r;
    for (r = rand(); r > nVals*nIntervals; );
    r = MOD(r,nVals);
    return r + lower;
}

int main() {
    // seed the PRNG
    srand(time(NULL));

    char state[LENGTH];
    char observation[LENGTH];
    for (int i=0; i<LENGTH; i++) {
        observation[i] = UNREVEALED;
    }
    InitializeState(state, WIDTH, HEIGHT, 15, LENGTH/2);
    RevealCell(LENGTH/2, state, observation, WIDTH, HEIGHT);
    PrintState(state, WIDTH, HEIGHT);
    PrintState(observation, WIDTH, HEIGHT);

    return 0;
}
