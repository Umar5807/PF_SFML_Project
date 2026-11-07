//#include <SFML/Graphics.hpp>
//#include <vector>
//using namespace sf;
//using namespace std;
//int main()
//{
//    // 1. Create the game window (800x600 resolution)
//    RenderWindow window(VideoMode(800, 600), "Gravity Flip Runner - Infinite Loop");
//    window.setFramerateLimit(60);
//
//    // 2. Create the player block
//    RectangleShape player(Vector2f(40.f, 40.f));
//    player.setFillColor(    Color::Cyan);
//    player.setPosition(100.f, 500.f); // Start near the floor
//
//    // Physics variables
//    float velocityY = 0.f;
//    float gravity = 0.6f;
//    bool isGravityDown = true;
//
//    // Boundaries
//    float floorY = 500.f;
//    float ceilingY = 60.f;
//
//    // 3. Infinite Scrolling Track Setup (Stripes on ceiling/floor)
//    struct Stripe {
//        float x;
//    };
//    std::vector<Stripe> stripes;
//    for (float i = 0; i < 800.f; i += 100.f) {
//        stripes.push_back({ i });
//    }
//    float scrollSpeed = 5.f; // Speed of the infinite loop
//
//    while (window.isOpen())
//    {
//        // Handle window events
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            // Flip gravity when Spacebar is pressed
//            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
//            {
//                isGravityDown = !isGravityDown;
//            }
//        }
//
//        // Apply gravity movement
//        if (isGravityDown)
//        {
//            velocityY += gravity;
//            player.move(0.f, velocityY);
//
//            // Floor collision check
//            if (player.getPosition().y >= floorY)
//            {
//                player.setPosition(player.getPosition().x, floorY);
//                velocityY = 0.f;
//            }
//        }
//        else
//        {
//            velocityY -= gravity;
//            player.move(0.f, velocityY);
//
//            // Ceiling collision check
//            if (player.getPosition().y <= ceilingY)
//            {
//                player.setPosition(player.getPosition().x, ceilingY);
//                velocityY = 0.f;
//            }
//        }
//
//        // Update infinite scrolling track stripes
//        for (auto& stripe : stripes)
//        {
//            stripe.x -= scrollSpeed;
//            if (stripe.x < -20.f) // Once it goes off the left side of the screen
//            {
//                stripe.x += 820.f; // Loop it back around to the right side
//            }
//        }
//
//        // Render everything
//        window.clear(Color::Black);
//
//        // Draw ceiling and floor base lines
//        RectangleShape ceiling(Vector2f(800.f, 20.f));
//        ceiling.setFillColor(Color::Red);
//        ceiling.setPosition(0.f, 40.f);
//
//        RectangleShape floor(Vector2f(800.f, 20.f));
//        floor.setFillColor(Color::Red);
//        floor.setPosition(0.f, 540.f);
//
//        window.draw(ceiling);
//        window.draw(floor);
//
//        // Draw the scrolling track markers (yellow stripes) to create motion illusion
//        for (const auto& stripe : stripes)
//        {
//            RectangleShape topStripe(Vector2f(20.f, 20.f));
//            topStripe.setFillColor(Color::Yellow);
//            topStripe.setPosition(stripe.x, 40.f);
//
//           RectangleShape bottomStripe(Vector2f(20.f, 20.f));
//            bottomStripe.setFillColor(Color::Yellow);
//            bottomStripe.setPosition(stripe.x, 540.f);
//
//            window.draw(topStripe);
//            window.draw(bottomStripe);
//        }
//
//        // Draw player
//        window.draw(player);
//
//        window.display();
//    }
//
//    return 0;
//}