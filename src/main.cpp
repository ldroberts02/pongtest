#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;


const int FPS = 60;
const int FRAME_DELAY = 1000/FPS;
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

SDL_Window *window = nullptr;
SDL_Surface *sprite = nullptr;
SDL_Surface *spriteball = nullptr;
SDL_Surface *backGroundImage = nullptr;
SDL_Surface *backBuffer = nullptr;

Mix_Chunk *hitSound = nullptr;
Mix_Music *backGroundMusic = nullptr;

TTF_Font *gameFont = nullptr;

float inputDirectionX = 0.0f;
float movementSpeed = 10.0f;

float paddleXVel = 1.0f;
float paddleYVel = 1.0f;
float paddleMovementSpeed = 10.0f;

int intScore = 0;
int highScore = 0;

int randX = rand() % 10 + 3;
int randneg = rand() % 2;
float spriteballXVel = 1.0f;
float spriteballYVel = 1.0f;
float spriteballMovementSpeed = 2.0f;

SDL_Rect paddleRect;
SDL_Rect ballRect;

bool isPlayer = true;
bool started = false;
bool paused = false;

bool LoadFiles();
void FreeFiles();
bool ProgramIsRunning();
SDL_Surface* LoadImage(const char* fileName);
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface, int x, int y, int width, int height, int frame);
void DrawText(SDL_Surface* surface, const char* string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b);
bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2);

int main(int argc, char* args[])
{
    paddleRect.x = (SCREEN_WIDTH / 3);
    paddleRect.w = 128;
    paddleRect.h = 16;

    ballRect.x = (SCREEN_WIDTH /2);
    ballRect.y = (SCREEN_HEIGHT / 2);
    ballRect.w = 20;
    ballRect.h = 20;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) //default initialization stuff
    {
        std::cout << "SDL failed to init!" << std::endl;
        SDL_Quit();
        return 1;
    }

    // load font
    if (TTF_Init() == -1)
        return 2;

    //load sdlmixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
        return 3;

    // create window
    window = SDL_CreateWindow(
        "Pong Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);
    
    backBuffer = SDL_GetWindowSurface(window);
    
    if (LoadFiles()) {

        //play sound
        Mix_PlayMusic(backGroundMusic, -1);


        while(ProgramIsRunning())
        {
            // get the time at the start of the frame
            int frameStart = SDL_GetTicks();
            // reset the back buffer with the back ground
            SDL_BlitSurface(backGroundImage, NULL, backBuffer, NULL);

            //initial setup of positions
            if(!started){
                paddleRect.x = (SCREEN_WIDTH /2) - 64;
                paddleRect.y = SCREEN_HEIGHT -32 ;
                ballRect.x = (SCREEN_WIDTH /2) -10;
                ballRect.y = (SCREEN_HEIGHT /2) -10;
                randX = rand() % - 10 + 6;
                randneg =rand() % 2;
                spriteballXVel = (randX);
                spriteballXVel = spriteballXVel /10;
                 if(randneg == 1){
                    spriteballXVel = spriteballXVel * -1;
                }
            }

            if(started & !paused){ //main loop for position calculating
                paddleRect.x = (paddleRect.x + (inputDirectionX * movementSpeed));
                paddleRect.x = (paddleRect.x + (paddleRect.w) < SCREEN_WIDTH) ? (paddleRect.x) : SCREEN_WIDTH - paddleRect.w;
                paddleRect.x = (paddleRect.x > 0) ? paddleRect.x : 0;

                paddleRect.y = (SCREEN_HEIGHT - 32); // paddle does not need to move vertically, so this is used instead

                ballRect.x = (ballRect.x +  (spriteballXVel * spriteballMovementSpeed));
                ballRect.y = (ballRect.y +  (spriteballYVel * spriteballMovementSpeed));
            }

            DrawImage(sprite, backBuffer, paddleRect.x, paddleRect.y);

            DrawImage(spriteball, backBuffer, ballRect.x, ballRect.y);

            DrawImage(spriteball, backBuffer, ballRect.x, ballRect.y);

            DrawImage(spriteball, backBuffer, ballRect.x, ballRect.y);

            DrawImage(spriteball, backBuffer, ballRect.x, ballRect.y);

            // text rendering stuff
            std::string stringscore ="Score " + std::to_string(intScore); //get frametime but make sure to check order of operations
            DrawText(backBuffer, stringscore.c_str(), 28, 28, gameFont, 255u, 255u, 255u);
            std::string stringHighscore = "High Score: " + std::to_string(highScore);
            if(!started){
                DrawText(backBuffer, stringHighscore.c_str(), 28, 56, gameFont, 255u, 255u, 255u);
            }

            // end drawing frame
            SDL_UpdateWindowSurface(window);
            
            // find the number of milliseconds 
            int frameTime = SDL_GetTicks() - frameStart;
            if(started && !paused){
            intScore = (frameTime + intScore);
            }
            // if we are rendering faster than FPS sleep the cpu
            if (frameTime < FRAME_DELAY)
                SDL_Delay(FRAME_DELAY - frameTime);
        }
    }


    FreeFiles();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

SDL_Surface* LoadImage(const char* fileName)
{
    SDL_Surface* imageLoaded = NULL;
    SDL_Surface* processedImage = NULL;

    imageLoaded = SDL_LoadBMP(fileName);

    if(imageLoaded != NULL)
    {
        processedImage = SDL_ConvertSurface(imageLoaded, backBuffer->format, 0);
        SDL_FreeSurface(imageLoaded);

        if(processedImage != NULL)
        {
            Uint32 colorKey = SDL_MapRGB(processedImage->format, 0xFF, 0, 0xFF);
            SDL_SetColorKey(processedImage, SDL_TRUE, colorKey);
        }
    }

    return processedImage;
}

bool ProgramIsRunning()
{
    SDL_Event event;
    if(isPlayer){
    inputDirectionX = 0.0f;
    }
    // input buffer
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if(isPlayer){
    if (keys[SDL_SCANCODE_LEFT])
        inputDirectionX = -1.0f;

    if (keys[SDL_SCANCODE_RIGHT])
        inputDirectionX = 1.0f;
    }


    if(ballRect.x >= SCREEN_WIDTH - 20 || ballRect.x <= 0){ //when ball hits wall x position
        spriteballXVel = spriteballXVel * -1; //inverts x velocity, thus making it move in the opposite X direction
          Mix_PlayChannel(-1,hitSound,0);
    }

    

    if(ballRect.y >= SCREEN_HEIGHT - 20){
            //Set score to 0
            //Re-init paddle and ball
            highScore = (isPlayer) ? intScore : highScore;
            intScore = 0.0;
            started = false;
            paused = false;
            spriteballMovementSpeed = 2.0f;
    }

//start of cpu stuff
    if(!isPlayer){
        if(started){
            if(ballRect.x >= paddleRect.x -20 && ballRect.x <= (paddleRect.x + paddleRect.w)){ //if ball is greater than px and less than pw its above it
                if(inputDirectionX > 0 || inputDirectionX < 0){
                    inputDirectionX = inputDirectionX * 0.9f;
                }
            }
            else if (ballRect.x <= paddleRect.x && ballRect.x <= (paddleRect.x + paddleRect.w)){ //if ball is less than px and less than pw then its to the left
                if(inputDirectionX >= -1.0f && inputDirectionX != 0.0f){
                    inputDirectionX = inputDirectionX - 1.0f;
                }
                else if (inputDirectionX == 0){
                     inputDirectionX = inputDirectionX - 0.01f;
                }
            }
            else if (ballRect.x >= paddleRect.x && ballRect.x >= (paddleRect.x + paddleRect.w)){ //if ball is greater than px and greater than pw its to the right
                if(inputDirectionX <= 1.0f && inputDirectionX != 0.0f){
                inputDirectionX = inputDirectionX + 1.0f;
                }
                else if (inputDirectionX == 0){
                     inputDirectionX = inputDirectionX + 0.01f;
                }
            }
        }

    }



    if(ballRect.y <= 0 || ballRect.y >= (paddleRect.y - paddleRect.h) && ballRect.y < SCREEN_HEIGHT -32 && (ballRect.x >= paddleRect.x -20 && ballRect.x <= (paddleRect.x + paddleRect.w))&& started & !paused ){ //when ball hits paddle in y position
        spriteballYVel = spriteballYVel * -1; //inverts y velocity, thus making it move in the opposite Y direction
         Mix_PlayChannel(-1,hitSound,0);
        spriteballMovementSpeed = spriteballMovementSpeed + 0.25 ;
       
    }



    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return false;
        if (event.type == SDL_KEYDOWN) {

            if(event.key.keysym.sym == SDLK_ESCAPE){
                break;
            }

            if (event.key.keysym.sym == SDLK_SPACE && event.key.repeat == 0){
                if(!started & !paused){
                isPlayer = true;
                started = true;
                break;
                }
            }
            if (event.key.keysym.sym == SDLK_p && event.key.repeat ==0 && started){
                paused = !paused;
                if(paused)
                    cout << "\n Paused";
                else if(!paused)
                    cout << "\n UnPaused";
                break;
            }
            if (event.key.keysym.sym == SDLK_a && event.key.repeat == 0){
                if(!started & !paused){
                    isPlayer = false;
                    started = true;
                    break;
                }
            }
            if (event.key.keysym.sym == SDLK_r && event.key.repeat ==0){
                if(started){
                    ballRect.y = SCREEN_HEIGHT +5;
                }
            }
        }
    

        if (event.type == SDL_MOUSEMOTION) {
            float x = event.motion.x;
            float y = event.motion.y;
        }
    }
    



    return true;
}

void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    SDL_BlitSurface( image, NULL, destSurface, &destRect);
}

void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface,
                    int x, int y, int width, int height, int frame)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    int collumns = (*image).w/width;

    SDL_Rect sourceRect;
    sourceRect.y = (frame/collumns)*height;
    sourceRect.x = (frame%collumns)*width;
    sourceRect.w = width;
    sourceRect.h = height;

    SDL_BlitSurface(image, &sourceRect, destSurface, &destRect);
}

bool LoadFiles()
{
    // load images
    backGroundImage = LoadImage("assets/graphics/background.bmp");
    sprite = LoadImage("assets/graphics/paddle.bmp");
    spriteball = LoadImage("assets/graphics/ball.bmp");

    if(sprite == nullptr)
        return false;

    if(backGroundImage == nullptr)
        return false;
    
    if(spriteball == nullptr)
        return false;
    // load font
    gameFont = TTF_OpenFont("assets/fonts/alfphabet.ttf", 30);

    if (gameFont == nullptr)
        return false;



    //load sounds
    hitSound = Mix_LoadWAV("assets/sounds/sfx_sounds_impact5.wav");

    if(hitSound == nullptr)
        return false;
    //load music
    backGroundMusic = Mix_LoadMUS("assets/sounds/srstrnc.wav");
    if (backGroundMusic == nullptr)

    return true;
}

void FreeFiles()
{
    if(sprite != nullptr)
    {
        SDL_FreeSurface(sprite);
        sprite = nullptr;
    }

    if(backGroundImage != nullptr)
    {
        SDL_FreeSurface(backGroundImage);
        backGroundImage = nullptr;
    }

    if (gameFont != nullptr)
    {
        TTF_CloseFont(gameFont);
        gameFont = nullptr;
    }
    if (hitSound != nullptr)
    {
        Mix_FreeChunk(hitSound);
        hitSound = nullptr;
    }
    if (backGroundMusic != nullptr)
    Mix_FreeMusic(backGroundMusic);
    backGroundMusic = nullptr;
}

void DrawText(SDL_Surface* surface, const char* string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Surface* renderedText = NULL;

    SDL_Color color;

    color.r = r;
    color.g = g;
    color.b = b;

    renderedText = TTF_RenderText_Solid( font, string, color );

    SDL_Rect pos;

    pos.x = x;
    pos.y = y;

    SDL_BlitSurface( renderedText, NULL, surface, &pos );
    SDL_FreeSurface(renderedText);
}

bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2)
{
    if(rect1.x >= rect2.x+rect2.w)
        return false;

    if(rect1.y >= rect2.y+rect2.h)
        return false;

    if(rect2.x >= rect1.x+rect1.w)
        return false;

    if(rect2.y >= rect1.y+rect1.h)
        return false;

    return true;
}