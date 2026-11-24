

#include <SFML/Graphics.hpp> // window,shapes , sprites, text
#include <SFML/Audio.hpp>// music & sound effects 
#include <fstream> // file reading/writing (high score)
#include <cstdlib> // rand(), srand()
#include <cmath>     // fmod()
#include <ctime>               // time() for the random seed
#include <string>
using namespace std;
const int   WIN_W = 800;
const int   WIN_H = 600;
const int   CELL = 40;      // size of one track cell AND of the player
const int   ROWS = 2;        // row 0 = ceiling lane, row 1 = floor lane
const int   COLS = 21;      // 20 visible columns + 1 extra for smooth scrolling
const float PLAYER_X = 100;     // the player never moves left/right
const float CEIL_Y = 60;      // player's y when touching the ceiling
const float FLOOR_Y = 500;     // player's y when touching the floor
// to track array;
const int EMPTY = 0;
const int OBSTACLE = 1;
const int SHIELD = 2;
const int BOOSTER = 3;
// the 4 screen
enum GameState { MENU, INSTRUCTIONS, PLAYING, GAMEOVER };
int   track[ROWS][COLS];              //  2d array that holds the whole map
float scrollOffset = 0;      // pixels scrolled inside the current cell
float stripeOffset = 0;    // moves the yellow stripes on the bars
float speed = 280;        // current scrolling speed (pixels/second)


const float START_SPEED = 280;
const float MAX_SPEED = 650;
const float ACCELERATION = 6;    // speed increase

// player data 
float playerY = FLOOR_Y;
float velocityY = 0;
bool  gravityDown = true;


// game data
bool  hasShield = false;         
bool  gameOver = false;       
float scoreF = 0;             // score as float 
int   highScore = 0;


sf::Font font;

sf::Texture bgTexture;         // the image stored in memory
sf::Sprite  bgSprite;       // the thing we actually draw on screen
bool        hasBackground = false;
float       bgOffset = 0;
const bool  BACKGROUND_SCROLLS = true;   // set to false for a still background
void updateBackground(float dt);
void resetPlayer();
void flipGravity();
void updatePlayer(float dt);
void loadPlayerAssets();
void drawPlayer(sf::RenderWindow& window);
void loadAudio();
void startMusic();
void stopMusic();
void playFlipSound();
void resetPowerups();
void spawnColumn();
void checkCollisions();
void drawTrackObjects(sf::RenderWindow& window);
void drawShieldRing(sf::RenderWindow& window);


void clearTrack()
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            track[r][c] = EMPTY;
}
/* moves every column one step  then asks  spawnColumn() to create a fresh column on the right edge*/
void shiftTrack()
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS - 1; c++)
            track[r][c] = track[r][c + 1];
             spawnColumn();

}/*scrolls smoothly we slide pixel by pixel, and every time we have
slid one full cell (40 px) we shift the array by one column.*/
void updateTrack(float dt){
    scrollOffset += speed * dt;
    while (scrollOffset >= CELL){
        scrollOffset -= CELL;
        shiftTrack();
    }
    stripeOffset = fmod(stripeOffset + speed * dt, 100.0f);
    if (speed < MAX_SPEED)
        speed += ACCELERATION * dt;            // game gets faster over time
}
void updateScore(float dt)
{
    scoreF += dt * 10;              // 10 points per second survived
}
// puts everything back to the starting state for a new run
void startGame(){
    clearTrack();
    scrollOffset = 0;
    stripeOffset = 0;
    speed = START_SPEED;
    gameOver = false;
    scoreF = 0;
    resetPlayer();       
    resetPowerups();     
    startMusic();        
}
// everything that changes in one frame while playing
void updateGame(float dt){
    updateTrack(dt);        
    updateBackground(dt);   
    updatePlayer(dt);       
    checkCollisions();      
    updateScore(dt);
}
int loadHighScore(){
    ifstream in("highscore.txt");       
    int value = 0;
    if (in.is_open())                  
    {
        in >> value;
        in.close();
    }
    return value;
}
void saveHighScore(int value){
    ofstream out("highscore.txt");    
    if (out.is_open())
    {
        out << value;
        out.close();
    }
}
// tries to load background.png. if the file is missing we simply
// keep hasBackground = false and draw a plain dark colour instead.
void loadBackground(){
    if (bgTexture.loadFromFile("background.png")){
        hasBackground = true;
        bgSprite.setTexture(bgTexture);
        float scaleX = (float)WIN_W / bgTexture.getSize().x;
        float scaleY = (float)WIN_H / bgTexture.getSize().y;
        bgSprite.setScale(scaleX, scaleY);
    }

}
// Slides the background left slower than the track;
void updateBackground(float dt){
    if (BACKGROUND_SCROLLS && hasBackground)
        bgOffset = fmod(bgOffset + speed * 0.15f * dt, (float)WIN_W);
}
void drawBackground(sf::RenderWindow& window){
    if (!hasBackground){
        window.clear(sf::Color(15, 15, 35));   
        return;
    }
    window.clear();
    // draw the picture twice side by side so it loops without a gap
    bgSprite.setPosition(-bgOffset, 0);
    window.draw(bgSprite);
    bgSprite.setPosition(WIN_W - bgOffset, 0);
    window.draw(bgSprite);
}
bool loadFont(){
    const string paths[] = {
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf"
    };
    for (int i = 0; i < 4; i++)
        if (font.loadFromFile(paths[i]))
            return true;
    return false;
}
void drawText(sf::RenderWindow& window, const string& str, int size,float x, float y, sf::Color color, bool centered){
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);


    if (centered)
    {
        sf::FloatRect b = text.getLocalBounds();
        text.setOrigin(b.left + b.width / 2.0f, 0);   // origin = middle of the text
    }
    text.setPosition(x, y);
    window.draw(text);
}
void drawScreen(sf::RenderWindow& window){
    drawBackground(window);              // 1. background


    drawTrackObjects(window);            // 2. obstacles, shields, boosters (Chunk 3)
    drawPlayer(window);                  // 3. the player (Chunk 2)
    drawShieldRing(window);              // 4. ring around player when shielded (Chunk 3)

    //  red ceiling and floor bars (drawn on top so edges stay clean)
    sf::RectangleShape bar(sf::Vector2f(WIN_W, 20));
    bar.setFillColor(sf::Color::Red);
    bar.setPosition(0, 40);
    window.draw(bar);
    bar.setPosition(0, 540);
    window.draw(bar);

    // to move stripes
    sf::RectangleShape stripe(sf::Vector2f(20, 20));
    stripe.setFillColor(sf::Color::Yellow);
    for (int i = 0; i < 9; i++){
        float x = i * 100 - stripeOffset;
        stripe.setPosition(x, 40);
        window.draw(stripe);
        stripe.setPosition(x, 540);
        window.draw(stripe);
    }



    drawText(window, "Score: " + to_string((int)scoreF), 22, 10, 6, sf::Color::White, false);
    drawText(window, "Best: " + to_string(highScore), 22, WIN_W - 150, 6, sf::Color(255, 220, 0), false);
    if (hasShield)
        drawText(window, "SHIELD ON", 18, WIN_W / 2.0f, 8, sf::Color(255, 220, 0), true);
}

void drawMenu(sf::RenderWindow& window, int selected){
    window.clear(sf::Color(20, 20, 40));
    drawText(window, "GRAVITY FLIP RUNNER", 48, WIN_W / 2.0f, 90, sf::Color::Cyan, true);

    string items[3] = { "Start Game", "Instructions", "Quit" };
    for (int i = 0; i < 3; i++)
    {
        sf::Color col = (i == selected) ? sf::Color(255, 220, 0) : sf::Color::White;
        string label = (i == selected) ? "> " + items[i] + " <" : items[i];
        drawText(window, label, 32, WIN_W / 2.0f, 220 + i * 70, col, true);
    }


    drawText(window, "Best score: " + to_string(highScore), 22, WIN_W / 2.0f, 500, sf::Color(180, 180, 180), true);
}

void drawInstructions(sf::RenderWindow& window){
    window.clear(sf::Color(20, 20, 40));
    drawText(window, "HOW TO PLAY", 40, WIN_W / 2.0f, 60, sf::Color::Cyan, true);
    drawText(window, "SPACE  -  flip gravity (floor <-> ceiling)", 24, WIN_W / 2.0f, 160, sf::Color::White, true);
    drawText(window, "Orange blocks end the game", 24, WIN_W / 2.0f, 215, sf::Color(255, 140, 0), true);
    drawText(window, "Blue orb = Shield (survive one hit)", 24, WIN_W / 2.0f, 270, sf::Color(90, 170, 255), true);
    drawText(window, "Green gem = +50 bonus points", 24, WIN_W / 2.0f, 325, sf::Color(90, 240, 110), true);
    drawText(window, "The longer you survive, the faster it gets!", 24, WIN_W / 2.0f, 380, sf::Color::White, true);
    drawText(window, "Press ESC to go back", 20, WIN_W / 2.0f, 500, sf::Color(180, 180, 180), true);
}


void drawGameOver(sf::RenderWindow& window)
{
    window.clear(sf::Color(30, 10, 10));
    drawText(window, "GAME OVER", 56, WIN_W / 2.0f, 110, sf::Color(255, 70, 70), true);
    drawText(window, "Final Score: " + to_string((int)scoreF), 32, WIN_W / 2.0f, 230, sf::Color::White, true);
    drawText(window, "Best Score: " + to_string(highScore), 32, WIN_W / 2.0f, 285, sf::Color(255, 220, 0), true);
    drawText(window, "Press R to restart  |  ESC for menu", 24, WIN_W / 2.0f, 400, sf::Color(200, 200, 200), true);
}
// how often things appear 
const int OBSTACLE_CHANCE = 30;     // 0-29  -> obstacle
const int SHIELD_CHANCE = 3;      // 30-32 -> shield (rare)
const int BOOSTER_CHANCE = 8;      // 33-40 -> score booster

const int   BOOSTER_POINTS = 50;    // bonus points for one booster

// counts empty columns since the last obstacle (only Chunk 3 uses it)
int columnsSinceObstacle = 0;
// Called when a new game starts
void resetPowerups()
{
    hasShield = false;
    columnsSinceObstacle = 0;
}
/* the gap between obstacles grows with speed so the player always
 has enough time to flip to the other side.*/
void spawnColumn()
{
    track[0][COLS - 1] = EMPTY;
    track[1][COLS - 1] = EMPTY;
    columnsSinceObstacle++;

    int minGap = (int)(speed * 0.6f / CELL) + 2;   // minimum empty columns
    int roll = rand() % 100;                  // random number 0..99
    int row = rand() % 2;                   // 0 = ceiling, 1 = floor

    if (roll < OBSTACLE_CHANCE && columnsSinceObstacle >= minGap)
    {
        track[row][COLS - 1] = OBSTACLE;
        columnsSinceObstacle = 0;
    }
    else if (roll >= OBSTACLE_CHANCE && roll < OBSTACLE_CHANCE + SHIELD_CHANCE)
        track[row][COLS - 1] = SHIELD;
    else if (roll >= OBSTACLE_CHANCE + SHIELD_CHANCE &&
        roll < OBSTACLE_CHANCE + SHIELD_CHANCE + BOOSTER_CHANCE)
        track[row][COLS - 1] = BOOSTER;
}
// Compares the player's rectangle with every object on the track.
void checkCollisions(){
    const float margin = 6; 
    // the player's hitbox (a slightly smaller rectangle)
    sf::FloatRect playerBox(PLAYER_X + margin, playerY + margin,
        CELL - 2 * margin, CELL - 2 * margin);

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            int cell = track[r][c];
            if (cell == EMPTY)
                continue;

            // where this cell really is on the screen
            float x = c * CELL - scrollOffset;
            float y = (r == 0) ? CEIL_Y : FLOOR_Y;
            sf::FloatRect cellBox(x + margin, y + margin,
                CELL - 2 * margin, CELL - 2 * margin);

            if (!playerBox.intersects(cellBox))
                continue;                       

            if (cell == OBSTACLE)
            {
                if (hasShield)
                {
                    hasShield = false;          // shield breaks, player survives
                    track[r][c] = EMPTY;
                }
                else
                    gameOver = true;            // no shield = game over
            }
            else if (cell == SHIELD)
            {
                hasShield = true;               // collect the shield
                track[r][c] = EMPTY;
            }
            else if (cell == BOOSTER)
            {
                scoreF += BOOSTER_POINTS;       // bonus points
                track[r][c] = EMPTY;
            }
        }
    }
}

// Draws every obstacle, shield orb and booster gem found in the array
void drawTrackObjects(sf::RenderWindow& window){
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++){
            int cell = track[r][c];
            if (cell == EMPTY)
                continue;

            float x = c * CELL - scrollOffset;
            float y = (r == 0) ? CEIL_Y : FLOOR_Y;

            if (cell == OBSTACLE)
            {
                sf::RectangleShape block(sf::Vector2f(CELL, CELL));
                block.setFillColor(sf::Color(255, 140, 0));     
                block.setPosition(x, y);
                window.draw(block);
            }
            else if (cell == SHIELD)
            {
                sf::CircleShape orb(16);                       
                orb.setFillColor(sf::Color(60, 160, 255));       
                orb.setOutlineColor(sf::Color::White);
                orb.setOutlineThickness(2);
                orb.setPosition(x + 4, y + 4);
                window.draw(orb);
            }
            else if (cell == BOOSTER)
            {
                sf::RectangleShape gem(sf::Vector2f(22, 22));   
                gem.setFillColor(sf::Color(80, 230, 100));      
                gem.setOrigin(11, 11);                          
                gem.setRotation(45);                             
                gem.setPosition(x + CELL / 2.0f, y + CELL / 2.0f);
                window.draw(gem);
            }
        }
    }
}

//ring around   the player while the shield is active
void drawShieldRing(sf::RenderWindow& window){
    if (!hasShield)
        return;

    sf::CircleShape ring(28);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineColor(sf::Color(255, 220, 0));
    ring.setOutlineThickness(3);
    ring.setOrigin(28, 28);                                      // centre of the circle
    ring.setPosition(PLAYER_X + CELL / 2.0f, playerY + CELL / 2.0f);
    window.draw(ring);
}

const float GRAVITY = 3500;   
const float MAX_VELOCITY = 1500;   

sf::Texture playerTexture;          
sf::Sprite  playerSprite;           
bool        hasPlayerSprite = false;

sf::Music       music;             
sf::SoundBuffer flipBuffer;         
sf::Sound       flipSound;         
bool hasMusic = false;
bool hasFlipSound = false;
// puts the player back on the floor for a new run
void resetPlayer()
{
    playerY = FLOOR_Y;
    velocityY = 0;
    gravityDown = true;
}

//swap the direction of gravity
void flipGravity()
{
    gravityDown = !gravityDown;     
    playFlipSound();           
}

// gravity pulls the player toward the floor or ceiling;
void updatePlayer(float dt)
{
    float direction = gravityDown ? 1.0f : -1.0f;     // +1 down, -1 up

    //increases the speed 
    velocityY += direction * GRAVITY * dt;
    if (velocityY > MAX_VELOCITY) velocityY = MAX_VELOCITY;
    if (velocityY < -MAX_VELOCITY) velocityY = -MAX_VELOCITY;
    // speed moves the player
    playerY += velocityY * dt;
    if (playerY >= FLOOR_Y)
    {
        playerY = FLOOR_Y;
        if (velocityY > 0) velocityY = 0;
    }
    if (playerY <= CEIL_Y)
    {
        playerY = CEIL_Y;
        if (velocityY < 0) velocityY = 0;
    }
}
void loadPlayerAssets(){
    if (playerTexture.loadFromFile("player.png"))
    {
        hasPlayerSprite = true;
        playerSprite.setTexture(playerTexture);
    }
}

// draw the player. when gravity is up , the sprite is flipped upside down.
void drawPlayer(sf::RenderWindow& window)
{
    if (hasPlayerSprite){
        const float sizeMultiplier = 2.5f;  

        float scale = ((float)CELL / playerTexture.getSize().y) * sizeMultiplier;
        float drawWidth = playerTexture.getSize().x * scale;
        float drawHeight = playerTexture.getSize().y * scale;
        float offsetX = (CELL - drawWidth) / 2.0f;
        if (gravityDown)
        {
            playerSprite.setScale(scale, scale);
            playerSprite.setPosition(PLAYER_X + offsetX, (playerY + CELL) - drawHeight);
        }
        else
        {
            
            playerSprite.setScale(scale, -scale);
            playerSprite.setPosition(PLAYER_X + offsetX, playerY + drawHeight);
        }
        window.draw(playerSprite);
    }else{
        sf::RectangleShape player(sf::Vector2f(CELL, CELL));
        player.setFillColor(sf::Color::Cyan);
        player.setPosition(PLAYER_X, playerY);
        window.draw(player);
    }
}

void loadAudio()
{
    hasMusic = music.openFromFile("music.ogg");
    if (hasMusic)
    {
        music.setLoop(true);       
        music.setVolume(40);       
    }


    hasFlipSound = flipBuffer.loadFromFile("flip.wav");
    if (hasFlipSound)
    {
        flipSound.setBuffer(flipBuffer);  
        flipSound.setVolume(70);
    }
}

void startMusic()
{
    if (hasMusic)
        music.play();                // to start player from begining
}

void stopMusic()
{
    if (hasMusic)
        music.stop();
}

void playFlipSound(){
    if (hasFlipSound)
        flipSound.play();
}
int main(){
    srand((unsigned)time(0));            

    sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Gravity Flip Runner");
    window.setFramerateLimit(60);
    if (!loadFont())
        return 1;
    loadBackground();                  
    loadPlayerAssets();                 
    loadAudio();

    highScore = loadHighScore();
    clearTrack();

    GameState state = MENU;
    int menuChoice = 0;
    sf::Clock clock;                   

    while (window.isOpen())
    {

        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                sf::Keyboard::Key key = event.key.code;

                if (state == MENU)
                {
                    if (key == sf::Keyboard::Up)
                        menuChoice = (menuChoice + 2) % 3;
                    else if (key == sf::Keyboard::Down)
                        menuChoice = (menuChoice + 1) % 3;
                    else if (key == sf::Keyboard::Return)
                    {
                        if (menuChoice == 0) { startGame(); state = PLAYING; }
                        else if (menuChoice == 1) state = INSTRUCTIONS;
                        else                      window.close();
                    }
                }
                else if (state == INSTRUCTIONS)
                {
                    if (key == sf::Keyboard::Escape)
                        state = MENU;
                }
                else if (state == PLAYING)
                {
                    if (key == sf::Keyboard::Space)
                        flipGravity();               
                    else if (key == sf::Keyboard::Escape)
                    {
                        stopMusic();                  
                        state = MENU;
                    }
                }
                else if (state == GAMEOVER)
                {
                    if (key == sf::Keyboard::R)
                    {
                        startGame();
                        state = PLAYING;
                    }
                    else if (key == sf::Keyboard::Escape)
                        state = MENU;
                }
            }
        }
        // update  game 
        if (state == PLAYING)
        {
            updateGame(dt);

            if (gameOver)
            {
                stopMusic();
                if ((int)scoreF > highScore)
                {
                    highScore = (int)scoreF;
                    saveHighScore(highScore);
                }
                state = GAMEOVER;
            }
        }
        if (state == MENU)              drawMenu(window, menuChoice);
        else if (state == INSTRUCTIONS) drawInstructions(window);
        else if (state == PLAYING)      drawScreen(window);
        else                            drawGameOver(window);
        window.display();                
    }
    return 0;
}
