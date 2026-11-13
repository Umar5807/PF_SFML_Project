//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <cstdlib>
//#include <ctime>
//using namespace sf;
//using namespace std;
//
//// window size, positions and physics values used by everyone
//const int WIN_W = 800;
//const int WIN_H = 600;
//const float FLOOR_Y = 500.f;
//const float CEILING_Y = 60.f;
//const float PLAYER_X = 100.f;
//const float BLOCK_SIZE = 40.f;
//const float GRAVITY = 0.6f;
//
//// one yellow stripe on the track
//struct Stripe
//{
//    float x;
//};
//
//// one obstacle, onFloor tells if it is on the floor or on the ceiling
//struct Obstacle
//{
//    float x;
//    bool onFloor;
//};
//
//// shared game data
//RectangleShape player(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
//float velocityY = 0.f;
//bool isGravityDown = true;
//float scrollSpeed = 5.f;
//
//vector<Stripe> stripes;
//vector<Obstacle> obstacles;
//int spawnTimer = 0;     // frames passed since the last obstacle
//int nextSpawn = 90;     // frames to wait before the next obstacle
//
//// function declarations
//void setupGame();
//void handleEvents(RenderWindow& window);
//void updateStripes();
//void drawTrack(RenderWindow& window);
//void updatePlayer();
//void drawPlayer(RenderWindow& window);
//void spawnObstacle();
//void updateObstacles();
//void drawObstacles(RenderWindow& window);
//
//// creates the window and runs the game loop
//int main()
//{
//    srand((unsigned)time(NULL));
//
//    RenderWindow window(VideoMode(WIN_W, WIN_H), "Gravity Flip Runner - Infinite Loop");
//    window.setFramerateLimit(60);
//
//    setupGame();
//
//    while (window.isOpen())
//    {
//        handleEvents(window);
//
//        updatePlayer();
//        updateStripes();
//        updateObstacles();
//
//        window.clear(Color::Black);
//        drawTrack(window);
//        drawObstacles(window);
//        drawPlayer(window);
//        window.display();
//    }
//
//    return 0;
//}
//
//// sets the player's color and start position and creates the stripes
//void setupGame()
//{
//    player.setFillColor(Color::Cyan);
//    player.setPosition(PLAYER_X, FLOOR_Y);
//
//    for (float i = 0; i < 800.f; i += 100.f)
//    {
//        stripes.push_back({ i });
//    }
//}
//
//// closes the window and flips gravity when space is pressed
//void handleEvents(RenderWindow& window)
//{
//    Event event;
//    while (window.pollEvent(event))
//    {
//        if (event.type == Event::Closed)
//            window.close();
//
//        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
//        {
//            isGravityDown = !isGravityDown;
//        }
//    }
//}
//
//// moves the stripes left and sends them back to the right side when they leave the screen
//void updateStripes()
//{
//    for (auto& stripe : stripes)
//    {
//        stripe.x -= scrollSpeed;
//        if (stripe.x < -20.f)
//        {
//            stripe.x += 820.f;
//        }
//    }
//}
//
//// draws the red ceiling and floor lines and the yellow stripes on them
//void drawTrack(RenderWindow& window)
//{
//    RectangleShape ceilingLine(Vector2f(800.f, 20.f));
//    ceilingLine.setFillColor(Color::Red);
//    ceilingLine.setPosition(0.f, 40.f);
//
//    RectangleShape floorLine(Vector2f(800.f, 20.f));
//    floorLine.setFillColor(Color::Red);
//    floorLine.setPosition(0.f, 540.f);
//
//    window.draw(ceilingLine);
//    window.draw(floorLine);
//
//    for (const auto& stripe : stripes)
//    {
//        RectangleShape topStripe(Vector2f(20.f, 20.f));
//        topStripe.setFillColor(Color::Yellow);
//        topStripe.setPosition(stripe.x, 40.f);
//
//        RectangleShape bottomStripe(Vector2f(20.f, 20.f));
//        bottomStripe.setFillColor(Color::Yellow);
//        bottomStripe.setPosition(stripe.x, 540.f);
//
//        window.draw(topStripe);
//        window.draw(bottomStripe);
//    }
//}