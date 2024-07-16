#include "stdio.h"
#include "ctype.h"
#include "graphics.h"
#include "time.h"
#include "pthread.h"
#include "windows.h"

#define ROW 24
#define COL 30
#define SIZE 22

char map[ROW + 2][COL + 2] = {0};
char show[ROW + 2][COL + 2] = {0};

IMAGE img[16];

int counter_sec = 0;
int flag1 = 0;
int fg1 = 0,fg2 = 0;

int N,M,C;
int remain;

void background()
{
    putimage(0,0,&img[14]);
    putimage(1*SIZE,1*SIZE,&img[9]);
    putimage((N-3)*SIZE,1*SIZE,&img[15]);

        setbkmode(TRANSPARENT);
        settextcolor(BLACK);
    settextstyle(20, 10, _T("Consolas"));
    TCHAR s[5] = {0};
    _stprintf(s, _T("%d"), C);
    outtextxy(2*SIZE,1*SIZE,s);
}

void *pthread_fun(void *args)
{
    for(int i = counter_sec; i >= 0; i++)
    {
        background();
        TCHAR m[10] = {0};
        _stprintf(m,_T("%d"),i);
        outtextxy((N-2)*SIZE,1*SIZE,m);
        Sleep(970);
        if(flag1 == 5)
        {
            TCHAR m[10] = {0};
            _stprintf(m,_T("%d"),i);
            outtextxy((N-2)*SIZE,1*SIZE,m);
            break;
        }
    }
}

void Game_Initial()
{
    srand((unsigned int)time(NULL));
    int n = 0;
    for(int i = 0; i < N + 2; i++)
    {
        for(int j = 0; j < M + 2; j++)
        {
            map[i][j] = '.';
        }
    }
    while(n < C)
    {
        int r = rand() % N + 1;
        int c = rand() % M + 1;
        if(map[r][c] == '.')
        {
            map[r][c] = '*';
            n++;
        }
    }
    for(int i = 1; i <= N; i++)
    {
        for(int j = 1; j <= M; j++)
        {
            show[i][j] = '.';
        }
    }
}

int Judge_Once()
{
    for(int i = 1; i <= N; i++)
    {
        for(int j = 1; j <= M; j++)
        {
            if(show[i][j] != '.')
            {
                return 1;
            }
        }
    }
    return 0;
}

void Right_Click(int z,int x,int y)
{
    if(z == 2)
    {
        if(show[x][y] == '.' || show[x][y] == '?')
        {
            show[x][y] = '!';
            C--;
        }
    }
    else if(z == 3)
    {
        if(show[x][y] == '.')
        {
            show[x][y] = '?';
        }
        if(show[x][y] == '!')
        {
            show[x][y] = '?';
            C++;
        }
    }
    else if(z == 4)
    {
        if(show[x][y] == '?')
        {
            show[x][y] = '.';
        }
        if(show[x][y] == '!')
        {
            show[x][y] = '.';
            C++;
        }
    }
}

void Print_Game(int n)
{
    for(int i = 1; i <= N; i++)
    {
        for(int j = 1; j <= M; j++)
        {
            if(n == 5 && map[i][j] == '*')
            {
                printf("*");
                show[i][j] = '*';
            }
            else if(n == 0)
            {
                printf("%c",show[i][j]);
            }
            else
            {
                printf("%c",map[i][j]);
            }

            if(show[i][j] == '*')
            {
                putimage((i-1)*SIZE,(j+2)*SIZE,&img[9]);
            }
            else if(show[i][j] >= '0' && show[i][j] <= '8')
            {
                putimage((i-1)*SIZE,(j+2)*SIZE,&img[(int)map[i][j] - '0']);
            }
            else if(show[i][j] == '.')
            {
                putimage((i-1)*SIZE,(j+2)*SIZE,&img[10]);
            }
            else if(show[i][j] == '!')
            {
                putimage((i-1)*SIZE,(j+2)*SIZE,&img[11]);
            }
            else if(show[i][j] == '?')
            {
                putimage((i-1)*SIZE,(j+2)*SIZE,&img[12]);
            }
            else if(show[i][j] == '/')
            {
                putimage((i-1)*SIZE,(j+2)*SIZE,&img[13]);
            }
        }
        printf("\n");
    }
}

int Left_Click(int x,int y)
{
    char Count_Bombs = '0';
    if(x < 1 || x > N || y < 1 || y > M || isdigit(map[x][y]) || show[x][y] == '!')
    {
        return 1; //continue
    }
    if(map[x][y] == '*')
    {
        if (Judge_Once() == 1)
        {
            return 0;
        }
        else
        {
            Game_Initial();
            Print_Game(1);
        }
    }
    for(int i = x - 1; i <= x + 1; i++)
    {
        for(int j = y - 1; j <= y + 1; j++)
        {
            if(i >= 1 && i <= N && j >= 1 && j <= M)
            {
                if(map[i][j] == '*')
                {
                    Count_Bombs++;
                }
            }
        }
    }
    map[x][y] = Count_Bombs;
    show[x][y] = Count_Bombs;

    remain--;

    if(Count_Bombs == '0')
    {
        for(int i = x - 1; i <= x + 1; i++)
        {
            for(int j = y - 1; j <= y + 1; j++)
            {
                if(i >= 1 && i <= N && j >= 1 && j <= M) {
                    if(i != x || j != y) {
                        Left_Click(i, j);
                    }
                }
            }
        }
    }
    return 1; //continue
}

int Double_Click(int x,int y)
{
    int Count_Marks = 0, Count_Bomb = 0;
    int flag = 0;
    for(int i = x - 1; i <= x + 1; i++)
    {
        for(int j = y - 1; j <= y + 1; j++)
        {
            if(show[i][j] == '!')
            {
                Count_Marks++;
            }
            if(map[i][j] == '*')
            {
                Count_Bomb++;
            }
            if(map[i][j] != '*' && show[i][j] == '!')
            {
                flag = 1;
            }
        }
    }
    if(Count_Bomb == Count_Marks)
    {
        if(flag == 1)
        {
            for(int i = x - 1; i <= x + 1; i++)
            {
                for(int j = y - 1; j <= y + 1; j++)
                {
                    if(map[i][j] != '*' && show[i][j] == '!')
                    {
                        show[i][j] = '/';
                    }
                }
            }
            return 0;
        }
        else
        {
            for(int i = x - 1; i <= x + 1; i++)
            {
                for(int j = y - 1; j <= y + 1; j++)
                {
                    if(map[i][j] != '*')
                    {
                        Left_Click(i,j);
                    }
                }
            }
            return 1;
        }
    }
    else return 1;
}

int Judge_Rec(MOUSEMSG msg,int x,int y,int w,int h)
{
    if(msg.x > x && msg.x < (x+w) && msg.y > y && msg.y < (y+h))
    {
        return 1;
    }
    else return 0;
}

void Menu()
{
    IMAGE menu[2];
    loadimage(&menu[0],"../mine1.jpg",960,580);
    loadimage(&menu[1],"../mine2.jpg",960,580);

    initgraph(960,580,SHOWCONSOLE);
    putimage(0,0,&menu[0]);

    while(fg1 == 0)
    {
        MOUSEMSG msg1 = {0};
        msg1 = GetMouseMsg();
        if(msg1.uMsg == WM_LBUTTONDOWN)
        {
            if(Judge_Rec(msg1,70,285,375,75) == 1)
            {
                fg1 = 1;
                putimage(0,0,&menu[1]);
            }
            else if(Judge_Rec(msg1,70,390,230,65) == 1)
            {
                fg1 = 0;
                exit(-1);
            }
        }
    }
    while(fg2 == 0)
    {
        MOUSEMSG msg2 = {0};
        msg2 = GetMouseMsg();
        if(msg2.uMsg == WM_LBUTTONDOWN) {
            if (Judge_Rec(msg2, 700, 65, 235, 65) == 1) {
                fg2 = 1;
                closegraph();
            } else if (Judge_Rec(msg2, 690, 170, 240, 70) == 1) {
                fg2 = 2;
                closegraph();
            } else if (Judge_Rec(msg2, 695, 275, 235, 70) == 1) {
                fg2 = 3;
                closegraph();
            } else if (Judge_Rec(msg2, 690, 380, 240, 70) == 1) {
                fg2 = 4;
                closegraph();
            }
            printf("%d",fg2);
        }
        else if(msg2.uMsg == WM_RBUTTONDOWN)
        {
            fg1 = 0;
            putimage(0,0,&menu[0]);

            while(fg1 == 0)
            {
                MOUSEMSG msg1 = {0};
                msg1 = GetMouseMsg();
                if(msg1.uMsg == WM_LBUTTONDOWN)
                {
                    if(Judge_Rec(msg1,70,285,375,75) == 1)
                    {
                        fg1 = 1;
                        putimage(0,0,&menu[1]);
                    }
                    else if(Judge_Rec(msg1,70,390,230,65) == 1)
                    {
                        fg1 = 0;
                        exit(-1);
                    }
                }
            }
        }
    }
}

int main()
{
    loadimage(&img[0],"../0.jpg",SIZE,SIZE);
    loadimage(&img[1],"../1.jpg",SIZE,SIZE);
    loadimage(&img[2],"../2.jpg",SIZE,SIZE);
    loadimage(&img[3],"../3.jpg",SIZE,SIZE);
    loadimage(&img[4],"../4.jpg",SIZE,SIZE);
    loadimage(&img[5],"../5.jpg",SIZE,SIZE);
    loadimage(&img[6],"../6.jpg",SIZE,SIZE);
    loadimage(&img[7],"../7.jpg",SIZE,SIZE);
    loadimage(&img[8],"../8.jpg",SIZE,SIZE);
    loadimage(&img[9],"../bomb.jpg",SIZE,SIZE);
    loadimage(&img[10],"../grey.jpg",SIZE,SIZE);
    loadimage(&img[11],"../flag.jpg",SIZE,SIZE);
    loadimage(&img[12],"../ques.jpg",SIZE,SIZE);
    loadimage(&img[13],"../cross.jpg",SIZE,SIZE);
    loadimage(&img[15],"../counter.jpg",SIZE,SIZE);

    Menu();
//    int n;
    int count = 0;
//    scanf("%d",&n);
    if(fg2 == 1)
    {
        N = 9;
        M = 9;
        C = 10;
    }
    else if(fg2 == 2)
    {
        N = 16;
        M = 16;
        C = 40;
    }
    else if(fg2 == 3)
    {
        N = 16;
        M = 30;
        C = 99;
    }
    else if(fg2 == 4)
    {
        scanf("%d %d %d",&N,&M,&C);
    }

    loadimage(&img[14],"../back.jpg",N*SIZE,3*SIZE);

        HWND hwnd = initgraph(N * SIZE, (M + 3) * SIZE);

        Game_Initial();

        Print_Game(fg2);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, pthread_fun, NULL);
        // Counter

        remain = N * M - C;

        MOUSEMSG msg = {0};
        msg = GetMouseMsg();
        while (remain > 0) {
            int z, x, y;
            msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) // Left Click
            {
                x = msg.x / SIZE + 1;
                y = msg.y / SIZE - 2;
                z = 1;
            } else if (msg.uMsg == WM_MBUTTONDOWN) // Chording
            {
                x = msg.x / SIZE + 1;
                y = msg.y / SIZE - 2;
                z = 9;
            } else if (msg.uMsg == WM_RBUTTONDOWN) // Right Click
            {
                x = msg.x / SIZE + 1;
                y = msg.y / SIZE - 2;
                if (show[x][y] == '.') {
                    z = 2;
                }
                if (show[x][y] == '!') {
                    z = 3;
                }
                if (show[x][y] == '?') {
                    z = 4;
                }
            }
//        scanf("%d %d %d",&z,&x,&y);
            count++;
            if (x >= 1 && x <= N && y >= 1 && y <= M) {
                int tmp;
                if (z == 1) {
                    tmp = Left_Click(x, y);
                } else if (z == 9) {
                    tmp = Double_Click(x, y);
                } else {
                    Right_Click(z, x, y);
                    tmp = 1;
                }

                if (tmp == 0) {
                    printf("Hit mine, you lose\n");
                    flag1 = 5;
                    Print_Game(flag1);
                    MessageBox(hwnd, "Hit mine, you lose", "", MB_OK);
                } else if (remain == 0) {
                    printf("Game over, you win\n");
                    flag1 = 5;
                    Print_Game(flag1);
                    MessageBox(hwnd, "Game over, you win", "", MB_OK);
                } else {
                    printf("Game in progress\n");
                    Print_Game(flag1);
                }
            }
            printf("%d %d\n", count, C);
            if (flag1 == 5) {
                break;
            }
        }
        closegraph();
    return 0;
}


