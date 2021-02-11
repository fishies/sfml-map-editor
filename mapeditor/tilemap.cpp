#include "tilemap.h"

#include <fstream>

TileMap::TileMap(const sf::Vector2u &mapSize, const sf::Texture &tileTexture, const sf::Vector2i &spritesz)
{
    offset = sf::Vector2f(5.0f,5.0f);

    size = mapSize;
    for(int i = 0; i < MapLayer::Count; ++i)
    {
        tiles[i] = std::vector<uint16_t>((size.x*size.y),0);
    }
    walls = std::vector<uint8_t>((size.x*size.y),0);

    tileset = tileTexture;
    spriteSize = spritesz;
    tilesPerRow = static_cast<int>(tileset.getSize().x)/spriteSize.x;

    showMidgroundPlane = false;
    midgroundPlane = sf::RectangleShape();
    midgroundPlane.setFillColor(sf::Color(0,255,255,64));
    midgroundPlane.setPosition(offset);
    midgroundPlane.setSize(sf::Vector2f(mapSize.x*spriteSize.x,mapSize.y*spriteSize.y));
}

void TileMap::saveToFile(const std::string &filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    uint8_t byte;
    //first byte: file format version
    byte = 0x00;
    ofs << byte;
    //second byte: X size of map
    byte = static_cast<uint8_t>(size.x);
    ofs << byte;
    //third byte: Y size of map
    byte = static_cast<uint8_t>(size.y);
    ofs << byte;
    //1 byte each tile representing walls
    for(auto wall : walls)
    {
        byte = wall;
        ofs << byte;
    }
    //2 bytes each tile per layer
    for(int layer = 0; layer < MapLayer::Count; ++layer)
    {
        for(unsigned int i = 0; i < size.x; ++i)
        {
            for(unsigned int j = 0; j < size.y; ++j)
            {
                uint16_t temp = tiles[layer][size.x*j+i];
                byte = temp&0x00FF;
                ofs << byte;
                byte = (temp>>8)&0x00FF;
                ofs << byte;
            }
        }
    }
}

void TileMap::loadFromFile(const std::string &filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    uint8_t byte;

    //first byte: file format version
    byte = ifs.get(); //must use .get instead of >> because 32 (space) will be treated as whitespace by >>
    if(byte != 0x00)
        return;
    //second byte: X size of map
    byte = ifs.get();
    size.x = byte;
    //third byte: Y size of map
    byte = ifs.get();
    size.y = byte;

    for(int i = 0; i < MapLayer::Count; ++i)
    {
        tiles[i].clear();
        tiles[i] = std::vector<uint16_t>((size.x*size.y),0);
    }
    walls.clear();
    walls = std::vector<uint8_t>((size.x*size.y),0);

    //1 byte each tile representing walls
    for(auto&& wall : walls)
    {
        byte = ifs.get();
        wall = byte;
    }

    //2 bytes each tile per layer
    for(int layer = 0; layer < MapLayer::Count; ++layer)
    {
        for(unsigned int i = 0; i < size.x; ++i)
        {
            for(unsigned int j = 0; j < size.y; ++j)
            {
                uint16_t temp = 0;
                byte = ifs.get();
                temp = byte;
                byte = ifs.get();
                temp |= (static_cast<uint16_t>(byte) << 8);
                tiles[layer][size.x*j+i] = temp;
            }
        }
    }
}

void TileMap::writeToPacket(sf::Packet &p)
{
    uint8_t byte;
    //first byte: file format version
    byte = 0x00;
    p << byte;
    //second byte: X size of map
    byte = static_cast<uint8_t>(size.x);
    p << byte;
    //third byte: Y size of map
    byte = static_cast<uint8_t>(size.y);
    p << byte;
    //1 byte each tile representing walls
    for(auto wall : walls)
    {
        byte = wall;
        p << byte;
    }
    //2 bytes each tile per layer
    for(int layer = 0; layer < MapLayer::Count; ++layer)
    {
        for(unsigned int i = 0; i < size.x; ++i)
        {
            for(unsigned int j = 0; j < size.y; ++j)
            {
                uint16_t temp = tiles[layer][size.x*j+i];
                byte = temp&0x00FF;
                p << byte;
                byte = (temp>>8)&0x00FF;
                p << byte;
            }
        }
    }
}

void TileMap::readFromPacket(sf::Packet &p)
{
    uint8_t byte;

    //first byte: file format version
    p >> byte;
    if(byte != 0x00)
        return;
    //second byte: X size of map
    p >> byte;
    size.x = byte;
    //third byte: Y size of map
    p >> byte;
    size.y = byte;

    for(int i = 0; i < MapLayer::Count; ++i)
    {
        tiles[i].clear();
        tiles[i] = std::vector<uint16_t>((size.x*size.y),0);
    }
    walls.clear();
    walls = std::vector<uint8_t>((size.x*size.y),0);

    //1 byte each tile representing walls
    for(auto&& wall : walls)
    {
        p >> byte;
        wall = byte;
    }

    //2 bytes each tile per layer
    for(int layer = 0; layer < MapLayer::Count; ++layer)
    {
        for(unsigned int i = 0; i < size.x; ++i)
        {
            for(unsigned int j = 0; j < size.y; ++j)
            {
                uint16_t temp = 0;
                p >> byte;
                temp = byte;
                p >> byte;
                temp |= (static_cast<uint16_t>(byte) << 8);
                tiles[layer][size.x*j+i] = temp;
            }
        }
    }
}

uint16_t TileMap::getTile(const sf::Vector2u &pos, MapLayer layer)
{
    return tiles[layer][pos.y*size.x + pos.x];
}

void TileMap::setTile(const sf::Vector2u &pos, MapLayer layer, uint16_t tileNum)
{
    tiles[layer][pos.y*size.x + pos.x] = tileNum;
}

int TileMap::fillAtTile(const sf::Vector2u &pos, MapLayer layer, uint16_t tileNum)
{
    //recursion seems to be most straightforward way to implement this... if it doesn't blow up the call stack
    return -1;
}

void TileMap::toggleMidgroundPlane()
{
    showMidgroundPlane = !showMidgroundPlane;
}

sf::FloatRect TileMap::getBoundingBox() const
{
    return sf::FloatRect(offset, sf::Vector2f(size.x*spriteSize.x,size.y*spriteSize.y));
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Sprite tile(tileset);
    for(unsigned int i = 0; i < MapLayer::Foreground0; ++i)
    {
        for(unsigned int j = 0; j < tiles[i].size(); ++j)
        {
            if(tiles[i][j])
            {
                tile.setTextureRect(sf::IntRect(sf::Vector2i(spriteSize.x*static_cast<int>((tiles[i][j]-1)%tilesPerRow),
                                                             spriteSize.y*static_cast<int>((tiles[i][j]-1)/tilesPerRow)),
                                                spriteSize));
                tile.setPosition(static_cast<unsigned int>(spriteSize.x)*(j%size.x),static_cast<unsigned int>(spriteSize.y)*(j/size.x));
                tile.move(offset);
                target.draw(tile, states);
            }
        }
    }

    if(showMidgroundPlane)
    {
        target.draw(midgroundPlane, states);
    }

    for(unsigned int i = MapLayer::Foreground0; i < MapLayer::Count; ++i)
    {
        for(unsigned int j = 0; j < tiles[i].size(); ++j)
        {
            if(tiles[i][j])
            {
                tile.setTextureRect(sf::IntRect(sf::Vector2i(spriteSize.x*static_cast<int>((tiles[i][j]-1)%tilesPerRow),
                                                             spriteSize.y*static_cast<int>((tiles[i][j]-1)/tilesPerRow)),
                                                spriteSize));
                tile.setPosition(static_cast<unsigned int>(spriteSize.x)*(j%size.x),static_cast<unsigned int>(spriteSize.y)*(j/size.x));
                tile.move(offset);
                target.draw(tile, states);
            }
        }
    }
}
