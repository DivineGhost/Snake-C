#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#define HEIGHT 30
#define WIDTH 30

#define RIGHT 1
#define LEFT 2
#define UP 3
#define DOWN 4

#define MAX_TAIL 30

void game_over_screen();
void change_direction();
void setup();
void draw_map();
void draw_snake();
void erase_snake();
void spawn_food();
void gotoxy(int,int);
void format_prompt();
void hide_cursor();

typedef struct object
{
    int x,y,direction;
}OBJECT;

OBJECT snake,food,tail[MAX_TAIL]={};
int game_over=FALSE,size,score;

int main()
{
    setup();
    draw_map();
    int check_food=FALSE,i,j;
    do
    {
        if(kbhit())
            change_direction();
        draw_snake();
        if(!check_food)
        {
            spawn_food();
            check_food=TRUE;
        }
    if(snake.x == food.x && snake.y == food.y)
    {
        check_food=FALSE;
        size++;
    }
    for(i=0;i<size;i++)
    {
        if(snake.x == tail[i].x && snake.y == tail[i].y)
            game_over=TRUE;
    }
    score=score+5;
    gotoxy(0,0);
    printf("Score: %d",score);
    if(snake.x==0 || snake.x==WIDTH-1 || snake.y==0 || snake.y==HEIGHT-1)
        game_over=TRUE;
    Sleep(150);
    }while(!game_over);
    game_over_screen();
    return 0;
}

void game_over_screen()
{
    system("cls");
    FILE* file;
    int highscore=0;
    file=fopen("highscore.txt","r"); // Descobrir o modo bom de leitura
    if(!file)
    {
        gotoxy(10,10);
        printf("File 'highscore.txt' not found");
    }
    else
    {
        fread(&highscore,sizeof(int),1,file);
        if(score>highscore)
            highscore=score; // Atualizar score aqui
    }
    gotoxy(10,20);
    printf("Final Score: %d || Highscore: %d",score,highscore);
}

void change_direction()
{
    char key=toupper(getch());
    switch(key)
    {
    case 'A':
        if(snake.direction!=RIGHT)
            snake.direction=LEFT;
        break;
    case 'D':
        if(snake.direction!=LEFT)
            snake.direction=RIGHT;
        break;
    case 'S':
        if(snake.direction!=UP)
            snake.direction=DOWN;
        break;
    case 'W':
        if(snake.direction!=DOWN)
            snake.direction=UP;
        break;
    }
}

void setup()
{
    format_prompt();
    snake.x=floor(WIDTH/2);
    snake.y=floor(HEIGHT/2);
    snake.direction=RIGHT;
    size=1;
    score=0;
}

void draw_map()
{
    int i,j;
    for(i=0;i<WIDTH;i++)
    {
        for(j=0;j<HEIGHT;j++)
        {
            if(i==0 || i==WIDTH-1 || j==0 || j==HEIGHT-1)
                printf("#");
            else
                printf(" ");

        }
    if(i!=WIDTH-1)
        printf("\n");
    }
}

void draw_snake()
{
    erase_snake();
    int last_x,last_y,i;
    last_x=snake.x;
    last_y=snake.y;
    tail[size].x=last_x;
    tail[size].y=last_y;
    for(i=0;i<size;i++)
    {
        tail[i]=tail[i+1];
    }
    switch(snake.direction)
    {
    case RIGHT:
        snake.x++;
        break;
    case LEFT:
        snake.x--;
        break;
    case UP:
        snake.y--;
        break;
    case DOWN:
        snake.y++;
        break;
    }
    gotoxy(snake.x,snake.y);
    printf("O");
    for(i=0;i<size;i++)
    {
        gotoxy(tail[i].x,tail[i].y);
        printf("o");
    }
}

void erase_snake()
{
    gotoxy(snake.x,snake.y);
    printf(" ");
    int i;
    for(i=0;i<size;i++)
    {
        gotoxy(tail[i].x,tail[i].y);
        printf(" ");
    }
}

void spawn_food()
{
    srand(time(NULL));
    food.x= 1 + (rand() % (WIDTH-2)); // MIN + (rand() % (MAX - MIN + 1));
    food.y= 1 + (rand() % (HEIGHT-2));
    gotoxy(food.x,food.y);
    printf("F");
}

void gotoxy(int x, int y) // Move o cursor para (x,y)
{
    COORD coord = {0, 0};
    coord.X = x; // Coordenadas X e Y
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void format_prompt()
{
    // Formatação do prompt
    setlocale(LC_ALL, "Portuguese");                  // Língua para português
    system("title Snake");                            // Muda nome do prompt
    system("color 0C");                               // Cor normal do prompt
    SMALL_RECT WinRect = {0, 0, WIDTH-1, HEIGHT-1};   // Formata o tamanho do prompt
    SMALL_RECT* WinSize = &WinRect;
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), 1, WinSize);
    hide_cursor();
}

void hide_cursor() // Esconde o cursor default do prompt
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // Esconde o cursor do console
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
