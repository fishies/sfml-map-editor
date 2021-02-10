#include <SFML/Graphics.hpp>
#include "tilepalette.h"
#include "tilemap.h"

#include <TGUI/TGUI.hpp>

int main()
{
    sf::Texture interfaceBackground;
    interfaceBackground.loadFromFile("interface.png");
    sf::Sprite interface(interfaceBackground);

    sf::Image tilesImage;
    tilesImage.loadFromFile("tiles1.png");
    tilesImage.createMaskFromColor(sf::Color::Black);
    sf::Texture tileset;
    tileset.loadFromImage(tilesImage);

    sf::RenderWindow mainWindow(sf::VideoMode(1280,720), "MapEditor", sf::Style::Close);
    mainWindow.setFramerateLimit(300);

    TileMap tilemap(sf::Vector2u(25,16), tileset);
    TilePalette palette(tileset);

    tgui::Gui gui(mainWindow);

    tgui::Scrollbar::Ptr scrollPalette = tgui::Scrollbar::create();
    scrollPalette->setPosition(1034,5);
    scrollPalette->setSize(16,352);
    scrollPalette->setMaximum((tileset.getSize().y-(32*11))/32+1);
    scrollPalette->setScrollAmount(1);
    scrollPalette->connect("ValueChanged", [&](unsigned int newVal){palette.jumpToRow(newVal);});
    gui.add(scrollPalette);

    tgui::ComboBox::Ptr layerSelect = tgui::ComboBox::create();
    layerSelect->setPosition(847,362);
    layerSelect->setSize(187,16);
    for (int i = 0; i < MapLayer::Foreground0; ++i)
    {
        layerSelect->addItem(sf::String("Background"+std::to_string(i)),sf::String(std::to_string(i)));
    }
    for (int i = MapLayer::Foreground0; i < MapLayer::Count; ++i)
    {
        layerSelect->addItem(sf::String("Foreground"+std::to_string(i-MapLayer::Foreground0)),sf::String(std::to_string(i)));
    }
    layerSelect->setSelectedItemByIndex(0);
    gui.add(layerSelect);

    tgui::CheckBox::Ptr midgroundToggle = tgui::CheckBox::create();
    midgroundToggle->setPosition(847,378);
    midgroundToggle->setSize(16,16);
    midgroundToggle->setText("Toggle Midground Plane");
    midgroundToggle->setTextClickable(true);
    midgroundToggle->connect("Changed", [&](){tilemap.toggleMidgroundPlane();});
    gui.add(midgroundToggle);

    for(;mainWindow.isOpen();)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(mainWindow);

        for(sf::Event event; mainWindow.pollEvent(event);)
        {
            gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
            {
                mainWindow.close();
            }
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                if(palette.getBoundingBox().contains(static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)))
                {
                    if (event.mouseWheelScroll.delta > 0)
                    {
                        if (scrollPalette->getValue() > 0)
                        {
                            scrollPalette->setValue(scrollPalette->getValue()-1);
                        }
                        //palette.rowUp();
                    }
                    else
                    {
                        scrollPalette->setValue(scrollPalette->getValue()+1);
                        //palette.rowDown();
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                case sf::Keyboard::PageUp:
                    if (scrollPalette->getValue() > 10)
                    {
                        scrollPalette->setValue(scrollPalette->getValue()-11);
                    }
                    else
                    {
                        scrollPalette->setValue(0);
                    }
                    //palette.pageUp();
                    break;
                case sf::Keyboard::PageDown:
                    scrollPalette->setValue(scrollPalette->getValue()+11);
                    //palette.pageDown();
                    break;
                case sf::Keyboard::Home:
                    scrollPalette->setValue(0);
                    //palette.firstPage();
                    break;
                case sf::Keyboard::End:
                    scrollPalette->setValue(scrollPalette->getMaximum());
                    //palette.lastPage();
                    break;
                case sf::Keyboard::Insert:
                    tilemap.saveToFile("test.map");
                    break;
                case sf::Keyboard::Delete:
                    tilemap.loadFromFile("test.map");
                    break;
                default:
                    ;
                }
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if(palette.getBoundingBox().contains(event.mouseButton.x,event.mouseButton.y))
                {
                    if(event.mouseButton.button == sf::Mouse::Left)
                    {
                        palette.selectTile(sf::Vector2i(event.mouseButton.x,event.mouseButton.y));
                    }
                    else if(event.mouseButton.button == sf::Mouse::Right)
                    {
                        palette.deselectTile();
                    }
                }
            }
        }

        if(tilemap.getBoundingBox().contains(static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)))
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                tilemap.setTile(sf::Vector2u((mousePos.x-5)/32,(mousePos.y-5)/32),
                                static_cast<MapLayer>(std::stoi(layerSelect->getSelectedItemId().toAnsiString())),
                                palette.getSelected());
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                tilemap.setTile(sf::Vector2u((mousePos.x-5)/32,(mousePos.y-5)/32),
                                static_cast<MapLayer>(std::stoi(layerSelect->getSelectedItemId().toAnsiString())),
                                0);
            }
        }

        mainWindow.clear();
        mainWindow.draw(interface);
        mainWindow.draw(tilemap);
        mainWindow.draw(palette);
        gui.draw();
        mainWindow.display();
    }

    return 0;
}