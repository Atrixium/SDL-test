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
    bool LoadKeyedTexture(std::string path, Uint8 r, Uint8 g, Uint8 b ); //Load a texture from a file, return false if fail
    void Render(int x, int y, SDL_Rect* clip); //Renders texture at X and Y and uses clip if not null

    int getWidth();
    int getHeight();

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

class SpriteSheet : LTexture
{
public:
    SpriteSheet();
    ~SpriteSheet();



private:

    int cols;
    int rows;
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
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
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
        mTexture = SDL_CreateTextureFromSurface( gRenderer, image );
        if(mTexture == NULL)
        {
            printf( "Unable to create texture from surface! SDL Error: %s", SDL_GetError() );
        }
        else
        {
            mWidth = image->w;
            mHeight = image->h;
        }
    }

    SDL_FreeSurface( image );

    return success;
    };

    bool LTexture::LoadKeyedTexture(std::string path, Uint8 r, Uint8 g, Uint8 b )
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
        SDL_SetColorKey( image, SDL_TRUE, SDL_MapRGB( image->format, r, g, b) );
        mTexture = SDL_CreateTextureFromSurface( gRenderer, image );
        if(mTexture == NULL)
        {
            printf( "Unable to create texture from surface! SDL Error: %s", SDL_GetError() );
            success = false;
        }
        else
        {
            mWidth = image->w;
            mHeight = image->h;
        }
    }

    SDL_FreeSurface( image );

    return success;
    };

    void LTexture::Render( int x, int y, SDL_Rect* clip )
    {
        SDL_Rect dest
        {
          .x = x,
          .y = y,
          .w = mWidth,
          .h =  mHeight
        };

        //Set clip rendering dimensions
        if( clip != NULL )
        {
            dest.w = clip->w;
            dest.h = clip->h;
        }

        SDL_RenderCopy( gRenderer, mTexture, clip, &dest);
    }
    int LTexture::getHeight()
    {
        return mHeight;
    }

    int LTexture::getWidth()
    {
        return mWidth;
    }

//Animation Struct
struct animation
{
    int numFrames;
    SDL_Rect frames[100]; //create an array of a max size of 100 frames
    int centerOffset[100]; //create an array of a max size of 100 frames
};

int main( int argc, char* args[] )
{

    //Initialize SDL
    if( !init() )
    {
        printf( "SDL could not initialize!\n" );
    }

    bool isRunning = true;

    SDL_Event e;

    LTexture anim;
    anim.LoadKeyedTexture( "images/Megaman.png", 0, 0, 0 );

    LTexture bground1;
    bground1.LoadTexture( "images/starfield.png" );

    LTexture bground2;
    bground2.LoadKeyedTexture( "images/alphastars.png", 0, 0, 0 );


    // Load and play animation with variable size frames

    // Instantiate an animation struct
    animation running;
    running.numFrames = 10;

    // SDL_Rect animation[10]; //Create an array large enough to hold each frame
    int i = 0; //current frame

    //Define frames

    running.frames[0].x = 0;
    running.frames[0].y = 91;
    running.frames[0].w = 32;
    running.frames[0].h = 42;

    running.frames[1].x = 34;
    running.frames[1].y = 91;
    running.frames[1].w = 33;
    running.frames[1].h = 42;

    running.frames[2].x = 73;
    running.frames[2].y = 91;
    running.frames[2].w = 45;
    running.frames[2].h = 42;

    running.frames[3].x = 122;
    running.frames[3].y = 91;
    running.frames[3].w = 36;
    running.frames[3].h = 42;

    running.frames[4].x = 162;
    running.frames[4].y = 91;
    running.frames[4].w = 29;
    running.frames[4].h = 42;

    running.frames[5].x = 193;
    running.frames[5].y = 91;
    running.frames[5].w = 29;
    running.frames[5].h = 42;

    running.frames[6].x = 224;
    running.frames[6].y = 91;
    running.frames[6].w = 29;
    running.frames[6].h = 42;

    running.frames[7].x = 258;
    running.frames[7].y = 91;
    running.frames[7].w = 39;
    running.frames[7].h = 42;

    running.frames[8].x = 302;
    running.frames[8].y = 91;
    running.frames[8].w = 30;
    running.frames[8].h = 42;

    running.frames[9].x = 336;
    running.frames[9].y = 91;
    running.frames[9].w = 28;
    running.frames[9].h = 42;

    running.centerOffset[0] = 20;
    running.centerOffset[1] = 21;
    running.centerOffset[2] = 28;
    running.centerOffset[3] = 23;
    running.centerOffset[4] = 17;
    running.centerOffset[5] = 17;
    running.centerOffset[6] = 17;
    running.centerOffset[7] = 22;
    running.centerOffset[8] = 18;
    running.centerOffset[9] = 15;

//    animation[0].x = 0;
//    animation[0].y = 91;
//    animation[0].w = 32;
//    animation[0].h = 42;
//
//    animation[1].x = 34;
//    animation[1].y = 91;
//    animation[1].w = 33;
//    animation[1].h = 42;
//
//    animation[2].x = 73;
//    animation[2].y = 91;
//    animation[2].w = 45;
//    animation[2].h = 42;
//
//    animation[3].x = 122;
//    animation[3].y = 91;
//    animation[3].w = 36;
//    animation[3].h = 42;
//
//    animation[4].x = 162;
//    animation[4].y = 91;
//    animation[4].w = 29;
//    animation[4].h = 42;
//
//    animation[5].x = 193;
//    animation[5].y = 91;
//    animation[5].w = 29;
//    animation[5].h = 42;
//
//    animation[6].x = 224;
//    animation[6].y = 91;
//    animation[6].w = 29;
//    animation[6].h = 42;
//
//    animation[7].x = 258;
//    animation[7].y = 91;
//    animation[7].w = 39;
//    animation[7].h = 42;
//
//    animation[8].x = 302;
//    animation[8].y = 91;
//    animation[8].w = 30;
//    animation[8].h = 42;
//
//    animation[9].x = 336;
//    animation[9].y = 91;
//    animation[9].w = 28;
//    animation[9].h = 42;
//
//    int centerOffset[10];
//
//    centerOffset[0] = 20;
//    centerOffset[1] = 21;
//    centerOffset[2] = 28;
//    centerOffset[3] = 23;
//    centerOffset[4] = 17;
//    centerOffset[5] = 17;
//    centerOffset[6] = 17;
//    centerOffset[7] = 22;
//    centerOffset[8] = 18;
//    centerOffset[9] = 15;


    // End animation process


    int mx;
    int my;

    int x1 = 0;
    int x2 = 0;
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
        anim.Render( (WINDOW_WIDTH - anim.getWidth() ) / 2 , ( WINDOW_HEIGHT - anim.getHeight() ) / 2, NULL );
        SDL_GetMouseState(&mx, &my);
        //mx = mx - ( logo.getWidth() / 2 ) ;
        //my = my - ( logo.getHeight() / 2 );
        printf ( "x1 = %d\n", x1 );
        printf ( "x2 = %d\n", x2 );
        x1--;
        x2 = x2 - 3;

        if( x1 <=(-bground1.getWidth() ) / 2 )
        {
            x1 = 0;
        }
            bground1.Render( x1, y, NULL );

        if( x2 <=(-bground2.getWidth() ) / 2 )
        {
            x2 = 0;
        }
            bground2.Render( x2, y, NULL );

            anim.Render( mx - running.centerOffset[i/4], my - 42, &running.frames[i/4] ); //Render current frame at mouse position
            i++; //increment frame count
            printf( "Displaying Frame #: %d\n", i/4 );
            if ( i /4 > running.numFrames - 1 ) //if frames is greater than total frames, recycle
            {
                i = 0;
            }


            SDL_RenderPresent( gRenderer );
    }

    //Destroy Texture(s)
    bground1.free();
    bground2.free();

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
