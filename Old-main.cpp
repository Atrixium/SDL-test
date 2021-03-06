// Includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>


// Global Constants

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

// Gloabal variables

    // The window we will be rendering to
    SDL_Window* gWindow = NULL;

    //The window renderer
    SDL_Renderer* gRenderer = NULL;

    //Globally used font
    TTF_Font* gFont = NULL;

    //Button constants
    const int BUTTON_WIDTH = 300;
    const int BUTTON_HEIGHT = 200;
    const int TOTAL_BUTTONS = 4;

    enum LButtonSprite
    {
        BUTTON_SPRITE_MOUSE_OUT = 0,
        BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
        BUTTON_SPRITE_MOUSE_DOWN = 2,
        BUTTON_SPRITE_MOUSE_UP = 3,
        BUTTON_SPRITE_TOTAL = 4
    };


// Forward Declarations

    // Starts up SDL and creates a window
    bool init();

    // Loads media
    bool loadMedia();

    //Frees media and shuts down SDL
    void close();

//Classes

        //Texture wrapper class
        class LTexture
        {
            public:
                //Initializes variables
                LTexture();

                //Deallocates memory
                ~LTexture();

                //Loads image at specified path
                bool loadFromFile( std::string path );

                #ifdef _SDL_TTF_H
                //Creates image from font string
                bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
                #endif

                //Deallocates texture
                void free();

                //Set color modulation
                void setColor( Uint8 red, Uint8 green, Uint8 blue );

                //Set blending mode
                void setBlendMode( SDL_BlendMode blending );

                //Set alpha modulation
                void setAlpha( Uint8 alpha );

                //Renders texture at given point
                void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

                //Gets image dimensions
                int getWidth();
                int getHeight();

            private:
                //The actual hardware texture
                SDL_Texture* mTexture;

                //Image dimensions
                int mWidth;
                int mHeight;
        };

        //The mouse button
        class LButton
        {
            public:
                //Initializes internal variables
                LButton();

                //Sets top left position
                void setPosition( int x, int y );

                //Handles mouse event
                void handleEvent(SDL_Event* e );

                //Shows button sprite
                void render();

            private:
                //Top left position
                SDL_Point mPosition;

                //Currently used global sprite
                LButtonSprite mCurrentSprite;
        };

//textures

    LTexture gButtonSpriteSheetTexture;
    SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];

    //Buttons objects
    LButton gButtons[ TOTAL_BUTTONS ];


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

            //While application is running
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

                    //Handle button events
                    for( int i = 0; i < TOTAL_BUTTONS; i++ )
                    {
                        gButtons[ i ].handleEvent( &e );
                    }
                }

                //Clear the screen
                SDL_SetRenderDrawColor( gRenderer, 0xff, 0xff, 0xff, 0xff );
                SDL_RenderClear( gRenderer );

                //Render buttons
                for(int i = 0; i < TOTAL_BUTTONS; i++ )
                {
                    gButtons[ i ].render();
                }

                //Update screen
                SDL_RenderPresent( gRenderer );

            }
        }
    }

    //Free resources and close SDL
    close();

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
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL)
            {
                printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer colour
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if ( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }

                //Initialize SDL_TTF
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error:%s\n", TTF_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load sprites
    if( !gButtonSpriteSheetTexture.loadFromFile( "images/button.png" ))
    {
        printf( "Unable to load button sprite sheet texture!\n");
        success = false;
    }
    else
    {
        //Set sprites
        for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
        {
            gSpriteClips[i].x = 0;
            gSpriteClips[i].y = i * 200;
            gSpriteClips[i].w = BUTTON_WIDTH;
            gSpriteClips[i].h = BUTTON_HEIGHT;
        }

        //Set buttons in corners
        gButtons[0].setPosition( 0, 0 );
        gButtons[1].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
        gButtons[2].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
        gButtons[3].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );

    }

return success;

}

void close()
{
    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xff, 0xff ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );

        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;

        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success

    mTexture = newTexture;
    return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "unable to render text surface! SDL_ttf Error:%s\n", SDL_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return mTexture != NULL;
}
#endif // _SDL_TTF_H

void LTexture::free()
{
    //Free Texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        //Check if mouse is in a button
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse is above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse is below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }

        //Mouse is outside a button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //mouse is inside a button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

void LButton::render()
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}
