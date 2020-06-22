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
size_t N_MINES = 5L;

int GetUniformRandomInt(int lower, int upper);
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
    for (size_t placedMines = 0L; placedMines<nMines;)
    {
        int minePos = GetUniformRandomInt(0, length-1);
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

void RevealCell(size_t cell, char* state, char* observation, size_t width, size_t height, int* nRevealsAddr) {
    if (observation[cell] != UNREVEALED)
        return;
    observation[cell] = state[cell];
    *nRevealsAddr = *nRevealsAddr + 1;
    if (observation[cell] != 0)
        return;
    size_t i = cell/width;
    size_t j = cell-i*width;
    for (size_t k = MaxL(0L,i-1L); k<MinL(width,i+2L); k++) {
        for (size_t l = MaxL(0L,j-1L); l<MinL(height,j+2L); l++) {
            if (observation[k*width + l]==UNREVEALED) {
                RevealCell(k*width + l, state, observation, width, height, nRevealsAddr);
            }
        }
    }
}

void RevealState(char* state, char* observation, size_t width, size_t height) {
    size_t length = height*width;
    int dummy = 0;
    int *dummyAddr = &dummy;
    for (size_t i = 0L; i<length; i++) {
        if (observation[i] == UNREVEALED){
            RevealCell(i, state, observation, width, height, dummyAddr);
        }
    }
}

void PrintState(char* state, size_t width, size_t height) {
    printf("    ");
    for (size_t i = 0L; i < width; i++) {
        printf("%ld)", i+1);
    }
    printf("\n");
    for (size_t i = 0L; i < width; i++) {
        //TODO handle more elegantly (get rid of special case by using printf better)
        if (i<9)
            printf("%ld) ",i+1L);
        else
            printf("%ld)",i+1L);
        for (size_t j = 0L; j < height; j ++) {
            char symbol = ('0')+state[i*width + j];
            if (symbol == '0')
                symbol = ' ';
            printf(" %c",symbol);
        }
        printf("\n");
    }
    printf("\n");
}

int GetUniformRandomInt(int lower, int upper) {
    unsigned int nVals = upper-lower+1;
    // TODO double check what RAND_MAX is (in terms of data type, signedness, etc.)
    // see if you can use size_t (long) instead
    unsigned int nIntervals = RAND_MAX/nVals;
    unsigned int r;
    for (r = rand(); r > nVals*nIntervals; );
    r = MOD(r,nVals);
    return r + lower;
}

void StringLower(char* inputString) {
    for (size_t i = 0; inputString[i] != '\0'; i++) {
        if (inputString[i] <= 'Z' && inputString[i] >= 'A') {
            char upperCharacter = inputString[i];
            upperCharacter += 32;
            inputString[i] = upperCharacter;
        }
    }
}

void StringUpper(char* inputString) {
    for (size_t i = 0; inputString[i] != '\0'; i++) {
        if (inputString[i] <= 'z' && inputString[i] >= 'a') {
            inputString[i] = inputString[i] - 32;
        }
    }
}

 int Power(int a, int b) {
    int output = 1;
    for (int i = 0; i<b; i++)
        output = output*a;
    return output;
}

//TODO handle non-number strings properly
unsigned int ParseStringToNumber(char* inputString) {
    size_t stringLength = 0;
    for (size_t i = 0; inputString[i] != '\0'; i++){
        stringLength ++;
    }
    unsigned int outputInt = 0;
    for (size_t i = 0; i < stringLength; i++) {
        int exponent = stringLength-i-1;
        outputInt += (int)(inputString[i] - '0')*Power(10,exponent);
    }
    return outputInt;
}

unsigned int GetCellFromUserInput() {
    char userInput[128];
    printf("select row: ");
    scanf("%s", userInput);
    unsigned int row = ParseStringToNumber(userInput);
    printf("select column: ");
    scanf("%s", userInput);
    unsigned int column = ParseStringToNumber(userInput);
    unsigned int cell = row*WIDTH + column;
    return cell-1;
}


int main() {
    // seed the PRNG
    srand(time(NULL));
    int nReveals = 0;

    char state[LENGTH];
    char observation[LENGTH];
    for (int i=0; i<LENGTH; i++) {
        observation[i] = UNREVEALED;
    }
    system("clear");
    PrintState(observation, WIDTH, HEIGHT);
    unsigned int cell = GetCellFromUserInput();
    InitializeState(state, WIDTH, HEIGHT, N_MINES, cell);
    RevealCell(cell, state, observation, WIDTH, HEIGHT, &nReveals);

    while(nReveals < (LENGTH - N_MINES)) {
        //TODO handle rows and columns out of bounds
        system("clear");
        PrintState(observation, WIDTH, HEIGHT);
        cell = GetCellFromUserInput();
        if (state[cell] == MINEVAL) {
            system("clear");
            PrintState(state, WIDTH, HEIGHT);
            printf("you are dead\n");
            return 0;
        }
        RevealCell(cell, state, observation, WIDTH, HEIGHT, &nReveals);
    }
    system("clear");
    PrintState(state, WIDTH, HEIGHT);
    printf("congratulations, you won!\n");
    return 0;
}
