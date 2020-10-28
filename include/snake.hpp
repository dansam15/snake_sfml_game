#ifndef SNAKE_SNAKE
#define SNAKE_SNAKE

#include <deque>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace SnakeNamespace
{
	enum { BLOCK_SIZE = 20 };
	enum { SPACE_ENC = 0, FOOD_ENC, BORD_ENC, FIELD_ENC, SELF_ENC, ERROR_ENC };//encounting flags
	 /*
	void make_game_block(sf::ConvexShape& block, sf::Color color = sf::Color::Transparent, int size = BLOCK_SIZE)
	{	
		block.setPointCount(4);
		block.setFillColor(color);
		block.setOutlineColor(sf::Color::Red);
		block.setOutlineThickness(1);
		block.setPoint(0, sf::Vector2f(0, 0));
		block.setPoint(1, sf::Vector2f(size, 0));
		block.setPoint(2, sf::Vector2f(size, size));
		block.setPoint(3, sf::Vector2f(0, size));
	};
*/

	class Snake
	{
		public:
		Snake(const sf::Vector2f& start_pos);
		
		std::deque<sf::ConvexShape> getBody() const { return body;}; 
		void turn(int direction);
		int move(const sf::Vector2f& food_pos, const std::vector<sf::ConvexShape>& borders, const sf::Vector2f& game_field_size);

		private:
		int check_position(const sf::Vector2f& food_pos, const std::vector<sf::ConvexShape>& borders, const sf::Vector2f& game_field_size);
		void grow() { body.push_back(body.back()); };
		
		std::deque<sf::ConvexShape> body;
		int direction = sf::Keyboard::Right;
	};
}

//extern void SnakeNamespace::make_game_block(sf::ConvexShape& block, sf::Color color = sf::Color::Transparent, int size = SnakeNamespace::BLOCK_SIZE);

#endif

