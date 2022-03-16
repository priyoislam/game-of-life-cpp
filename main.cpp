#define ALIVE_CELL_COLOR glColor3f(0.0f, 0.8f, 0.0f);
#define DEAD_CELL_COLOR glColor3f(0.0f, 0.0f, 0.0f);

#define ROUND_ZERO(a) ((a) - 1 < 0 ? 0 : (a) - 1)
#define ROUND_MAX(a, max) ((a) + 1 > (max) ? (max) : (a) + 1)

// using opengl for rendering
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;
#include <algorithm>


const unsigned int ROWS = 100;
const unsigned int COLUMNS = 100;
const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

unsigned int FPS = 4;

const float CELL_WIDTH = (float)WIDTH / (float)ROWS;
const float CELL_HEIGHT = (float)HEIGHT / (float)COLUMNS;

bool cells[COLUMNS][ROWS];

bool continue_calc = true;

void initCells(bool randomize)
{
    for(unsigned int i = 0; i < COLUMNS; i++)
    {
        for(unsigned int j = 0; j < ROWS; j++)
        {
            if((int)rand()%2 == 0 || !randomize)
                cells[i][j] = false;
                //if randomize is false ,every cells become false or any even number of times cells become randomly false
            else
                cells[i][j] = true;
        }
    }

    continue_calc = randomize;
    if(!randomize) FPS = 60;
}

void init(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    /*Specifies which matrix stack is the target for subsequent matrix operations.
    Three values are accepted: GL_MODELVIEW, GL_PROJECTION, and GL_TEXTURE
    GL_PROJECTION : Applies subsequent matrix operations to the projection matrix stack.*/
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    //GL_MODELVIEW : Applies subsequent matrix operations to the modelview matrix stack.

    initCells(true);
}

unsigned int getAliveNeightborCount(const int y, const int x)
{
    unsigned int neighbours = 0;

    // horizonal neighbours
    if(cells[y][ROUND_ZERO(x)])
        neighbours++;

    if(cells[y][ROUND_MAX(x, COLUMNS)])
        neighbours++;

    // vertical neighbours
    if(cells[ROUND_ZERO(y)][x])
        neighbours++;

    if(cells[ROUND_MAX(y, COLUMNS - 1)][x])
        neighbours++;

    // diagonal neighbours
    if(cells[ROUND_ZERO(y)][ROUND_MAX(x, ROWS - 1)])
        neighbours++;

    if(cells[ROUND_ZERO(y)][ROUND_ZERO(x)])
        neighbours++;

    if(cells[ROUND_MAX(y, COLUMNS - 1)][ROUND_MAX(x, ROWS - 1)])
        neighbours++;

    if(cells[ROUND_MAX(y, COLUMNS - 1)][ROUND_ZERO(x)])
        neighbours++;

    return neighbours;
}

void recalculateCells(void)
{
    bool new_cells[COLUMNS][ROWS];
    std::copy(&cells[0][0], &cells[0][0] + ROWS * COLUMNS, &new_cells[0][0]);

    for(unsigned int i = 0; i < COLUMNS; i++)
    {
        for(unsigned int j = 0; j < ROWS; j++)
        {
            // source of comments on the behavior are from wikipedia (https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

            unsigned int n_count = getAliveNeightborCount(i, j);
            // alive
            if(cells[i][j])
            {

                /* Any live cell with fewer than two live neighbours dies, as if caused by under-population.and
                 Any live cell with more than three live neighbours dies, as if by over-population.*/
                if(n_count < 2 || n_count > 3)
                    new_cells[i][j] = false;

                // Any live cell with two or three live neighbours lives on to the next generation. nothing to do here, the cell survives
            }
            else
            {
                // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                if(n_count == 3)
                    new_cells[i][j] = true;
            }
        }
    }

    std::copy(&new_cells[0][0], &new_cells[0][0] + ROWS * COLUMNS, &cells[0][0]);
}

void renderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(unsigned int i = 0; i < COLUMNS; i++)
    {
        for(unsigned int j = 0; j < ROWS; j++)
        {
            if(cells[i][j])
                ALIVE_CELL_COLOR
                else
                    DEAD_CELL_COLOR

                    glRectf(
                        (CELL_WIDTH * j) / WIDTH,
                        (CELL_HEIGHT * i) / HEIGHT,
                        (CELL_WIDTH * (j + 1)) / WIDTH,
                        (CELL_HEIGHT * (i + 1)) / HEIGHT
                    );

        }
    }

    if(continue_calc) recalculateCells();
    glutSwapBuffers();
    /*Performs a buffer swap on the layer in use for the current window.
    Specifically, glutSwapBuffers promotes the contents of the back buffer
    of the layer in use of the current window to become the contents of the front buffer.
     The contents of the back buffer then become undefined.*/
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

void glidergun()
{
    /*The glider is a pattern that travels across
    the board in Conway's Game of Life. It was first discovered by Richard K. Guy in 1969,
    while John Conway's group was attempting to track the evolution of the R-pentomino.
    Gliders are the smallest spaceships, and they travel diagonally at a speed of one cell every four generations,
    or {\displaystyle c/4}c/4. The glider is often produced from randomly generated starting configurations.*/

    initCells(false);
    unsigned int start_x = (unsigned int)(ROWS / 2) - 18;
    unsigned int start_y = (unsigned int)(COLUMNS / 2) - 4;

    cells[start_y][start_x] = true;
    cells[start_y][start_x + 1] = true;
    cells[start_y + 1][start_x] = true;
    cells[start_y + 1][start_x + 1] = true;

    cells[start_y][start_x + 10] = true;
    cells[start_y + 1][start_x + 10] = true;
    cells[start_y - 1][start_x + 10] = true;

    cells[start_y + 2][start_x + 10 + 1] = true;
    cells[start_y - 2][start_x + 10 + 1] = true;

    cells[start_y + 3][start_x + 10 + 2] = true;
    cells[start_y - 3][start_x + 10 + 2] = true;
    cells[start_y + 3][start_x + 10 + 3] = true;
    cells[start_y - 3][start_x + 10 + 3] = true;

    cells[start_y][start_x + 10 + 4] = true;

    cells[start_y + 2][start_x + 10 + 5] = true;
    cells[start_y - 2][start_x + 10 + 5] = true;

    cells[start_y + 1][start_x + 10 + 6] = true;
    cells[start_y][start_x + 10 + 6] = true;
    cells[start_y - 1][start_x + 10 + 6] = true;

    cells[start_y][start_x + 10 + 7] = true;

    cells[start_y + 1][start_x + 20] = true;
    cells[start_y + 2][start_x + 20] = true;
    cells[start_y + 3][start_x + 20] = true;

    cells[start_y + 1][start_x + 21] = true;
    cells[start_y + 2][start_x + 21] = true;
    cells[start_y + 3][start_x + 21] = true;

    cells[start_y + 4][start_x + 22] = true;
    cells[start_y][start_x + 22] = true;

    cells[start_y + 5][start_x + 24] = true;
    cells[start_y + 4][start_x + 24] = true;
    cells[start_y][start_x + 24] = true;
    cells[start_y - 1][start_x + 24] = true;

    cells[start_y + 2][start_x + 34] = true;
    cells[start_y + 3][start_x + 34] = true;

    cells[start_y + 2][start_x + 35] = true;
    cells[start_y + 3][start_x + 35] = true;

}
void acorn ()
{
    /*Patterns which evolve for long periods before stabilizing are called Methuselahs,
    Acorn takes 5206 generations to generate 633 cells, including 13 escaped gliders */
    initCells(false);
    unsigned int  start_x = (unsigned int)(ROWS / 2) - 3;
    unsigned int  start_y = (unsigned int)(COLUMNS / 2) - 1;


    cells[start_y][start_x] = true;

    cells[start_y][start_x + 1] = true;
    cells[start_y + 2][start_x + 1] = true;

    cells[start_y + 1][start_x + 3] = true;

    cells[start_y][start_x + 4] = true;
    cells[start_y][start_x + 5] = true;
    cells[start_y][start_x + 6] = true;
}
void keyboardInput(unsigned char key, int x, int y)
{
    unsigned int start_x;
    unsigned int start_y;

    switch(key)
    {
    case 'c':
        initCells(false);
        return;
    case 'r':
        initCells(true);
        return;
    case 's':
        continue_calc = true;
        FPS = 4;
        return;
    case '1':
        glidergun();
        return;

    case '2':
        acorn();
        return;
    }
}

void adaptCell(const int& mx, const int& my, const bool& revert = true)
{
    unsigned int x = (unsigned int)(mx / CELL_WIDTH);
    unsigned int y = COLUMNS - (unsigned int)(my / CELL_HEIGHT) - 1;
    if(revert) cells[y][x] = !cells[y][x];
    else cells[y][x] = true;
}

void mouseInput(int button, int state, int mx, int my)
{
    if(state == GLUT_UP) adaptCell(mx, my);
    /*The state parameter is either GLUT_UP or GLUT_DOWN indicating whether the callback was due to a release or press respectively.*/
}

void continuousMouseInput(int mx, int my)
{
    adaptCell(mx, my, false);
}


void welctext()
{
    cout << "Conway's Game of Life:\n\n" << endl;

    cout << "Press S to Start" << endl;
    cout << "Press C to Clear screen" << endl;
    cout << "Press R to Reset screen" << endl;
    cout << "Press 1 for Glider Pattern" << endl;
    cout << "Press 2 for Acorn Pattern" << endl;
    cout << "Press,Drag and Relase mouse for new seed in the screen" << endl;

}
int main(int argc, char** argv)
{
    welctext();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Game Of Life");
    glutDisplayFunc(renderFunction);
    /*glutDisplayFunc sets the display callback for the current window.
     When GLUT determines that the normal plane for the window needs to be redisplayed,
    the display callback for the window is called.*/
    glutKeyboardFunc(keyboardInput);
    glutMouseFunc(mouseInput);
    glutMotionFunc(continuousMouseInput);

    init();
    glutTimerFunc(1000 / FPS, timer, 0);
    /*GlutTimerFunc registers the timer callback func to be triggered in at least msecs milliseconds.
    The value parameter to the timer callback will be the value of the value parameter to glutTimerFunc. */
    glutMainLoop();
    /*glutMainLoop enters the GLUT event processing loop. This routine should be called at most once in a GLUT program.
    Once called, this routine will never return.
    It will call as necessary any callbacks that have been registered.*/

    return 0;
}


//https://github.com/realmar/game-of-life-cpp
