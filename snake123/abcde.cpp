#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int GRID_SIZE = 20;
const int INITIAL_LENGTH = 3;

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font;

struct Snake {
    int x, y;
};

Snake snake[100];
int length = INITIAL_LENGTH;
int fruitX, fruitY;
int dirX, dirY;
int score = 0;

void InitializeGame() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create window
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Load font
    font = TTF_OpenFont("/Users/akshitgoel/Desktop/snake123/OpenSans-Light.ttf", 24); // Change the path to your font file

    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize snake
    for (int i = 0; i < INITIAL_LENGTH; ++i) {
        snake[i].x = WIDTH / 2 - i * GRID_SIZE;
        snake[i].y = HEIGHT / 2;
    }

    // Initialize fruit position
    fruitX = rand() % (WIDTH / GRID_SIZE) * GRID_SIZE;
    fruitY = rand() % (HEIGHT / GRID_SIZE) * GRID_SIZE;

    // Set initial direction
    dirX = GRID_SIZE;
    dirY = 0;
}

void RenderSnake() {
    for (int i = 0; i < length; ++i) {
        SDL_Rect rect = {snake[i].x, snake[i].y, GRID_SIZE, GRID_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
        SDL_RenderFillRect(renderer, &rect);
    }
}

void RenderFruit() {
    SDL_Rect rect = {fruitX, fruitY, GRID_SIZE, GRID_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_RenderFillRect(renderer, &rect);
}

void RenderScore() {
    SDL_Color textColor = {255, 255, 255, 255}; // White color

    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    SDL_Rect textRect = {10, 10, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void HandleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(EXIT_SUCCESS);
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (dirY == 0) {
                        dirX = 0;
                        dirY = -GRID_SIZE;
                    }
                    break;
                case SDLK_DOWN:
                    if (dirY == 0) {
                        dirX = 0;
                        dirY = GRID_SIZE;
                    }
                    break;
                case SDLK_LEFT:
                    if (dirX == 0) {
                        dirX = -GRID_SIZE;
                        dirY = 0;
                    }
                    break;
                case SDLK_RIGHT:
                    if (dirX == 0) {
                        dirX = GRID_SIZE;
                        dirY = 0;
                    }
                    break;
            }
        }
    }
}

void Update() {
    // Move the snake
    for (int i = length - 1; i > 0; --i) {
        snake[i] = snake[i - 1];
    }
    snake[0].x += dirX;
    snake[0].y += dirY;

    // Check for collision with the walls
    if (snake[0].x < 0 || snake[0].x >= WIDTH || snake[0].y < 0 || snake[0].y >= HEIGHT) {
        exit(EXIT_SUCCESS);
    }

    // Check for collision with itself
    for (int i = 1; i < length; ++i) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            exit(EXIT_SUCCESS);
        }
    }

    // Check for collision with the fruit
    if (snake[0].x == fruitX && snake[0].y == fruitY) {
        // Increase the length of the snake
        ++length;

        // Increase the score
        ++score;

        // Generate a new fruit position
        fruitX = rand() % (WIDTH / GRID_SIZE) * GRID_SIZE;
        fruitY = rand() % (HEIGHT / GRID_SIZE) * GRID_SIZE;
    }
}

void Render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color
    SDL_RenderClear(renderer);

    RenderSnake();
    RenderFruit();
    RenderScore();

    // Update the screen
    SDL_RenderPresent(renderer);
}

void Cleanup() {
    // Clean up SDL_ttf
    TTF_CloseFont(font);
    TTF_Quit();

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    InitializeGame();

    while (true) {
        HandleInput();
        Update();
        Render();
        SDL_Delay(100); // 100 milliseconds delay
    }

    Cleanup();

    return 0;
}