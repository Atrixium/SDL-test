// Using SDL2 and stdio
#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>

// Global Constants

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

// Forward Declarations

    // Starts up SDL and creates a window
    bool init();

    // Loads media
    bool loadMedia();

    //Frees media and shuts down SDL
    void kill();

// Gloabal variables

    // The window we will be rendering to
    SDL_Window* gWindow = NULL;

    //The surface contained by the window
    SDL_Surface* gScreenSurface = NULL;

    //The image we will load and show on the screen
    SDL_Surface* gHelloWorld = NULL;



int main(int argc, char* args[] )
{
    //Start SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //load media
        loadMedia();
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {

            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) !=0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }

                //Apply the image
                SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );

                //Update the surface
                SDL_UpdateWindowSurface( gWindow );

            }
        }
    }

    //Free resources and close SDL
    kill();

    return 0;

}

bool init()
{
    //Initiaslization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not intialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create Window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL)
        {
            printf( "Window could not be created!! SDL_Error:%s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load splash image
    gHelloWorld = SDL_LoadBMP( "images/hello_world.bmp" );
    if( gHelloWorld == NULL )
    {
        printf( "Unable to load image: %s!\nSDL_Error: %s\n", "images/hello_world.bmp", SDL_GetError() );
        success = false;
    }

    return success;
}

void kill()
{
    //Deallocate surface
    SDL_FreeSurface( gHelloWorld );
    gHelloWorld = NULL;

    //destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //quit SDL subsystems
    SDL_Quit();
}
