/*
####################################################
|    ________  ________  ________   ________       |
|   |\   __  \|\   __  \|\   ___  \|\   ____\      |
|   \ \  \|\  \ \  \|\  \ \  \\ \  \ \  \___|      |
|    \ \   ____\ \  \\\  \ \  \\ \  \ \  \  ___    |
|     \ \  \___|\ \  \\\  \ \  \\ \  \ \  \|\  \   |
|      \ \__\    \ \_______\ \__\\ \__\ \_______\  |
|       \|__|     \|_______|\|__| \|__|\|_______|  |    
####################################################    
*/
#include "raylib.h"
#include <iostream>


//Window
#define WINDOW_WIDTH 1200
#define WINDOW_HIGHT 700

//Court
#define COURT_TOP WINDOW_HIGHT - WINDOW_HIGHT*0.85
#define COURT_BOTTOM WINDOW_HIGHT - WINDOW_HIGHT*0.1
#define COURT_CENTER_X WINDOW_WIDTH/2
#define COURT_CENTER_Y (((COURT_BOTTOM)- (COURT_TOP))/2)+(COURT_TOP)
#define DOTS_HIGHT 10
#define DOTS_WIDTH 3


using namespace std;

struct PLAYER
{
    int Score = 0;
    float Speed = 500;
    float Hight = WINDOW_HIGHT * 0.3f;
    float Width = WINDOW_WIDTH * 0.025f;
    float X , Y = COURT_CENTER_Y - Hight/2;
    KeyboardKey UpKey,DownKey;
    Rectangle Rectangle;
    //------------------------------------------------------
    void Draw()
    {
        Rectangle = {X ,Y ,Width ,Hight};
        DrawRectangleRec(Rectangle ,WHITE);
    }
    //------------------------------------------------------
    void Move()
    {
        if (IsKeyDown(UpKey) && Y > COURT_TOP)
        {
            Y -= Speed * GetFrameTime();
        }
        if (IsKeyDown(DownKey) && (Y+Hight) < COURT_BOTTOM)
        {
            Y += Speed * GetFrameTime();
        }
    }
};
struct BALL
{
    float X,Y;
    float SpeedX,SpeedY;
    float Size = 5;
    bool Start = false;
    //------------------------------------------------------
    void Draw()
    {
        DrawCircle((int)X ,(int)Y ,Size ,WHITE);
    }
    //------------------------------------------------------
    void Move()
    {
        if (Y < COURT_TOP)
        {
            Y = COURT_TOP;
            SpeedY *= -1;
        }
        if (Y > COURT_BOTTOM)
        {
            Y = COURT_BOTTOM;
            SpeedY *= -1;
        }
        if (Start)
        {
            X += SpeedX * GetFrameTime();
            Y += SpeedY * GetFrameTime();
        }  
    }
    //------------------------------------------------------
    void Collision(PLAYER pLeft,PLAYER pRight)
    {
        if (CheckCollisionCircleRec(Vector2{X,Y},Size,pRight.Rectangle))
        {
            if (SpeedX > 0)
            {
                SpeedX *= -1;
            }
        }
        if (CheckCollisionCircleRec(Vector2{X,Y},Size,pLeft.Rectangle))
        {
            if (SpeedX < 0)
            {
                SpeedX *= -1;
            }
        }
    }
    //------------------------------------------------------
};
struct Tip
{
    Color DrawColor = WHITE;
    void IfDone(KeyboardKey Key , KeyboardKey Key2)
    {
        if(IsKeyPressed(Key)||IsKeyPressed(Key2))
        {
            DrawColor = DARKGRAY;
        }
    }
    void Draw(string Text ,int X , int Y , int Size)
    {
        DrawText(TextFormat(Text.c_str()) ,X ,Y ,Size ,DrawColor);
    }
};

struct Visual
{
    int X= 2, Y = COURT_TOP;
    Rectangle hcDots[(int)(WINDOW_HIGHT - WINDOW_HIGHT*0.62)/DOTS_HIGHT];
    Rectangle hp1Dots[WINDOW_HIGHT/DOTS_HIGHT];
    Rectangle hp2Dots[WINDOW_HIGHT/DOTS_HIGHT];
    Rectangle vtDots[WINDOW_WIDTH/DOTS_HIGHT];
    Rectangle vbDots[WINDOW_WIDTH/DOTS_HIGHT];
    void SetGrid(PLAYER lPlayer , PLAYER rPlayer)
    {
        for (int i = 0; i < WINDOW_WIDTH/DOTS_HIGHT; i++)
        {
            vtDots[i] = {(float)X ,COURT_TOP ,DOTS_HIGHT ,DOTS_WIDTH};
            vbDots[i] = {(float)X ,COURT_BOTTOM ,DOTS_HIGHT ,DOTS_WIDTH};
            X += DOTS_HIGHT * 2;
        }
        for (int i = 0; i < (WINDOW_HIGHT - WINDOW_HIGHT*0.62)/DOTS_HIGHT; i++)
        {
            hcDots[i] = {COURT_CENTER_X ,(float)Y ,DOTS_WIDTH ,DOTS_HIGHT};
            hp1Dots[i] = {(lPlayer.X + lPlayer.Width/2) ,(float)Y ,DOTS_WIDTH ,DOTS_HIGHT};
            hp2Dots[i] = {(rPlayer.X + rPlayer.Width/2) ,(float)Y ,DOTS_WIDTH ,DOTS_HIGHT};
            Y += DOTS_HIGHT * 2;
        }
        
    }
    void DrawGrid()
    {
        for (int i = 0; i < WINDOW_WIDTH/DOTS_HIGHT; i++)
        {
            DrawRectangleRec(vtDots[i],WHITE);
            DrawRectangleRec(vbDots[i],WHITE);
        }
        for (int i = 0; i < (WINDOW_HIGHT - WINDOW_HIGHT*0.62)/DOTS_HIGHT; i++)
        {
            DrawRectangleRec(hcDots[i],DARKGRAY);
            DrawRectangleRec(hp1Dots[i],WHITE);
            DrawRectangleRec(hp2Dots[i],WHITE);
        }
    }
    

    Tip Start;
    Tip lPlayer;
    Tip rPlayer;
    void DrawTips()
    {
        Start.Draw("PRESS SPACE OR ENTER TO START",400,650,20);
        lPlayer.Draw("PRESS: W FOR UP , S FOR DOWN",30,650,15);
        rPlayer.Draw("PRESS: UP FOR UP , DOWN FOR DOWN",880,650,15);
    }
};

int main() 
{
    {//* Set Main Window *//
        InitWindow(WINDOW_WIDTH,WINDOW_HIGHT,"PONG");
        SetWindowState(FLAG_VSYNC_HINT);
        SetWindowIcon(LoadImage("Pong.png"));
    }//*-----------------*//

    // Set Game
        // Set Ball
            BALL Ball;
            Ball.X = COURT_CENTER_X;
            Ball.Y = COURT_CENTER_Y;
            Ball.SpeedX = 200;        
            Ball.SpeedY = 200;
        //--------
        // Set Player 1 (left)
            PLAYER Player1;
            Player1.X = (int)WINDOW_WIDTH * 0.05f;
            Player1.UpKey = KEY_W;
            Player1.DownKey = KEY_S;
        //--------
        // Set Player 2 (right)
            PLAYER Player2;
            Player2.X = (int)WINDOW_WIDTH - Player2.Width - WINDOW_WIDTH * 0.05f;
            Player2.UpKey = KEY_UP;
            Player2.DownKey = KEY_DOWN;
        //--------
        // Set Grid
            Visual MainVisual;
            MainVisual.SetGrid(Player1,Player2);
        //--------    
    //--------

    //* Game Loop
    while (!WindowShouldClose())
    {
        //Move Ball
            Ball.Move();
            Ball.Collision(Player1,Player2);
            if (IsKeyPressed(KEY_SPACE)) Ball.Start = true;
        //--------
        //Point
            if (Ball.X < 0 || Ball.X > WINDOW_WIDTH)
            {
                if (Ball.X < 0) Player2.Score++;  
                if (Ball.X > WINDOW_WIDTH) Player1.Score++;      
                Ball.X = COURT_CENTER_X;
                Ball.Y = COURT_CENTER_Y;
                Ball.SpeedX *= -1;
                Ball.Start = false;
                Player1.Y = COURT_CENTER_Y - Player1.Hight/2;
                Player2.Y = COURT_CENTER_Y - Player1.Hight/2;
            }
        //--------
        //Control players
            Player1.Move();
            Player2.Move();
            MainVisual.Start.IfDone(KEY_SPACE,KEY_ENTER);
            MainVisual.lPlayer.IfDone(KEY_W,KEY_S);
            MainVisual.rPlayer.IfDone(KEY_UP,KEY_DOWN);
        //--------

        //* Show On Screen
        BeginDrawing();
        {
            ClearBackground(BLACK);
            Player1.Draw();
            Player2.Draw();
            MainVisual.DrawTips();
            MainVisual.DrawGrid();
            Ball.Draw();
            DrawText(TextFormat("P1:%d",Player1.Score),300,30,50,WHITE);
            DrawText(TextFormat("P2:%d",Player2.Score),800,30,50,WHITE);
            DrawText(TextFormat("FPS:%d",GetFPS()),10,10,20,WHITE);
        }
        EndDrawing();
    }

    {//! Close The Main Window !//
        CloseWindow();
        return 0;
    }//!----------------------!//
}