#include "Application.h"
#include  <iostream>
#include <ctime>
#include <cstdlib>

// 初期化
Application::Application()
    : counter(0)
    , quit_flg(1)
    , anim(0)
{
    // SDL初期化
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL_Init(): " << SDL_GetError() << std::endl;
    }
        
    // ウインドウ生成
    window = SDL_CreateWindow("Prime Number", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL);
    if(window == NULL)
    {
        std::cout << "Can not create window" << std::endl;
    }
    // レンダラー生成
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL )
    {
        std::cout << "Can not create renderer" << std::endl;
    }

    // 描画する座標をリセット
//    ResetCursor();

    // 素数テーブルを作る
    for(auto i = 0; i < SCREEN_H*SCREEN_W; i++)
    {
        bPrime.push_back(IsPrimeNumber(i));
    }
    
    
    
    // SDL2_imageを初期化
    IMG_Init(IMG_INIT_PNG);
    // イメージをロードする
    image = IMG_Load("Assets/1-1.png");
    // テクスチャ化
    image_texture = SDL_CreateTextureFromSurface(renderer, image);
    
    // 乱数テーブルを初期化
    std::srand((unsigned int)time(NULL));

}

// デストラクタ
Application::~Application()
{

    // imegeとTextureを解放
    if(image) SDL_FreeSurface(image);
    if(image_texture) SDL_DestroyTexture(image_texture);
    
    // 素数フラグを解放
    bPrime.clear();
    
    // リリース処理
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    SDL_Quit();
}

// 素数かどうかを調べる
bool Application::IsPrimeNumber(unsigned int num)
{
    for(auto i = 2; i < num / 2; i++)
    {
        if(num % i == 0)
        {
            return false;
        }
    }
    return true;
}


// 描画座標を螺旋状に動かす
void Application::MoveCursor()
{
    switch(dir)
    {
    case DIR_RIGHT:
        x++;
        if(x > prev_right)
        {
            prev_right = x;
            dir = DIR_UP;
        }
        break;
    case DIR_UP:
        y--;
        if(y < prev_up)
        {
            prev_up = y;
            dir = DIR_LEFT;
        }
        break;
    case DIR_LEFT:
        x--;
        if(x < prev_left)
        {
            prev_left = x;
            dir = DIR_DOWN;
        }
        break;
    case DIR_DOWN:
        y++;
        if(y > prev_down)
        {
            prev_down = y;
            dir = DIR_RIGHT;
        }
        break;
    }
}



// 描画座標をリセット
void Application::ResetCursor()
{
    x = prev_left = prev_right = SCREEN_W /2;
    y = prev_down = prev_up = SCREEN_H / 2;
    dir = DIR_RIGHT;
}

// 描画
void Application::Draw()
{
    // 背景クリア
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
/*
    // ピクセル数を上限
    if(cur_num < SCREEN_H*SCREEN_W)
    {
        if(cur_num < 10)
        {
            cur_num++;
        }
        else
        {
            cur_num += cur_num / 8;
        }
    } 
    else
    {
        cur_num = SCREEN_H*SCREEN_W;
    }
*/
    ResetCursor();
    // 素数の場所に点を打つ
    //for(auto i = 0; i < cur_num; i++)
    for(auto i = 0; i < SCREEN_H*SCREEN_W; i++)
    {

        if(bPrime[i])
        {
  
            // ランダムに色を明るくする
            auto r =  rand() % 8;
            SDL_SetRenderDrawColor(renderer, 0, 20 + r*10, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, x, y);
            // 上と下も追加して縦長にする（Line命令にしても良い）
            SDL_SetRenderDrawColor(renderer, 0, 10 + r*5, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, x, y-1);
            SDL_RenderDrawPoint(renderer, x, y+1);

            // テクスチャの該当座標の色を加算
            SDL_QueryTexture(image_texture, NULL, NULL, NULL, NULL);
            SDL_Rect imageRect=(SDL_Rect)   {(int)anim + x,y,1,1};
            SDL_Rect drawRect=(SDL_Rect)    {x,y,1,1};
            SDL_SetTextureBlendMode(image_texture, SDL_BLENDMODE_ADD);
            SDL_RenderCopy(renderer, image_texture, &imageRect, &drawRect);
            SDL_RenderCopy(renderer, image_texture, &imageRect, &drawRect);

        }

        // 螺旋状に座標を動かす
        MoveCursor();
    }

    
    // 描画
    SDL_RenderPresent(renderer);

}

void Application::Execute()
{
    // メインループ
    while(quit_flg) 
    {
        // 画面描画
        Draw();
        
        // ぱらぱら漫画のコマを選ぶ
        counter++;
        if(counter % 1 == 0)
        {
            anim += 20;
            if(anim >= 7500)
            {
                anim = 0;
            }
        }
        
        // アプリ終了条件
        while( SDL_PollEvent(&event) )
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
            case SDL_QUIT:
                quit_flg = 0;
                break;
            }
        }
    }
}
