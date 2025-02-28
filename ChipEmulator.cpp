#include <SFML/Graphics.hpp>
#include "CHIP8.h"

int main()
{
    CHIP8 chip;

    if (chip.load_file("IBM.ch8") == false)
    {
        std::cout << "Error to load a file" << std::endl;
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode({ 1200, 720}), "SFML works!");

    sf::RectangleShape shape({100, 100});
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({150, 150});

    while (window.isOpen())
    {
        //  Logic Events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        //  Chip Logic

        chip.update();
        

        //      Displaying
        window.clear();

        window.draw(shape);
        
        window.display();
    }

    return 0;
}