#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>

SDL_Surface* Background = NULL;
SDL_Surface* Sprite = NULL;
SDL_Surface* Backbuffer = NULL;
SDL_Window* Window = NULL;

// Sounds
Mix_Chunk *chuck;

//Font
TTF_Font *GameFont = NULL;

bool ProgramIsRunning();
bool LoadImages();
void FreeImages();

int main(int argc, char* args[])
{

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL!!!", "SDL failed to initialize!", NULL);
        SDL_Quit();
        return 0;
    }

    Window = SDL_CreateWindow("SDL!!!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

    Backbuffer = SDL_GetWindowSurface(Window);

    if(!LoadImages())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL!!!", "Images failed to load!", NULL);
        SDL_DestroyWindow(Window);
        FreeImages();
        SDL_Quit();

        return 0;
    }

    SDL_BlitSurface(Background, NULL, Backbuffer, NULL );

    // intit SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    // load sound
    chuck = Mix_LoadWAV("assets/sounds/JuhaniJunkala[RetroGameMusicPack]TitleScreen.wav");
    // play sound
    Mix_PlayChannel( -1, chuck, -1 );

    // init TTF subsystem
    if(TTF_Init() == -1)
    {
        return 0;
    }

    // load font
    GameFont = TTF_OpenFont("assets/fonts/alfphabet.ttf", 30);

    while(ProgramIsRunning())
    {
        SDL_Rect spritePos;
        spritePos.x = rand()%800;
        spritePos.y = rand()%600;

        SDL_BlitSurface(Sprite, NULL, Backbuffer, &spritePos);

        // Render Font
        SDL_Surface* renderedText = NULL;

        SDL_Color color;

        color.r = 255u;
        color.g = 255u;
        color.b = 255u;

        renderedText = TTF_RenderText_Solid( GameFont, "Yoooooo!!!!!!!!!", color );

        SDL_Rect pos;

        pos.x = 100;
        pos.y = 100;

        SDL_BlitSurface( renderedText, NULL, Backbuffer, &pos );
        SDL_FreeSurface(renderedText);
        // End

        SDL_UpdateWindowSurface(Window);

        SDL_Delay(100);
    }

    //Free font
    TTF_CloseFont(GameFont);

    Mix_FreeChunk(chuck);

    SDL_DestroyWindow(Window);
    SDL_Quit();

    return 1;
}

bool ProgramIsRunning()
{
    SDL_Event event;

    bool running = true;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;
    }

    return running;
}

bool LoadImages()
{
    Background = SDL_LoadBMP("assets/graphics/background.bmp");

    if(Background == NULL)
        return false;

    Sprite = SDL_LoadBMP("assets/graphics/sprite.bmp");

    if(Sprite == NULL)
        return false;

    return true;
}

void FreeImages()
{
    if(Background != NULL)
    {
        SDL_FreeSurface(Background);
        Background = NULL;
    }

    if(Sprite != NULL)
    {
        SDL_FreeSurface(Sprite);
        Sprite = NULL;
    }
}