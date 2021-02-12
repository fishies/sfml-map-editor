#ifndef TILEPALETTE_H
#define TILEPALETTE_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class TilePalette : public sf::Drawable
{
    sf::Vector2f offset;

    const int tilesPerRow = 7;
    const int rowsPerPage = 11;

    sf::Texture tileset;
    sf::Sprite page;

    int currentRow;
    std::vector<int> bookmarkedRows;

    int selectedTile;

    sf::Vector2i spriteSize; //in pixels
    sf::Vector2i tilesetSize; //in pixels
    sf::Vector2i pageSize; //in pixels

public:
    TilePalette(const sf::Texture &tileTexture, const sf::Vector2i &spriteSize = sf::Vector2i(32,32));

    bool jumpToRow(int row);
    bool rowUp();
    bool rowDown();
    void pageUp();
    void pageDown();
    void firstPage();
    void lastPage();

    int selectTile(const sf::Vector2i &mousePos);
    void deselectTile();
    int getSelected() const;

    sf::FloatRect getBoundingBox() const;

    void draw(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const;
};

#endif // TILEPALETTE_H
