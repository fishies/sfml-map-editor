#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <cstdint>

enum MapLayer
{
    Background0 = 0
    ,Background1
    ,Background2
    ,Background3
    ,Foreground0
    ,Foreground1
    ,Foreground2
    ,Foreground3
    ,Count
};

class TileMap : public sf::Drawable
{
    sf::Vector2f offset;

    sf::Vector2u size;
    sf::Texture tileset;
    sf::Vector2i spriteSize;
    int tilesPerRow;

    std::vector<uint16_t> tiles[MapLayer::Count];
    std::vector<uint8_t> walls;

    bool showMidgroundPlane;
    sf::RectangleShape midgroundPlane;

public:
    TileMap(const sf::Vector2u &mapSize, const sf::Texture &tileTexture, const sf::Vector2i &spritesz = sf::Vector2i(32,32));

    void saveToFile(const std::string &filename);
    void loadFromFile(const std::string &filename);

    void writeToPacket(sf::Packet &p);
    void readFromPacket(sf::Packet &p);

    uint16_t getTile(const sf::Vector2u &pos, MapLayer layer);
    void setTile(const sf::Vector2u &pos, MapLayer layer, uint16_t tileNum);
    int fillAtTile(const sf::Vector2u &pos, MapLayer layer, uint16_t tileNum); // returns number of tiles filled
    void toggleMidgroundPlane();

    sf::FloatRect getBoundingBox() const;

    void draw(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const;
};

#endif // TILEMAP_H
