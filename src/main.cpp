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
const int SCREEN_HEIGHT = 800;

SDL_Window *window = nullptr;
SDL_Surface *sprite = nullptr;
SDL_Surface *spriteball = nullptr;
SDL_Surface *backGroundImage = nullptr;
SDL_Surface *backBuffer = nullptr;

Mix_Chunk *hitSound = nullptr;
Mix_Music *backGroundMusic = nullptr;

TTF_Font *gameFont = nullptr;

float inputDirectionX = 0.0f;
float inputDirectionY = 0.0f;
float movementSpeed = 10.0f;

float paddleXVel = 1.0f;
float paddleYVel = 1.0f;
float paddleMovementSpeed = 10.0f;

int intScore = 0;
int intscoresegment = 0;
float floatScore = 0.0f;

float spriteballXVel = 1.0f;
float spriteballYVel = 1.0f;
float spriteballMovementSpeed = 10.0f;

SDL_Rect paddleRect;
SDL_Rect ballRect;

bool noInput = true;
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
    std::cout << "test" << std::endl; //prints to terminal

    paddleRect.x = (SCREEN_WIDTH / 3);
    paddleRect.w = 128;
    paddleRect.h = 32;

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
        "Programming Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);
    
    backBuffer = SDL_GetWindowSurface(window);
    
    if (LoadFiles()) {

        //play sound
        //Mix_PlayChannel(-1,hitSound,0);
       // Mix_PlayMusic(backGroundMusic, -1); //UNCOMMENT FOR MUSIC


        while(ProgramIsRunning())
        {
            // get the time at the start of the frame
            int frameStart = SDL_GetTicks();
            // reset the back buffer with the back ground
            SDL_BlitSurface(backGroundImage, NULL, backBuffer, NULL);

            // draw the image
            paddleRect.x = (paddleRect.x + (inputDirectionX * movementSpeed));
            paddleRect.x = (paddleRect.x + (paddleRect.w) < SCREEN_WIDTH) ? (paddleRect.x) : SCREEN_WIDTH - paddleRect.w;
            paddleRect.x = (paddleRect.x > 0) ? paddleRect.x : 0;

            paddleRect.y = (SCREEN_HEIGHT - 32); // paddle does not need to move vertically, so this is used instead

            ballRect.x = (ballRect.x +  spriteballXVel);
            ballRect.y = (ballRect.y +  spriteballYVel);


            //ballRect.x = (paddleRect.x);
            //ballRect.y = (paddleRect.y - 200);

            DrawImage(sprite, backBuffer, paddleRect.x, paddleRect.y);

            DrawImage(spriteball, backBuffer, ballRect.x, ballRect.y);

            DrawImage(spriteball, backBuffer, ballRect.x, ballRect.y);

            DrawImage(spriteball, backBuffer, ballRect.x, ballRect.y);

            DrawImage(spriteball, backBuffer, ballRect.x, ballRect.y);

            // text rendering stuff
            std::string stringscore = "Score: " + std::to_string(intScore) + " BallVelX " + std::to_string(spriteballXVel);
            DrawText(backBuffer, stringscore.c_str(), 28, 28, gameFont, 255u, 255u, 255u);

            // end drawing frame
            SDL_UpdateWindowSurface(window);
            
            // find the number of milliseconds 
            int frameTime = SDL_GetTicks() - frameStart;

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
    inputDirectionX = 0.0f;
    inputDirectionY = 0.0f;

    // input buffer
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT])
        inputDirectionX = -1.0f;


    if (keys[SDL_SCANCODE_RIGHT])
        inputDirectionX = 1.0f;

    
    if (keys[SDL_SCANCODE_RETURN])
        noInput = !noInput;

    floatScore ++;
    intScore = floatScore / 60 ;

  //  while(noInput){
  //      paddleRect.x = ballRect.x; //this code crashes the game, ask why later
  //  }

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return false;
        if (event.type == SDL_KEYDOWN) {
            /*if (event.key.keysym.sym == SDLK_LEFT)
                inputDirectionX = -1.0f;
            if (event.key.keysym.sym == SDLK_RIGHT)
                inputDirectionX = 1.0f;*/
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
    backGroundImage = LoadImage("assets/graphics/new/background.bmp");
    sprite = LoadImage("assets/graphics/new/paddle.bmp");
    spriteball = LoadImage("assets/graphics/new/ball.bmp");

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
    hitSound = Mix_LoadWAV("assets/sounds/JuhaniJunkala[RetroGameMusicPack]TitleScreen.wav");

    if(hitSound == nullptr)
        return false;
    //load music
    backGroundMusic = Mix_LoadMUS("assets/sounds/JuhaniJunkala[RetroGameMusicPack]TitleScreen.wav");
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