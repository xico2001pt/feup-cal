#include "exercises.h"
#include <stdlib.h>
#include <time.h>

Sudoku::Sudoku() {
    this->initialize();
}

Sudoku::Sudoku(int nums[9][9]): Sudoku() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (nums[i][j] != 0) {
                int n = nums[i][j];
                if (n < 1 || n > 9 || !accepts(i, j, n))
                    throw IllegalArgumentException;
                else
                    place(i, j, n);
            }
        }
    }
}

void Sudoku::initialize() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int n = 0; n < 10; n++) {
                numbers[i][j] = 0;
                lineHasNumber[i][n] = false;
                columnHasNumber[j][n] = false;
                block3x3HasNumber[i / 3][j / 3][n] = false;
            }
        }
    }
    this->countFilled = 0;
}

bool Sudoku::isComplete() const {
    return countFilled == 9 * 9;
}

bool Sudoku::solve() {
    if (isComplete())
        return true;

    int line, column;
    bestPlace(line, column);

    for (int n = 1; n < 10; ++n) {
        if (accepts(line, column, n)) {
            place(line, column, n);
            if (solve())
                return true;
            clear(line, column);
        }
    }
    return false;
}

int Sudoku::countSolutions() {
    if (isComplete())
        return 1;


    int line, column, solutions = 0;
    bestPlace(line, column);

    for (int n = 1; n < 10; ++n) {
        if (accepts(line, column, n)) {
            place(line, column, n);
            solutions += countSolutions();
            if (solutions >= 2)
                return 2;
            clear(line, column);
        }
    }
    return solutions;
}

void Sudoku::generate() {
    while (true) {
        int i = rand() % 9;
        int j = rand() % 9;
        int n = (rand() % 9) + 1;
        if (accepts(i, j, n)) {
            place(i, j, n);
            int sol = countSolutions();
            if (sol == 0)
                clear(i, j);
            else if (sol == 1)
                return;
        }
    }
}

int** Sudoku::getNumbers() {
    int** ret = new int*[9];
    for (int i = 0; i < 9; i++) {
        ret[i] = new int[9];
        for (int a = 0; a < 9; a++)
            ret[i][a] = numbers[i][a];
    }
    return ret;
}

#include <iostream>

void Sudoku::print() const {
    for (int i = 0; i < 9; i++) {
        for (int a = 0; a < 9; a++)
            std::cout << this->numbers[i][a] << " ";
        std::cout << std::endl;
    }
}

bool Sudoku::accepts(int i, int j, int n) {
    return !(numbers[i][j] !=0 || lineHasNumber[i][n] || columnHasNumber[j][n] || block3x3HasNumber[i / 3][j / 3][n]);
}

void Sudoku::place(int i, int j, int n) {
    if (numbers[i][j] != 0)
        return;

    numbers[i][j] = n;
    lineHasNumber[i][n] = true;
    columnHasNumber[j][n] = true;
    block3x3HasNumber[i / 3][j / 3][n] = true;
    countFilled++;
}

int Sudoku::clear(int i, int j) {
    int n = numbers[i][j];

    numbers[i][j] = 0;
    lineHasNumber[i][n] = false;
    columnHasNumber[j][n] = false;
    block3x3HasNumber[i / 3][j / 3][n] = false;
    countFilled--;

    return n;
}

void Sudoku::clear() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (numbers[i][j] != 0)
                clear(i, j);
}

void Sudoku::bestPlace(int &x, int&y) {
    int minNum = 10, aux;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (numbers[i][j] != 0)
                continue;

            aux = 0;
            for (int n = 1; n < 10; ++n) {
                if (accepts(i, j, n))
                    ++aux;
            }
            if (aux < minNum) {
                minNum = aux;
                x = i;
                y = j;
            }
        }
    }
}

/// TESTS ///
#include <gtest/gtest.h>

void compareSudokus(int in[9][9], int out[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int a = 0; a < 9; a++)
            ASSERT_EQ(in[i][a], out[i][a]);
    }
}

TEST(TP2_Ex2, testSudokuAlreadySolved) {
    int in[9][9] =
            {{8, 6, 2, 3, 4, 1, 7, 9, 5},
             {1, 5, 4, 9, 7, 6, 3, 8, 2},
             {9, 3, 7, 8, 2, 5, 1, 4, 6},
             {5, 7, 6, 1, 3, 8, 9, 2, 4},
             {2, 1, 8, 5, 9, 4, 6, 3, 7},
             {3, 4, 9, 7, 6, 2, 8, 5, 1},
             {6, 2, 3, 4, 8, 7, 5, 1, 9},
             {7, 8, 5, 2, 1, 9, 4, 6, 3},
             {4, 9, 1, 6, 5, 3, 2, 7, 8}};

    Sudoku s(in);
    EXPECT_EQ(s.solve(), true);

    int out[9][9];
    int** res = s.getNumbers();

    for (int i = 0; i < 9; i++)
        for (int a = 0; a < 9; a++)
            out[i][a] = res[i][a];

    compareSudokus(in, out);
}

TEST(TP2_Ex2, testSudokuNoneBackStepsRequired) {
    int in[9][9] =
            {{8, 6, 0, 0, 0, 0, 0, 9, 0},
             {0, 0, 4, 0, 7, 6, 3, 0, 0},
             {9, 0, 0, 0, 2, 5, 1, 0, 0},
             {0, 7, 6, 1, 3, 0, 0, 2, 0},
             {2, 1, 0, 0, 0, 0, 0, 3, 7},
             {0, 4, 0, 0, 6, 2, 8, 5, 0},
             {0, 0, 3, 4, 8, 0, 0, 0, 9},
             {0, 0, 5, 2, 1, 0, 4, 0, 0},
             {0, 9, 0, 0, 0, 0, 0, 7, 8}};

    int out[9][9] =
            {{8, 6, 2, 3, 4, 1, 7, 9, 5},
             {1, 5, 4, 9, 7, 6, 3, 8, 2},
             {9, 3, 7, 8, 2, 5, 1, 4, 6},
             {5, 7, 6, 1, 3, 8, 9, 2, 4},
             {2, 1, 8, 5, 9, 4, 6, 3, 7},
             {3, 4, 9, 7, 6, 2, 8, 5, 1},
             {6, 2, 3, 4, 8, 7, 5, 1, 9},
             {7, 8, 5, 2, 1, 9, 4, 6, 3},
             {4, 9, 1, 6, 5, 3, 2, 7, 8}};

    Sudoku s(in);
    EXPECT_EQ(s.solve(), true);

    int sout[9][9];
    int** res = s.getNumbers();

    for (int i = 0; i < 9; i++)
        for (int a = 0; a < 9; a++)
            sout[i][a] = res[i][a];

    compareSudokus(out, sout);
}

TEST(TP2_Ex2, testSudokuSomeBackStepsRequired) {
    int in[9][9] =
            {{7, 0, 5, 2, 6, 3, 4, 0, 9},
             {0, 0, 0, 0, 0, 0, 0, 3, 0},
             {0, 0, 0, 0, 8, 0, 0, 0, 0},
             {0, 0, 9, 5, 0, 4, 0, 0, 2},
             {5, 0, 6, 0, 0, 0, 7, 0, 8},
             {2, 0, 0, 8, 0, 0, 1, 0, 0},
             {0, 0, 0, 0, 1, 0, 0, 0, 0},
             {0, 2, 0, 0, 0, 0, 0, 0, 0},
             {3, 0, 8, 7, 2, 9, 6, 0, 4}};

    int out[9][9] =
            {{7, 1, 5, 2, 6, 3, 4, 8, 9},
             {8, 6, 4, 9, 5, 7, 2, 3, 1},
             {9, 3, 2, 4, 8, 1, 5, 7, 6},
             {1, 8, 9, 5, 7, 4, 3, 6, 2},
             {5, 4, 6, 1, 3, 2, 7, 9, 8},
             {2, 7, 3, 8, 9, 6, 1, 4, 5},
             {4, 9, 7, 6, 1, 5, 8, 2, 3},
             {6, 2, 1, 3, 4, 8, 9, 5, 7},
             {3, 5, 8, 7, 2, 9, 6, 1, 4}};

    Sudoku s(in);
    EXPECT_EQ(s.solve(), true);

    int sout[9][9];
    int** res = s.getNumbers();

    for (int i = 0; i < 9; i++)
        for (int a = 0; a < 9; a++)
            sout[i][a] = res[i][a];

    compareSudokus(out, sout);
}

TEST(TP2_Ex2, testSudokuManyBackStepsRequired) {
    int in[9][9] =
            {{1, 0, 0, 0, 0, 7, 0, 0, 0},
             {0, 7, 0, 0, 6, 0, 8, 0, 0},
             {2, 0, 0, 0, 4, 0, 6, 0, 0},
             {7, 6, 4, 0, 0, 0, 9, 0, 0},
             {0, 0, 0, 0, 2, 0, 5, 6, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 1, 0, 0, 3, 0, 0, 0, 0},
             {4, 0, 0, 1, 0, 0, 0, 0, 5},
             {0, 5, 0, 0, 0, 4, 0, 9, 0}};

    int out[9][9] =
            {{1, 4, 6, 8, 5, 7, 2, 3, 9},
             {3, 7, 9, 2, 6, 1, 8, 5, 4},
             {2, 8, 5, 9, 4, 3, 6, 7, 1},
             {7, 6, 4, 3, 1, 5, 9, 2, 8},
             {8, 3, 1, 4, 2, 9, 5, 6, 7},
             {5, 9, 2, 6, 7, 8, 4, 1, 3},
             {9, 1, 8, 5, 3, 2, 7, 4, 6},
             {4, 2, 7, 1, 9, 6, 3, 8, 5},
             {6, 5, 3, 7, 8, 4, 1, 9, 2}};

    Sudoku s(in);
    EXPECT_EQ(s.solve(), true);

    int sout[9][9];
    int **res = s.getNumbers();

    for (int i = 0; i < 9; i++)
        for (int a = 0; a < 9; a++)
            sout[i][a] = res[i][a];

    compareSudokus(out, sout);
}

TEST(TP2_Ex2, testSudokuWithMinimalClues) {
    int in[9][9] =
            {{7, 0, 0, 1, 0, 8, 0, 0, 0},
             {0, 9, 0, 0, 0, 0, 0, 3, 2},
             {0, 0, 0, 0, 0, 5, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 1, 0, 0},
             {9, 6, 0, 0, 2, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 8, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 5, 0, 0, 1, 0, 0, 0},
             {3, 2, 0, 0, 0, 0, 0, 0, 6}};

    int out[9][9] =
            {{7, 5, 2, 1, 3, 8, 6, 9, 4},
             {1, 9, 8, 7, 4, 6, 5, 3, 2},
             {4, 3, 6, 2, 9, 5, 7, 8, 1},
             {2, 8, 3, 4, 5, 9, 1, 6, 7},
             {9, 6, 1, 8, 2, 7, 3, 4, 5},
             {5, 7, 4, 6, 1, 3, 8, 2, 9},
             {6, 1, 9, 3, 7, 2, 4, 5, 8},
             {8, 4, 5, 9, 6, 1, 2, 7, 3},
             {3, 2, 7, 5, 8, 4, 9, 1, 6}};

    Sudoku s(in);
    EXPECT_EQ(s.solve(), true);

    int sout[9][9];
    int** res = s.getNumbers();

    for (int i = 0; i < 9; i++)
        for (int a = 0; a < 9; a++)
            sout[i][a] = res[i][a];

    compareSudokus(out, sout);
}

TEST(TP2_Ex2, testSudokuWithMultipleSolutions) {
    int in[9][9] =
            {{0/*7*/, 0, 0, 1, 0, 8, 0, 0, 0},
             {0, 9, 0, 0, 0, 0, 0, 3, 2},
             {0, 0, 0, 0, 0, 5, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 1, 0, 0},
             {9, 6, 0, 0, 2, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 8, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 5, 0, 0, 1, 0, 0, 0},
             {3, 2, 0, 0, 0, 0, 0, 0, 6}};

    Sudoku s(in);
    EXPECT_EQ(s.solve() && s.isComplete(), true);
    int** out = s.getNumbers();
    for (int i=0; i<9; i++)
        for (int j=0; j<9; j++)
            if (in[i][j] != 0)
                EXPECT_EQ(in[i][j], out[i][j]);
}

TEST(TP2_Ex2, testSudokuEmpty) {
    int in[9][9] =
            {{0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    Sudoku s(in);
    EXPECT_EQ(s.solve(), true);
    EXPECT_EQ(s.isComplete(), true);
}

TEST(TP2_Ex2, testSudokuImpossible) {
    int in[9][9] =
            {{7, 0, 0, 1, 0, 8, 0, 0, 0},
             {4/*0*/, 9, 0, 0, 0, 0, 0, 3, 2},
             {0, 0, 0, 0, 0, 5, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 1, 0, 0},
             {9, 6, 0, 0, 2, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 8, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 5, 0, 0, 1, 0, 0, 0},
             {3, 2, 0, 0, 0, 0, 0, 0, 6}};

    Sudoku s(in);
    EXPECT_EQ(s.solve(), false);

    int out[9][9];
    int** res = s.getNumbers();

    for (int i = 0; i < 9; i++)
        for (int a = 0; a < 9; a++)
            out[i][a] = res[i][a];

    compareSudokus(in, out);
}

TEST(TP2_Ex2, testSudokuVariousSolutions) {
    int in1[9][9] =
            {{2,9,5,7,4,3,8,6,1},
             {4,3,1,8,6,5,9,0,0},
             {8,7,6,1,9,2,5,4,3},
             {3,8,7,4,5,9,2,1,6},
             {6,1,2,3,8,7,4,9,5},
             {5,4,9,2,1,6,7,3,8},
             {7,6,3,5,2,4,1,8,9},
             {9,2,8,6,7,1,3,5,4},
             {1,5,4,9,3,8,6,0,0}};

    Sudoku s1(in1);
    EXPECT_EQ(s1.countSolutions(), 2);

    int in2[9][9] =
            { {5, 3, 0, 0, 7, 0, 0, 0, 0},
              {6, 0, 0, 1, 9, 5, 0, 0, 0},
              {0, 9, 8, 0, 0, 0, 0, 6, 0},
              {8, 0, 0, 0, 6, 0, 0, 0, 3},
              {4, 0, 0, 8, 0, 3, 0, 0, 1},
              {7, 0, 0, 0, 2, 0, 0, 0, 6},
              {0, 6, 0, 0, 0, 0, 2, 8, 0},
              {0, 0, 0, 4, 1, 9, 0, 0, 5},
              {0, 0, 0, 0, 8, 0, 0, 0, 0} };
    Sudoku s2(in2);
    EXPECT_EQ(s2.countSolutions(),2);

    int in3[9][9] =
            {{0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 3, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 4, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 2, 0, 0, 0, 0, 0, 1},
             {0, 0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    Sudoku s3(in3);
    EXPECT_EQ(s3.countSolutions(), 2);
}