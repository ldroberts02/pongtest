#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>

SDL_Surface* Background = NULL;
SDL_Surface* Sprite = NULL;
SDL_Surface* Backbuffer = NULL;
SDL_Window* Window = NULL;

Mix_Chunk *chuck;

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

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    chuck = Mix_LoadWAV("assets/sounds/JuhaniJunkala[RetroGameMusicPack]TitleScreen.wav");
    std::cout << " YO " << Mix_PlayChannel( -1, chuck, -1 ) << std::endl;

    while(ProgramIsRunning())
    {
        SDL_Rect spritePos;
        spritePos.x = rand()%800;
        spritePos.y = rand()%600;

        SDL_BlitSurface(Sprite, NULL, Backbuffer, &spritePos);

        SDL_UpdateWindowSurface(Window);

        SDL_Delay(100);
    }

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