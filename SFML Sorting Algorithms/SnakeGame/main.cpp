#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Visual Sorting Algorithms");

    // Create an instance of the SFML CircleShape and initialise it so radius is 100
    sf::CircleShape shape(100.f);

    // Set the shape's fill colour attribute to Green
    shape.setFillColor(sf::Color::Green);

    // We can still output to the console window
    std::cout << "SnakeGame: Starting" << std::endl;

    // Main loop that continues until we call window.close()
    while (window.isOpen())
    {
        // Handle any pending SFML events
        // These cover keyboard, mouse,joystick etc.
        sf::Event event;
        while (window.pollEvent(event))
        {
          switch(event.type)
          {
            case sf::Event::Closed:
              window.close();
            break;
            default:
              break;
          }
        }

        // We must clear the window each time around the loop
        window.clear();

        // draw our circle shape to the window
        window.draw(shape);

        // Get the window to display its contents
        window.display();
    }

    std::cout << "SnakeGame: Finished" << std::endl;

    return 0;
}
