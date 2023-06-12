#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>


#define SCREEN_W 500
#define SCREEN_H 500

// 座標を動かす方向
enum DIRECTION
{
    DIR_RIGHT   = 0,
    DIR_UP      = 1,
    DIR_LEFT    = 2,
    DIR_DOWN    = 3
};

// ピクセルを表示する座標
struct Point
{
    int x;
    int y;
};


class Application
{
private:
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int quit_flg;

    SDL_Surface* image;
    SDL_Texture* image_texture;
    
    unsigned int anim;
    
    // 描画座標
    int x;
    int y;
    // 螺旋状に折り返すための目印
    int prev_right;
    int prev_left;
    int prev_up;
    int prev_down;
    // 描画する自然数の数, ピクセル数が上限
    unsigned int cur_num;
    unsigned int counter;

    // 座標が動く方向
    DIRECTION dir;

    // 座標を動かす
    void MoveCursor();
    
    // 座標をリセット
    void ResetCursor();

    // 描画
    void Draw();
    
    // 素数かどうかを調べる
    bool IsPrimeNumber(unsigned int num);

    // 素数フラグテーブル
    std::vector<bool> bPrime;
    std::vector<struct Point> point;
    

public:
    // コンストラクタ、デストラクタ
    Application();
    ~Application();

    // メインルーチン
    void Execute();

 
};
