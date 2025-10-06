#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20
#define BLOCK 30

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Game Dua Xe - SDL3",
                                       WIDTH * BLOCK,
                                       HEIGHT * BLOCK,
                                       0);
    if (!win) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, NULL);
    if (!ren) {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    srand((unsigned int)time(NULL));

    int carX = WIDTH / 2;
    int obstacleX = rand() % WIDTH;
    float obstacleY = 0;
    int score = 0;
    int running = 1;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = 0;
            else if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_LEFT && carX > 0)
                    carX--;
                else if (e.key.key == SDLK_RIGHT && carX < WIDTH - 1)
                    carX++;
            }
        }

        obstacleY += 0.15f;
        if (obstacleY > HEIGHT) {
            obstacleY = 0;
            obstacleX = rand() % WIDTH;
            score++;
        }

        // kiểm tra va chạm
        if ((int)obstacleY == HEIGHT - 1 && obstacleX == carX) {
            SDL_Log("Game Over! Score = %d", score);
            running = 0;
        }

        // vẽ khung
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // vẽ xe (màu xanh lá)
        SDL_FRect car = {carX * BLOCK, (HEIGHT - 1) * BLOCK, BLOCK, BLOCK};
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
        SDL_RenderFillRect(ren, &car);

        // vẽ chướng ngại vật (màu đỏ)
        SDL_FRect obs = {obstacleX * BLOCK, obstacleY * BLOCK, BLOCK, BLOCK};
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_RenderFillRect(ren, &obs);

        SDL_RenderPresent(ren);
        SDL_Delay(16); // ~60fps
    }
// Khi game kết thúc, hiển thị "Game Over" trong cửa sổ
SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
SDL_RenderClear(ren);

// Vẽ chữ đơn giản bằng SDL_Log (nếu chưa dùng font)
SDL_Log("GAME OVER! Your score: %d", score);

// Tạm dừng để người chơi đọc kết quả
SDL_RenderPresent(ren);
SDL_Delay(3000);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    SDL_system_h_( "pause");
}