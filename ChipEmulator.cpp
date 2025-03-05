#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <nfd.h>

#include "CHIP8.h"

#include <vector>
#include <chrono>
#include <thread>

CHIP8 chip;
const unsigned int TILE_SIZE = 15;

std::vector<std::vector<sf::Color>> colorPalette = 
{ 
    { sf::Color(32, 42, 53), sf::Color(143, 145, 133) },
    { sf::Color(143, 145, 133), sf::Color(32, 42, 53) },
    { sf::Color(182, 124, 36), sf::Color(27, 30, 37)  } 
};

unsigned short pallete_index = 0;

void handle_user_input();

std::string OpenFileDialog();

int main()
{
    if (chip.load_file("Chip8Picture.ch8") == false)
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
        handle_user_input();

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
                    rect.setFillColor(colorPalette[pallete_index][0]);
                else
                    rect.setFillColor(colorPalette[pallete_index][1]);

                rect.setPosition({i*TILE_SIZE, j*TILE_SIZE});

                window.draw(rect);

            }
        }
        
        window.display();
    }

    return 0;
}


void handle_user_input()
{
    //  --- Program settings buttons ---

    //  Restart program
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F1))
    {
        chip.reset_program();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    //  Load program path
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F2))
    {
        chip.load_file( OpenFileDialog() );
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    //  Change theme
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F3))
    {
        pallete_index = (pallete_index + 1) % colorPalette.size();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }


    //  Main 16 input 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1))
        chip.setKeyStatus(1, 1);
    else
        chip.setKeyStatus(1, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2))
        chip.setKeyStatus(2, 1);
    else
        chip.setKeyStatus(2, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3))
        chip.setKeyStatus(3, 1);
    else
        chip.setKeyStatus(3, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4))
        chip.setKeyStatus(12, 1);
    else
        chip.setKeyStatus(12, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Q))
        chip.setKeyStatus(4, 1);
    else
        chip.setKeyStatus(4, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
        chip.setKeyStatus(5, 1);
    else
        chip.setKeyStatus(5, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
        chip.setKeyStatus(6, 1);
    else
        chip.setKeyStatus(6, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::R))
        chip.setKeyStatus(13, 1);
    else
        chip.setKeyStatus(13, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
        chip.setKeyStatus(7, 1);
    else
        chip.setKeyStatus(7, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
        chip.setKeyStatus(8, 1);
    else
        chip.setKeyStatus(8, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
        chip.setKeyStatus(9, 1);
    else
        chip.setKeyStatus(9, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F))
        chip.setKeyStatus(14, 1);
    else
        chip.setKeyStatus(14, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z))
        chip.setKeyStatus(10, 1);
    else
        chip.setKeyStatus(10, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::X))
        chip.setKeyStatus(0, 1);
    else
        chip.setKeyStatus(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::C))
        chip.setKeyStatus(11, 1);
    else
        chip.setKeyStatus(11, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::V))
        chip.setKeyStatus(15, 1);
    else
        chip.setKeyStatus(15, 0);

}

std::string OpenFileDialog() {
    nfdchar_t* outPath = nullptr;
    nfdfilteritem_t filterItem[2] = { { "Chip8 Roms", "ch8" } };

    if (NFD_OpenDialog(&outPath, filterItem, 1, NULL) == NFD_OKAY) {
        std::string filePath(outPath);
        free(outPath);
        return filePath;
    }

    free(outPath);
    return "";
}