#include "snake.hpp"


//extern void SnakeNamespace::make_game_block(sf::ConvexShape& block, sf::Color color, int size);
SnakeNamespace::Snake::Snake(const sf::Vector2f& start_pos)
{

	//FIXME
	sf::ConvexShape head;
	//SnakeNamespace::make_game_block(head, sf::Color::White);
	sf::Color color = sf::Color::White; int size = SnakeNamespace::BLOCK_SIZE;
	head.setPointCount(4);
	head.setFillColor(color);
	head.setOutlineColor(sf::Color::Red);
	head.setOutlineThickness(1);
	head.setPoint(0, sf::Vector2f(0, 0));
	head.setPoint(1, sf::Vector2f(size, 0));
	head.setPoint(2, sf::Vector2f(size, size));
	head.setPoint(3, sf::Vector2f(0, size));

	head.setPosition(start_pos);
	body.push_back(head);
	
}

int SnakeNamespace::Snake::move(const sf::Vector2f& food_pos, const std::vector<sf::ConvexShape>& borders, const sf::Vector2f& game_field_size)
{
	body.push_front(body.front());
	
	switch (direction)
	{
		case (sf::Keyboard::Left):
		{
			body.front().setPosition(body.front().getPosition().x - SnakeNamespace::BLOCK_SIZE, body.front().getPosition().y);
			break;
		}
		case (sf::Keyboard::Right):
		{
			body.front().setPosition(body.front().getPosition().x + SnakeNamespace::BLOCK_SIZE, body.front().getPosition().y);
			break;
		}
		case (sf::Keyboard::Up):
		{
			body.front().setPosition(body.front().getPosition().x, body.front().getPosition().y - SnakeNamespace::BLOCK_SIZE);
			break;
		}
		case (sf::Keyboard::Down):
		{
			body.front().setPosition(body.front().getPosition().x, body.front().getPosition().y + SnakeNamespace::BLOCK_SIZE);
			break;
		}
		default:
		{
			return 1;
		}
	};
	
	int retval = check_position(food_pos, borders, game_field_size);

	body.pop_back();
	
	return retval;
}

void SnakeNamespace::Snake::turn(int direction)
{
	
	if 
	(
		(this->direction == sf::Keyboard::Left)  && (direction != sf::Keyboard::Right) || 
		(this->direction == sf::Keyboard::Right) && (direction != sf::Keyboard::Left)  ||
		(this->direction == sf::Keyboard::Up)    && (direction != sf::Keyboard::Down)  ||
		(this->direction == sf::Keyboard::Down)  && (direction != sf::Keyboard::Up) 
	)
		this->direction = direction;
}

int SnakeNamespace::Snake::check_position(const sf::Vector2f& food_pos, const std::vector<sf::ConvexShape>& borders, const sf::Vector2f& game_field_size)
{
	//TODO named functions?
	//check for food
	if ((body.front().getPosition().x == food_pos.x) && (body.front().getPosition().y == food_pos.y))
		{   
			grow();
			return SnakeNamespace::FOOD_ENC;
		}
	
	//check for borders
	for (auto it: borders)
		if ((it).getPosition().x == body.front().getPosition().x && (it).getPosition().y == body.front().getPosition().y)
		{
			return SnakeNamespace::BORD_ENC;		
		}
		
	if ((body.front().getPosition().x < 0) ||  (body.front().getPosition().x > (game_field_size.x - SnakeNamespace::BLOCK_SIZE)) || (body.front().getPosition().y < 0) ||  
	(body.front().getPosition().y > (game_field_size.y - SnakeNamespace::BLOCK_SIZE)))
		{
			return SnakeNamespace::FIELD_ENC;  
		}
			
	//check for self-eating
	//FIXME lol
	auto body_tmp = body;
	body_tmp.pop_front();
	for (auto it: body_tmp)
		if ((it).getPosition().x == body.front().getPosition().x && (it).getPosition().y == body.front().getPosition().y)
		{
			return SnakeNamespace::SELF_ENC;			
		}
		
	return SPACE_ENC;
}
