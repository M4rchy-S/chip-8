#include <SFML/Graphics.hpp>
#include "CHIP8.h"

int main()
{
    CHIP8 chip;
    const unsigned int TILE_SIZE = 20;

    if (chip.load_file("IBM.ch8") == false)
    //if (chip.load_file("Chip8Picture.ch8") == false)
    //if (chip.load_file("Minimalgame.ch8") == false)
    //if (chip.load_file("RandomNumberTest.ch8") == false)
    {
        std::cout << "Error to load a file" << std::endl;
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode({ TILE_SIZE * WIDTH, TILE_SIZE * HEIGHT}), "CHIP-8 Emulator");

    while (window.isOpen())
    {
        //  Logic Events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        //  User Inputs

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1))
            chip.setKeyStatus(0, 1);
        else
            chip.setKeyStatus(0, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2))
            chip.setKeyStatus(1, 1);
        else
            chip.setKeyStatus(1, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3))
            chip.setKeyStatus(2, 1);
        else
            chip.setKeyStatus(2, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4))
            chip.setKeyStatus(3, 1);
        else
            chip.setKeyStatus(3, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Q))
            chip.setKeyStatus(4, 1);
        else
            chip.setKeyStatus(4, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
            chip.setKeyStatus(5, 1);
        else
            chip.setKeyStatus(5, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E) )
            chip.setKeyStatus(6, 1);
        else
            chip.setKeyStatus(6, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::R))
            chip.setKeyStatus(7, 1);
        else
            chip.setKeyStatus(7, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
            chip.setKeyStatus(8, 1);
        else
            chip.setKeyStatus(8, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
            chip.setKeyStatus(9, 1);
        else
            chip.setKeyStatus(9, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
            chip.setKeyStatus(10, 1);
        else
            chip.setKeyStatus(10, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F))
            chip.setKeyStatus(11, 1);
        else
            chip.setKeyStatus(11, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z))
            chip.setKeyStatus(12, 1);
        else
            chip.setKeyStatus(12, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::X))
            chip.setKeyStatus(13, 1);
        else
            chip.setKeyStatus(13, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::C))
            chip.setKeyStatus(14, 1);
        else
            chip.setKeyStatus(14, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::V))
            chip.setKeyStatus(15, 1);
        else
            chip.setKeyStatus(15, 0);




        //  Chip Logic

        chip.update();

        //      Displaying
        window.clear();

        for (float i = 0; i < WIDTH; i++)
        {
            for (float j = 0; j < HEIGHT; j++)
            {
                sf::RectangleShape rect({ TILE_SIZE, TILE_SIZE });

                if (chip.get_display(i, j) == 1)
                    rect.setFillColor(sf::Color(32, 42, 53));
                else
                    rect.setFillColor(sf::Color(143, 145, 133));

                rect.setPosition({i*TILE_SIZE, j*TILE_SIZE});

                window.draw(rect);

            }
        }
        
        window.display();
    }

    return 0;
}