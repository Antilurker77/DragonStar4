// ================================================================
//
// Dragon Star Project
// v0.4.0
//
// ================================================================

#include <memory>
#include "core/game.hpp"
#include "data/gameData.hpp"

int main() {
	gameData.LoadData();

	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}

//#include <SFML/Graphics.hpp>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }
//
//    return 0;
//}