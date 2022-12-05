#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#undef main
using namespace std;


SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;

TTF_Font *gFontSmall = NULL;
TTF_Font *gFontMid = NULL;
TTF_Font *gFontBig = NULL;
const SDL_Color whiteC = {255, 255, 255, 1};

SDL_Texture *title = NULL;
SDL_Surface *titleS = NULL;
SDL_Rect titleRect = {300, 100, 250, 150};

SDL_Texture *author = NULL;
SDL_Surface *authorS = NULL;
SDL_Rect authorRect = {600, 550, 350, 25};

SDL_Texture *menuLine1 = NULL;
SDL_Surface *menuLine1S = NULL;
SDL_Rect menuLine1Rect = {300, 300, 200, 25};
SDL_Texture *menuLine2 = NULL;
SDL_Surface *menuLine2S = NULL;
SDL_Rect menuLine2Rect = {300, 350, 200, 25};
SDL_Texture *menuPlay = NULL;
SDL_Surface *menuPlayS = NULL;
SDL_Rect menuPlayRect = {400, 450, 200, 50};
int menuFocus = 0, menuFocusOld = 0;

SDL_Texture *points = NULL;
SDL_Surface *pointsS = NULL;
SDL_Rect pointsRect = {750, 25, 150, 50};
std::string scoreStr;

SDL_Texture *gameMode = NULL;
SDL_Surface *gameModeS = NULL;
SDL_Rect gameModeRect = {50, 25, 150, 50};
std::string gameModeStr = "";
int gameModeID = 0;

SDL_Texture *deathMSG = NULL;
SDL_Surface *deathMSGS = NULL;
SDL_Rect deathMSGRect = {300, 250, 300, 50};

struct ballD
{
    int speed;
    int direction;
};
ballD ballData{1, 1};
SDL_Rect player = {20, 100, 40, 160};
SDL_Rect enemy = {940, 100, 40, 160};
SDL_Rect ball = {450, 200, 40, 40};
bool quit = false;
int mode = 0, score = 0, scorep2 = 0;
bool modeInit[7] = {false};



double Round(double Zahl, int Stellen)
{
    double v[] = { 1, 10, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8 };
    return floor(Zahl * v[Stellen] + 0.5) / v[Stellen];
}

void cleanExit()
{
    clog << "-- CLEANING RAM AND QUIT --\n";;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_CloseFont(gFontSmall);
    TTF_CloseFont(gFontMid);
    TTF_CloseFont(gFontBig);
    TTF_Quit();
    SDL_Quit();
}

bool init()
{
    clog << "-- INIT --\n";
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if(TTF_Init() == -1)
    {
        cerr << "!! FONT ENGINE INIT ERROR !!\n";
        return false;
    }
    gFontSmall = TTF_OpenFont("font.ttf", 12);
    gFontMid = TTF_OpenFont("font.ttf", 18);
    gFontBig = TTF_OpenFont("font.ttf", 24);
    if(gFontSmall == NULL || gFontMid == NULL || gFontBig == NULL)
    {
        cerr << "!! FONT LOADING ERROR !! " << TTF_GetError << "\n";
        return false;
    }
    gWindow = SDL_CreateWindow("PONG!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    return true;
}

int main()
{
    if(init() == false)
        return 1;
    SDL_Event e;
    while (!quit)
    {
        // QUIT EVENT
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
                quit = true;
        }
        if(mode == 0)
        {
            if(modeInit[0] == false)
            {
                clog << "-- INIT VIEW 0 - MAIN MENUE --\n";
                titleS = TTF_RenderUTF8_Solid(gFontBig, "PONG", whiteC);
                title = SDL_CreateTextureFromSurface(gRenderer, titleS);
                authorS = TTF_RenderUTF8_Solid(gFontSmall, "2017  Benjamin Zastrow", whiteC);
                author = SDL_CreateTextureFromSurface(gRenderer, authorS);
                menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "PLAY 1P )(", whiteC);
                menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "PLAY 2P   ", whiteC);
                menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                menuLine2 = SDL_CreateTextureFromSurface(gRenderer, menuLine2S);
                modeInit[0] = true;
            }
            if(menuFocus != menuFocusOld)
            {
                if(menuFocus == 0)
                {
                    menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "PLAY 2P   ", whiteC);
                    menuLine2 = SDL_CreateTextureFromSurface(gRenderer, menuLine2S);
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "PLAY 1P )(", whiteC);
                    menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                }
                if(menuFocus == 1)
                {
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "PLAY 1P   ", whiteC);
                    menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                    menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "PLAY 2P )(", whiteC);
                    menuLine2 = SDL_CreateTextureFromSurface(gRenderer, menuLine2S);
                }
            }
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, menuLine1, NULL, &menuLine1Rect);
            SDL_RenderCopy(gRenderer, menuLine2, NULL, &menuLine2Rect);
            SDL_RenderCopy(gRenderer, author, NULL, &authorRect);
            SDL_RenderCopy(gRenderer, title, NULL, &titleRect);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(25);
            menuFocusOld = menuFocus;
            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if(currentKeyStates[SDL_SCANCODE_ESCAPE])
                quit = true;
            if((currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN]) && menuFocus < 1)
                menuFocus++;
            if((currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]) && menuFocus > 0)
                menuFocus--;
            if(currentKeyStates[SDL_SCANCODE_RETURN])
            {
                if(menuFocus == 0)
                {
                    mode = 1;
                    modeInit[0] = false;
                    menuFocus = 0;

                }

                if(menuFocus == 1)
                {
                    mode = 3;
                    modeInit[0] = false;
                    menuFocus = 0;
                }
            }
        }
        if(mode == 1)
        {
            if(modeInit[1] == false)
            {
                clog << "-- INIT VIEW 1 - SELECTION SCREEN 1P --\n";
                menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "MODE CLASSIC )(", whiteC);
                menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 1        ", whiteC);
                menuPlayS = TTF_RenderUTF8_Solid(gFontMid, "PLAY   ", whiteC);
                menuPlay = SDL_CreateTextureFromSurface(gRenderer, menuPlayS);
                menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                menuLine2 = SDL_CreateTextureFromSurface(gRenderer, menuLine2S);
                modeInit[1] = true;
            }
            if(menuFocus == 0)
            {
                if(gameModeID == 0)
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "MODE CLASSIC  )(", whiteC);
                else
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "MODE TIMEBOMB )(", whiteC);
                menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                if(ballData.speed == 1)
                    menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 1         ", whiteC);
                else
                    menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 2         ", whiteC);

                menuLine2 = SDL_CreateTextureFromSurface(gRenderer, menuLine2S);
                menuPlayS = TTF_RenderUTF8_Solid(gFontMid, "PLAY   ", whiteC);
                menuPlay = SDL_CreateTextureFromSurface(gRenderer, menuPlayS);
            }
            if(menuFocus == 1)
            {
                if(gameModeID == 0)
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "MODE CLASSIC    ", whiteC);
                else
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "MODE TIMEBOMB   ", whiteC);
                menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                if(ballData.speed == 1)
                    menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 1       )(", whiteC);
                else
                    menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 2       )(", whiteC);

                menuLine2 = SDL_CreateTextureFromSurface(gRenderer, menuLine2S);
                menuPlayS = TTF_RenderUTF8_Solid(gFontMid, "PLAY   ", whiteC);
                menuPlay = SDL_CreateTextureFromSurface(gRenderer, menuPlayS);
            }
            if(menuFocus == 2)
            {
                if(gameModeID == 0)
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "MODE CLASSIC    ", whiteC);
                else
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "MODE TIMEBOMB   ", whiteC);
                if(ballData.speed == 1)
                    menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 1         ", whiteC);
                else
                    menuLine2S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 2         ", whiteC);
                menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                menuLine2 = SDL_CreateTextureFromSurface(gRenderer, menuLine2S);
                menuPlayS = TTF_RenderUTF8_Solid(gFontMid, "PLAY )(", whiteC);
                menuPlay = SDL_CreateTextureFromSurface(gRenderer, menuPlayS);
            }

            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, menuLine1, NULL, &menuLine1Rect);
            SDL_RenderCopy(gRenderer, menuLine2, NULL, &menuLine2Rect);
            SDL_RenderCopy(gRenderer, menuPlay, NULL, &menuPlayRect);
            SDL_RenderCopy(gRenderer, author, NULL, &authorRect);
            SDL_RenderCopy(gRenderer, title, NULL, &titleRect);

            SDL_RenderPresent(gRenderer);

            SDL_Delay(75);

            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if(currentKeyStates[SDL_SCANCODE_ESCAPE])
                quit = true;
            if((currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN]) && menuFocus < 2)
                menuFocus++;
            if((currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]) && menuFocus > 0)
                menuFocus--;
            if(currentKeyStates[SDL_SCANCODE_RETURN])
            {
                clog << "FOCUS: " << menuFocus << "\n";
                if(menuFocus == 0)
                {
                    if(gameModeID == 0)
                        gameModeID = 1;
                    else
                        gameModeID = 0;
                }
                if(menuFocus == 1)
                {
                    if(ballData.speed == 1)
                        ballData.speed = 2;
                    else
                        ballData.speed = 1;
                }
                if(menuFocus == 2)
                {
                    if(gameModeID == 0)
                        gameModeStr = "CLASSIC";
                    else
                        gameModeStr = "TIMEBOMB";
                    mode = 2;
                    modeInit[1] = false;
                    menuFocus = 0;
                    gameModeS = TTF_RenderUTF8_Solid(gFontMid, gameModeStr.c_str(), whiteC);
                    gameMode = SDL_CreateTextureFromSurface(gRenderer, gameModeS);
                }
            }

        }
        if(mode == 2)
        {
            // KEYBOARD INPUT / PLAYER CONTROLS
            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if(currentKeyStates[SDL_SCANCODE_W] && player.y > 102)
            {
                player.y = player.y - 2;
            }
            if(currentKeyStates[SDL_SCANCODE_S] && player.y < 398)
            {
                player.y = player.y + 2;
            }
            if(currentKeyStates[SDL_SCANCODE_ESCAPE])
                quit = true;

            //BALL CONTACT WITH PLAYER/ENEMY
            if(ball.x + 40 >= enemy.x)
            {
                clog << "CONTACT WITH ENEMY\n";
                if(ballData.direction > 0 && ballData.direction < 100)
                {
                    ballData.direction = 100 + (100 - ballData.direction);
                }
                else if(ballData.direction > 300 && ballData.direction < 400)
                {
                    ballData.direction = 300 + (300 - ballData.direction);
                }
                else
                {
                    ballData.direction = 200;
                }
            }
            if(ball.x <= player.x + 40 && (ball.y <= player.y + 160 && ball.y + 160 >= player.y))
            {
                if(ball.y - player.y >= 0 && ball.y - player.y <= 32)
                    ballData.direction = 75;
                if(ball.y - player.y > 32 && ball.y - player.y <= 64)
                    ballData.direction = 25;
                if(ball.y - player.y > 64 && ball.y - player.y <= 96)
                    ballData.direction = 25;
                if(ball.y - player.y > 96 && ball.y - player.y <= 128)
                    ballData.direction = 375;
                if(ball.y - player.y > 128 && ball.y - player.y <= 160)
                    ballData.direction = 350;
                score++;
                clog << "CONTACT WITH PLAYER - SCORE: " << score << "\n";
                scoreStr = "SCORE   " + std::to_string(score);
                pointsS = TTF_RenderUTF8_Solid(gFontMid, scoreStr.c_str(), whiteC);
                points = SDL_CreateTextureFromSurface(gRenderer, pointsS);
            }

            //BALL CONTACT WITH BORDERS
            if(ball.x <= 20)
            {
                //"YOU DIED" SCREEN IMPLEMENTATION
                mode = 5;
            }
            else if(ball.y >= 518)
            {
                clog << "CONTACT WITH LOWER BORDER\n";
                if(ballData.direction > 300)
                    ballData.direction = 51;
                else
                    ballData.direction = 140;
            }
            else if(ball.y <= 102)
            {
                clog << "CONTACT WITH UPPER BORDER\n";
                if(ballData.direction < 100)
                    ballData.direction = 320;
                else
                    ballData.direction = 251;
            }

            //BALL MOVEMENT
            if(ballData.direction == 0)
            {
                ball.x = ball.x + ballData.speed * 5;
            }
            if(ballData.direction > 0 && ballData.direction <= 50)
            {
                ball.x = ball.x + ballData.speed * 4;
                ball.y = ball.y - ballData.speed * 1;
            }
            if(ballData.direction > 50 && ballData.direction <= 100)
            {
                ball.x = ball.x + ballData.speed * 3;
                ball.y = ball.y - ballData.speed * 2;
            }
            if(ballData.direction > 100 && ballData.direction <= 150)
            {
                ball.x = ball.x - ballData.speed * 3;
                ball.y = ball.y - ballData.speed * 2;
            }
            if(ballData.direction > 150 && ballData.direction <= 200)
            {
                ball.x = ball.x - ballData.speed * 5;
            }
            if(ballData.direction > 200 && ballData.direction <= 250)
            {
                ball.x = ball.x - ballData.speed * 3;
                ball.y = ball.y + ballData.speed * 2;
            }
            if(ballData.direction > 250 && ballData.direction <= 300)
            {
                ball.x = ball.x - ballData.speed * 2;
                ball.y = ball.y + ballData.speed * 3;
            }
            if(ballData.direction > 300 && ballData.direction <= 350)
            {
                ball.x = ball.x + ballData.speed * 2;
                ball.y = ball.y + ballData.speed * 3;
            }
            if(ballData.direction > 350 && ballData.direction < 400)
            {
                ball.x = ball.x + ballData.speed * 4;
                ball.y = ball.y + ballData.speed * 1;
            }

            //SET ENEMY Y TO BALL'S Y
            enemy.y = ball.y - 80;

            // DRAW SCREEN
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(gRenderer);
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(gRenderer, &player);
            SDL_RenderFillRect(gRenderer, &enemy);
            SDL_RenderFillRect(gRenderer, &ball);
            SDL_RenderDrawLine(gRenderer, 0, 100, 1000, 100);
            SDL_RenderDrawLine(gRenderer, 0, 558, 1000, 558);

            SDL_RenderCopy(gRenderer, points, NULL, &pointsRect);
            SDL_RenderCopy(gRenderer, gameMode, NULL, &gameModeRect);

            SDL_RenderPresent(gRenderer);

            // DELAY THE LOOP FOR SYSTEM LOAD DECREASE
            SDL_Delay(10);
        }

        if(mode == 3)
        {
            if(modeInit[3] == false)
            {
                clog << "-- INIT VIEW 3 - SELECTION MENU 2P --\n";
                menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 1       )(", whiteC);
                menuPlayS = TTF_RenderUTF8_Solid(gFontMid, "PLAY   ", whiteC);
                menuPlay = SDL_CreateTextureFromSurface(gRenderer, menuPlayS);
                menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                gameModeS = TTF_RenderUTF8_Solid(gFontMid, gameModeStr.c_str(), whiteC);
                gameMode = SDL_CreateTextureFromSurface(gRenderer, gameModeS);
                modeInit[3] = true;
            }
            if(menuFocus == 0)
            {
                if(ballData.speed == 1)
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 1       )(", whiteC);
                else
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 2       )(", whiteC);
                menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                menuPlayS = TTF_RenderUTF8_Solid(gFontMid, "PLAY   ", whiteC);
                menuPlay = SDL_CreateTextureFromSurface(gRenderer, menuPlayS);
            }
            if(menuFocus == 1)
            {
                if(ballData.speed == 1)
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 1         ", whiteC);
                else
                    menuLine1S = TTF_RenderUTF8_Solid(gFontSmall, "SPEED 2         ", whiteC);
                menuLine1 = SDL_CreateTextureFromSurface(gRenderer, menuLine1S);
                menuPlayS = TTF_RenderUTF8_Solid(gFontMid, "PLAY )(", whiteC);
                menuPlay = SDL_CreateTextureFromSurface(gRenderer, menuPlayS);
            }

            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, menuLine1, NULL, &menuLine1Rect);
            SDL_RenderCopy(gRenderer, menuPlay, NULL, &menuPlayRect);
            SDL_RenderCopy(gRenderer, author, NULL, &authorRect);
            SDL_RenderCopy(gRenderer, title, NULL, &titleRect);
            SDL_RenderPresent(gRenderer);

            SDL_Delay(75);
            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if(currentKeyStates[SDL_SCANCODE_ESCAPE])
                quit = true;
            if((currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN]) && menuFocus < 1)
                menuFocus++;
            if((currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]) && menuFocus > 0)
                menuFocus--;
            if(currentKeyStates[SDL_SCANCODE_RETURN])
            {
                clog << "FOCUS: " << menuFocus << "\n";
                if(menuFocus == 0)
                {
                    if(ballData.speed == 1)
                        ballData.speed = 2;
                    else
                        ballData.speed = 1;
                }

                if(menuFocus == 1)
                {
                    gameModeStr = "DUOS";
                    mode = 4;
                    modeInit[3] = false;
                    menuFocus = 0;
                }
            }
        }
        if(mode == 4)
        {
            // KEYBOARD INPUT / PLAYER CONTROLS
            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if(currentKeyStates[SDL_SCANCODE_W] && player.y > 102)
            {
                player.y = player.y - 2;
            }
            if(currentKeyStates[SDL_SCANCODE_S] && player.y < 398)
            {
                player.y = player.y + 2;
            }
            if(currentKeyStates[SDL_SCANCODE_O] && enemy.y > 102)
            {
                enemy.y = enemy.y - 2;
            }
            if(currentKeyStates[SDL_SCANCODE_L] && enemy.y < 398)
            {
                enemy.y = enemy.y + 2;
            }
            if(currentKeyStates[SDL_SCANCODE_ESCAPE])
                quit = true;

            //BALL CONTACT WITH PLAYER/ENEMY
            if(ball.x + 40 >= enemy.x && ball.y <= enemy.y + 160 && ball.y + 160 >= enemy.y)
            {
                if(ball.y - enemy.y >= 0 && ball.y - enemy.y <= 32)
                    ballData.direction = 125;
                if(ball.y - enemy.y > 32 && ball.y - enemy.y <= 64)
                    ballData.direction = 175;
                if(ball.y - enemy.y > 64 && ball.y - enemy.y <= 96)
                    ballData.direction = 175;
                if(ball.y - enemy.y > 96 && ball.y - enemy.y <= 128)
                    ballData.direction = 225;
                if(ball.y - enemy.y > 128 && ball.y - enemy.y <= 160)
                    ballData.direction = 250;
                scorep2++;
                clog << "CONTACT WITH PLAYER 2 - SCORE: " << scorep2 << "\n";
                scoreStr = "SCORE " + std::to_string(score) + " " + std::to_string(scorep2);
                pointsS = TTF_RenderUTF8_Solid(gFontMid, scoreStr.c_str(), whiteC);
                points = SDL_CreateTextureFromSurface(gRenderer, pointsS);
            }
            if(ball.x <= player.x + 40 && ball.y <= player.y + 160 && ball.y + 160 >= player.y)
            {
                if(ball.y - player.y >= 0 && ball.y - player.y <= 32)
                    ballData.direction = 75;
                if(ball.y - player.y > 32 && ball.y - player.y <= 64)
                    ballData.direction = 25;
                if(ball.y - player.y > 64 && ball.y - player.y <= 96)
                    ballData.direction = 25;
                if(ball.y - player.y > 96 && ball.y - player.y <= 128)
                    ballData.direction = 375;
                if(ball.y - player.y > 128 && ball.y - player.y <= 160)
                    ballData.direction = 350;
                score++;
                clog << "CONTACT WITH PLAYER 1 - SCORE: " << score << "\n";
                scoreStr = "SCORE   " + std::to_string(score) + " " + std::to_string(scorep2);
                pointsS = TTF_RenderUTF8_Solid(gFontMid, scoreStr.c_str(), whiteC);
                points = SDL_CreateTextureFromSurface(gRenderer, pointsS);
            }

            //BALL CONTACT WITH BORDERS
            if(ball.x <= 20)
            {
                //"YOU DIED" SCREEN IMPLEMENTATION
                mode = 6;
            }
            if(ball.x >= 980)
                mode = 6;
            else if(ball.y >= 518)
            {
                clog << "CONTACT WITH LOWER BORDER\n";
                if(ballData.direction > 300)
                    ballData.direction = 51;
                else
                    ballData.direction = 140;
            }
            else if(ball.y <= 102)
            {
                clog << "CONTACT WITH UPPER BORDER\n";
                if(ballData.direction < 100)
                    ballData.direction = 320;
                else
                    ballData.direction = 251;
            }

            //BALL MOVEMENT
            if(ballData.direction == 0)
            {
                ball.x = ball.x + ballData.speed * 5;
            }
            if(ballData.direction > 0 && ballData.direction <= 50)
            {
                ball.x = ball.x + ballData.speed * 4;
                ball.y = ball.y - ballData.speed * 1;
            }
            if(ballData.direction > 50 && ballData.direction <= 100)
            {
                ball.x = ball.x + ballData.speed * 3;
                ball.y = ball.y - ballData.speed * 2;
            }
            if(ballData.direction > 100 && ballData.direction <= 150)
            {
                ball.x = ball.x - ballData.speed * 3;
                ball.y = ball.y - ballData.speed * 2;
            }
            if(ballData.direction > 150 && ballData.direction <= 200)
            {
                ball.x = ball.x - ballData.speed * 5;
            }
            if(ballData.direction > 200 && ballData.direction <= 250)
            {
                ball.x = ball.x - ballData.speed * 3;
                ball.y = ball.y + ballData.speed * 2;
            }
            if(ballData.direction > 250 && ballData.direction <= 300)
            {
                ball.x = ball.x - ballData.speed * 2;
                ball.y = ball.y + ballData.speed * 3;
            }
            if(ballData.direction > 300 && ballData.direction <= 350)
            {
                ball.x = ball.x + ballData.speed * 2;
                ball.y = ball.y + ballData.speed * 3;
            }
            if(ballData.direction > 350 && ballData.direction < 400)
            {
                ball.x = ball.x + ballData.speed * 4;
                ball.y = ball.y + ballData.speed * 1;
            }

            // DRAW SCREEN
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(gRenderer);
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(gRenderer, &player);
            SDL_RenderFillRect(gRenderer, &enemy);
            SDL_RenderFillRect(gRenderer, &ball);
            SDL_RenderDrawLine(gRenderer, 0, 100, 1000, 100);
            SDL_RenderDrawLine(gRenderer, 0, 558, 1000, 558);

            SDL_RenderCopy(gRenderer, points, NULL, &pointsRect);
            SDL_RenderCopy(gRenderer, gameMode, NULL, &gameModeRect);

            SDL_RenderPresent(gRenderer);

            // DELAY THE LOOP FOR SYSTEM LOAD DECREASE
            SDL_Delay(10);
        }
        if(mode == 5)
        {
            if(modeInit[5] == false)
            {
                clog << "-- INIT VIEW 5 - DEATH SCREEN 1P --\n";
                titleS = TTF_RenderUTF8_Solid(gFontBig, "YOU DIED", whiteC);
                deathMSGS = TTF_RenderUTF8_Solid(gFontMid, "PRESS ENTER TO RESTART", whiteC);
                title = SDL_CreateTextureFromSurface(gRenderer, titleS);
                deathMSG = SDL_CreateTextureFromSurface(gRenderer, deathMSGS);
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
                SDL_RenderClear(gRenderer);
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderDrawLine(gRenderer, 0, 100, 1000, 100);
                SDL_RenderDrawLine(gRenderer, 0, 558, 1000, 558);
                SDL_RenderCopy(gRenderer, title, NULL, &titleRect);
                SDL_RenderCopy(gRenderer, deathMSG, NULL, &deathMSGRect);
                SDL_RenderCopy(gRenderer, points, NULL, &pointsRect);
                SDL_RenderCopy(gRenderer, gameMode, NULL, &gameModeRect);
                SDL_RenderPresent(gRenderer);
                modeInit[5] = true;
            }
            SDL_Delay(75);
            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if(currentKeyStates[SDL_SCANCODE_ESCAPE])
                quit = true;
            if(currentKeyStates[SDL_SCANCODE_RETURN])
            {
                modeInit[5] = false;
                mode = 0;
                score = 0;
            }
        }
        if(mode == 6)
        {
            if(modeInit[6] == false)
            {
                clog << "-- INIT VIEW 6 - DEATH SCREEN 2P --\n";
                if(ball.x >= 980)
                {
                    titleS = TTF_RenderUTF8_Solid(gFontBig, "P2 DIED", whiteC);
                }
                if(ball.x <= 20)
                    titleS = TTF_RenderUTF8_Solid(gFontBig, "P1 DIED", whiteC);
                deathMSGS = TTF_RenderUTF8_Solid(gFontMid, "PRESS ENTER TO RESTART", whiteC);
                title = SDL_CreateTextureFromSurface(gRenderer, titleS);
                deathMSG = SDL_CreateTextureFromSurface(gRenderer, deathMSGS);
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
                SDL_RenderClear(gRenderer);
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderDrawLine(gRenderer, 0, 100, 1000, 100);
                SDL_RenderDrawLine(gRenderer, 0, 558, 1000, 558);
                SDL_RenderCopy(gRenderer, title, NULL, &titleRect);
                SDL_RenderCopy(gRenderer, deathMSG, NULL, &deathMSGRect);
                SDL_RenderCopy(gRenderer, points, NULL, &pointsRect);
                SDL_RenderCopy(gRenderer, gameMode, NULL, &gameModeRect);
                SDL_RenderPresent(gRenderer);
                modeInit[5] = true;
            }
            SDL_Delay(75);
            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if(currentKeyStates[SDL_SCANCODE_ESCAPE])
                quit = true;
            if(currentKeyStates[SDL_SCANCODE_RETURN])
            {
                modeInit[6] = false;
                mode = 0;
                score = 0;
                scorep2 = 0;
            }
        }

    }

    cleanExit();
    return 0;
}
