#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> //For PNG support
#include <stdio.h> //For printf
#include <string>

//Global constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//Global variables
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//Forward declarations

bool init();

//Classes

class LTexture // LTexture == Lazy Texture, change this convention with a better one
{
public:
    LTexture(); //Constructor - Used to initialize mVariables - Look these up to see what else we can do with them
    ~LTexture(); //Destructor - Used to deallocate pointers - Look these up to see what else we can do with them

    void free(); // Deallocates texture
    bool LoadTexture(std::string path); //Load a texture from a file, return false if fail
    void Render(int x, int y); //Renders texture at X and Y

    int getWidth();
    int getHeight();

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

LTexture::LTexture()
{
    mTexture = nullptr;
    mWidth = 0;
    mWidth = 0;
}

LTexture::~LTexture()
{
    free();
}

void LTexture::free()
{
    SDL_DestroyTexture( mTexture );
    mTexture = nullptr;
}

bool LTexture::LoadTexture(std::string path)
{
    bool success = true;
    SDL_Surface* image = NULL;
    image = IMG_Load( path.c_str() );
    if(!image)
    {
        printf( "Could not load image %s! IMG_Error:%s\n", path.c_str(), IMG_GetError() );
        success = false;
    }
    else
    {
        mWidth = image->w;
        mHeight = image->h;
        mTexture = SDL_CreateTextureFromSurface( gRenderer, image );
    }

    SDL_FreeSurface( image );

    return success;
    };

    void LTexture::Render( int x, int y )
    {
        SDL_Rect dest
        {
          .x = x,
          .y = y,
          .w = mWidth,
          .h =  mHeight
        };
        SDL_RenderCopy( gRenderer, mTexture, NULL, &dest);
    }
    int LTexture::getHeight()
    {
        return mHeight;
    }

    int LTexture::getWidth()
    {
        return mWidth;
    }


int main( int argc, char* args[] )
{

    //Initialize SDL
    if( !init() )
    {
        printf( "SDL could not initialize!\n" );
    }
//    LTexture bground2;
//    bground2.LoadTexture( "images/alphastars.png" )

    bool isRunning = true;

    SDL_Event e;

    LTexture bground1;
    bground1.LoadTexture( "images/starfield.png" );

//    LTexture bground2;
//    bground2.LoadTexture( "images/alphastars.png" )
    int x = 0;
    int y = 0;

    while( isRunning ) //Main loop
    {
        while( SDL_PollEvent( &e ) )
        {
            if( e.type == SDL_QUIT )
            {
                isRunning = false;
            }

        }

        SDL_RenderClear( gRenderer );
        //logo.Render( (WINDOW_WIDTH - logo.getWidth() ) / 2 , ( WINDOW_HEIGHT - logo.getHeight() ) / 2 ) ;
//        SDL_GetMouseState(&x, &y);
//        x = x - ( logo.getWidth() / 2 ) ;
//        y = y - ( logo.getHeight() / 2 );
    printf ( "x = %d\n", x );
    x--;
    if( x <=(-bground1.getWidth() ) / 2 )
    {
        x = 0;
    }
    bground1.Render( x, y );
        SDL_RenderPresent( gRenderer );
    }


    //Destroy Renderer
    SDL_DestroyRenderer( gRenderer );

    //Destroy Window
    SDL_DestroyWindow( gWindow );

    //Close SDL and all subsystems
    SDL_Quit();

    return 0;
}

bool init()
{
    bool success = true;

    //Initialize SDL with video subsystem
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to initialize SDL! SDL Error:%s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create a window
        gWindow = SDL_CreateWindow( "SDL-Test", NULL, NULL, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL)
        {
            printf( "Failed to create window! SDL Error:%s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL)
            {
                printf( "Failed to create renderer! SDL Error:%s\n", SDL_GetError() );
                success = false;
            }

            //Initialize SDL_image

            int flags = IMG_INIT_PNG;
            if( !( IMG_Init(flags) ) & flags)
            {
                printf( "Unable to initialize SDL_image! IMG Error:%s\n", IMG_GetError() );
                success = false;
            }

            //Clear the display
            SDL_SetRenderDrawColor( gRenderer, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderClear( gRenderer );
            SDL_RenderPresent( gRenderer );
        }
    }

return success;
}
