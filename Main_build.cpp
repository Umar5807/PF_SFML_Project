//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <cstdlib>
//#include <ctime>
//using namespace sf;
//using namespace std;
//const int WIN_W = 800, WIN_H = 600;
//const float FLOOR_Y = 500.f, CEILING_Y = 60.f,PLAYER_X = 100.f, BLOCK_SIZE = 40.f, GRAVITY = 0.6f;
//// stripe k lia
//struct Stripe
//{
//    float x;
//};
//struct Obstacle
//{
//    float x;
//    bool onFloor;
//};
//RectangleShape player(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
//float velocityY = 0.f, scrollSpeed = 5.f;
//bool isGravityDown = true;
//vector<Stripe> stripes;
//vector<Obstacle> obstacles;
//int spawnTimer = 0;
//int nextSpawn = 90;
//void setupGame();
//void handleEvents(RenderWindow& window);
//void updateStripes();
//void drawTrack(RenderWindow& window);
//void updatePlayer();
//void drawPlayer(RenderWindow& window);
//void spawnObstacle();
//void updateObstacles();
//void drawObstacles(RenderWindow& window);
//int main()
//{
//    srand((unsigned)time(NULL));
//    RenderWindow window(VideoMode(WIN_W, WIN_H), "Gravity Flip Runner - Infinite Loop");
//    window.setFramerateLimit(60);
//    setupGame();
//    while (window.isOpen())
//    {
//        handleEvents(window);
//        updatePlayer();
//        updateStripes();
//        updateObstacles();
//        window.clear(Color::Black);
//        drawTrack(window);
//        drawObstacles(window);
//        drawPlayer(window);
//        window.display();
//    }
//    return 0;
//}
////player colour, start position aur stripes
//void setupGame()
//{
//    player.setFillColor(Color::Cyan);
//    player.setPosition(PLAYER_X, FLOOR_Y);
//    for (float i = 0; i < 800.f;i += 100.f)
//    {
//        stripes.push_back({ i });
//    }
//}
//
//// tofilp gravity  ;
//void handleEvents(RenderWindow& window)
//{
//    Event event;
//    while (window.pollEvent(event))
//    {
//        if (event.type == Event::Closed)
//            window.close();
//        if (event.type ==Event::KeyPressed && event.key.code ==Keyboard::Space){
//            isGravityDown = !isGravityDown;
//        }
//    }
//}
////screen move and send stripes again on screen
//void updateStripes()
//{
//    for (auto& stripe : stripes){
//        stripe.x -= scrollSpeed;
//        if (stripe.x < -20.f)
//        {
//            stripe.x += 820.f;
//        }
//    }
//}
////celing and flor lines
//void drawTrack(RenderWindow& window)
//{
//    RectangleShape ceilingLine(Vector2f(800.f, 20.f));
//    ceilingLine.setFillColor(Color::Red);
//    ceilingLine.setPosition(0.f, 40.f);
//    RectangleShape floorLine(Vector2f(800.f, 20.f));
//    floorLine.setFillColor(Color::Red);
//    floorLine.setPosition(0.f, 540.f);
//    window.draw(ceilingLine);
//    window.draw(floorLine);
//    for (const auto& stripe : stripes){
//        RectangleShape topStripe(Vector2f(20.f, 20.f));
//        topStripe.setFillColor(Color::Yellow);
//        topStripe.setPosition(stripe.x, 40.f);
//        RectangleShape bottomStripe(Vector2f(20.f, 20.f));
//        bottomStripe.setFillColor(Color::Yellow);
//        bottomStripe.setPosition(stripe.x, 540.f);
//        window.draw(topStripe);
//        window.draw(bottomStripe);
//    }
//}
////for gravity;
//void updatePlayer()
//{
//    if (isGravityDown)
//    {
//        velocityY += GRAVITY;
//        player.move(0.f, velocityY);
//        // floor check;
//        if (player.getPosition().y >=FLOOR_Y)
//        {
//            player.setPosition(player.getPosition().x, FLOOR_Y);
//            velocityY = 0.f;
//        }
//    }
//    else
//    {
//        velocityY -= GRAVITY;
//        player.move(0.f, velocityY);
//        // ceiling check
//        if (player.getPosition().y <= CEILING_Y){
//            player.setPosition(player.getPosition().x, CEILING_Y);
//            velocityY = 0.f;
//        }
//    }
//}
//void drawPlayer(RenderWindow& window)
//{
//    window.draw(player);
//}
//// adds one obstacle at the right edge, randomly on the floor and the ceiling
//void spawnObstacle()
//{
//    Obstacle newObstacle;
//    newObstacle.x = WIN_W + 20.f;
//    newObstacle.onFloor = (rand() % 2 == 0);
//    obstacles.push_back(newObstacle);
//}
//// Yeh function kuch random intezar ke baad naya obstacle banata hai, tamam obstacles ko left taraf hilata hai, aur jo obstacles screen se bahar nikal jaayein unhein delete kar deta hai
//void updateObstacles()
//{
//    spawnTimer++;
//    if (spawnTimer >= nextSpawn)
//    {
//        spawnObstacle();
//        spawnTimer = 0;
//        nextSpawn = 80 + rand() % 70;   // next wait is between 80 and 150 frames
//    }
//    // loop goes backwards so erasing doesn't skip any obstacle
//    for (int i = (int)obstacles.size() - 1; i >= 0; i--)
//    {
//        obstacles[i].x -= scrollSpeed;
//        if (obstacles[i].x < -BLOCK_SIZE)
//        {
//            obstacles.erase(obstacles.begin() + i);
//        }
//    }
//}
//// draws every obstacle as an orange block on the floor and the ceiling
//void drawObstacles(RenderWindow& window)
//{
//    for (const auto& obs : obstacles)
//    {
//        RectangleShape block(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
//        block.setFillColor(Color(255, 140, 0));
//        if (obs.onFloor)
//            block.setPosition(obs.x, FLOOR_Y);
//        else
//            block.setPosition(obs.x, CEILING_Y);
//        window.draw(block);
//    }
//}