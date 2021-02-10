#include "tilepalette.h"

TilePalette::TilePalette(const sf::Texture &tileTexture, const sf::Vector2i &spritesz)
{
    tileset = tileTexture;
    currentRow = 0;
    selectedTile = 0;
    spriteSize = spritesz;
    tilesetSize = sf::Vector2i(tileset.getSize());
    pageSize = sf::Vector2i(tilesetSize.x, spriteSize.y * rowsPerPage);
    page = sf::Sprite(tileset, sf::IntRect(sf::Vector2i(0,0),pageSize));

    offset = sf::Vector2f(810.0f, 5.0f);
    page.move(offset);
}

bool TilePalette::jumpToRow(int row)
{
    if(row < 0 || row > ((tilesetSize.y-pageSize.y)/spriteSize.y))
    {
        return false;
    }
    currentRow = row;
    page.setTextureRect(sf::IntRect(sf::Vector2i(0,32*currentRow),pageSize));
    return true;
}

bool TilePalette::rowUp()
{
    if(currentRow > 0)
    {
        --currentRow;
        page.setTextureRect(sf::IntRect(sf::Vector2i(0,32*currentRow),pageSize));
        return true;
    }
    return false;
}

bool TilePalette::rowDown()
{
    if(currentRow < ((tilesetSize.y-pageSize.y)/spriteSize.y))
    {
        ++currentRow;
        page.setTextureRect(sf::IntRect(sf::Vector2i(0,32*currentRow),pageSize));
        return true;
    }
    return false;
}

void TilePalette::pageUp()
{
    currentRow -= rowsPerPage;
    if(currentRow < 0)
    {
        currentRow = 0;
    }
    page.setTextureRect(sf::IntRect(sf::Vector2i(0,32*currentRow),pageSize));
}

void TilePalette::pageDown()
{
    currentRow += rowsPerPage;
    if(currentRow > ((tilesetSize.y-pageSize.y)/spriteSize.y))
    {
        currentRow = ((tilesetSize.y-pageSize.y)/spriteSize.y);
    }
    page.setTextureRect(sf::IntRect(sf::Vector2i(0,32*currentRow),pageSize));
}

void TilePalette::firstPage()
{
    currentRow = 0;
    page.setTextureRect(sf::IntRect(sf::Vector2i(0,32*currentRow),pageSize));
}

void TilePalette::lastPage()
{
    currentRow = ((tilesetSize.y-pageSize.y)/spriteSize.y);
    page.setTextureRect(sf::IntRect(sf::Vector2i(0,32*currentRow),pageSize));
}

int TilePalette::selectTile(const sf::Vector2i &mousePos)
{
    int result = 1;
    result += currentRow * (tilesetSize.x/spriteSize.x);

    result += (static_cast<int>(mousePos.x-page.getPosition().x)/spriteSize.x);
    result += (static_cast<int>(mousePos.y-page.getPosition().y)/spriteSize.y) * (tilesetSize.x/spriteSize.x);

    selectedTile = result;
    return selectedTile;
}

void TilePalette::deselectTile()
{
    selectedTile = 0;
}

int TilePalette::getSelected() const
{
    return selectedTile;
}

sf::FloatRect TilePalette::getBoundingBox() const
{
    return page.getGlobalBounds();
}

void TilePalette::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(page, states);

    if(selectedTile)
    {
        sf::Sprite selectedSprite(tileset);
        selectedSprite.setTextureRect(sf::IntRect(sf::Vector2i(spriteSize.x*static_cast<int>((selectedTile-1)%tilesPerRow),
                                                               spriteSize.y*static_cast<int>((selectedTile-1)/tilesPerRow)), spriteSize));
        selectedSprite.setPosition(offset);
        selectedSprite.move(0, pageSize.y+5.0f);
        target.draw(selectedSprite, states);
    }
}
