#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef uint8_t  u8;
typedef uint16_t u16;

#define count_ones __builtin_popcount
#define BOARD_SIZE 81 // 9 * 9

typedef struct {
    u8 numbers[BOARD_SIZE];
} Board;

typedef struct {
    u16 x, y, value;
} Change;

typedef struct {
    Change stack[BOARD_SIZE];
    int cursor;
} History;

typedef enum {
    ONE   = 1 << 0,
    TWO   = 1 << 1,
    THREE = 1 << 2,
    FOUR  = 1 << 3,
    FIVE  = 1 << 4,
    SIX   = 1 << 5,
    SEVEN = 1 << 6,
    EIGHT = 1 << 7,
    NINE  = 1 << 8
} Digit;

void print_options(u16 options);
void print_board(Board board); 

Digit u8_to_digit(u8 num) { return 1 << (num - 1); }
u8 digit_to_u8(Digit num) { return log(num) / log(2) + 1; }

// if there are no more tries left, then we don't go back in history past that cell, I guess...
u16 tried[BOARD_SIZE] = { };
History history = { };

// eliminates numbers by looking through rows, columns & blocks
u16 valid_options(Board board, int x, int y) {
    int width  = sqrt(BOARD_SIZE);
    int block  = sqrt(width);
    u16 answer = 0x01FF;

    // the row
    for(int i = 0; i < width; i ++) {
        answer &= ~u8_to_digit(board.numbers[i + y*width]);
    }

    // the column
    for(int i = 0; i < width; i ++) {
        answer &= ~u8_to_digit(board.numbers[x + i*width]);
    }

    // the sub-box
    for(int i = 0; i < width; i ++) {
        x = x / 3 * 3;
        y = y / 3 * 3;
        int x2 = i % block; 
        int y2 = i / block; 
        answer &= ~u8_to_digit(board.numbers[(x+x2) + (y+y2)*width]);
    }
    
    return answer;
}


void solve_sudoku(Board board) {
    int width = sqrt(BOARD_SIZE);

    // prepass to not try the numbers...
    for(int y = 0; y < width; y ++) {
        for(int x = 0; x < width; x ++) {
            tried[x + y*width] = 0x01FF * (board.numbers[x + y*width] != 0);
        }
    }

    for(int the_try = 0; the_try < 100; the_try ++) {

        Change fewest_options = { .value = 0x01FF };
        for(int y = 0; y < width; y ++) {
            for(int x = 0; x < width; x ++) {
                if(board.numbers[x + y*width] != 0) continue;

                u16 options    = valid_options(board, x, y);
                u16 opts_tried = tried[x + y*width];
                // options = options & ~opts_tried;

                if(options == 0) continue;

                if(count_ones(options) < count_ones(fewest_options.value)){
                    fewest_options.value = options;
                    fewest_options.x = x;
                    fewest_options.y = y;

                    if(count_ones(options) == 1) {
                        goto break1;
                    }
                }

            }
        }
        
        // go back in history to the last time we had no more tries left
        if(count_ones(fewest_options.value) == 0) {
            for(; history.cursor >= 0; history.cursor --) {
                Change step = history.stack[history.cursor];
                if((fewest_options.value & ~tried[step.x + step.y*width]) == 0) {
                    break;
                }
            }
        }

        break1:

        // printf("(%d %d)\n", fewest_options.x, fewest_options.y);
        // print_options(fewest_options.value);

        if(count_ones(fewest_options.value) == 1) {
            board.numbers[fewest_options.x + fewest_options.y*width] = digit_to_u8(fewest_options.value);
            continue;
        } else {
            printf("cycles done: %d\n", the_try + 1);
            break;
        }

        // TODO implementing history & ...
        // would allow me to solve 
        // harder sudokus
        //
        // but this is enough to solve leetcode example...

    }

    print_board(board);
}

void print_board(Board board) {
    int width = sqrt(BOARD_SIZE);
    int space = sqrt(width);
    puts("");
    for(int i = 0; i < BOARD_SIZE; i ++) {
        printf("%d  ", board.numbers[i]);
        if(i % space == space - 1) printf("  ");
        if(i % width == width - 1) {
            puts("");
            if((i/width) % space == space - 1) puts("");
        }
    }
    puts("");
}

void print_options(u16 options) {
    printf("options = { ");
    for(int i = 1; i < 10; i ++) {
        if((options & (1<<(i-1))) != 0) {
            printf("%d ", i);
        }
    }
    printf("}\n");
}

int main() {

    Board board = { {
        5, 3, 0, 0, 7, 0, 0, 0, 0,
        6, 0, 0, 1, 9, 5, 0, 0, 0,
        0, 9, 8, 0, 0, 0, 0, 6, 0,
        8, 0, 0, 0, 6, 0, 0, 0, 3,
        4, 0, 0, 8, 0, 3, 0, 0, 1,
        7, 0, 0, 0, 2, 0, 0, 0, 6,
        0, 6, 0, 0, 0, 0, 2, 8, 0,
        0, 0, 0, 4, 1, 9, 0, 0, 5,
        0, 0, 0, 0, 8, 0, 0, 7, 9
    } };

    // print_board(board);
    
    solve_sudoku(board);
}
