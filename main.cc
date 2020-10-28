#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <array>
#include <string>
#include <iostream>

#include "snake.hpp"

//extern void SnakeNamespace::make_game_block(sf::ConvexShape& block, sf::Color color, int size);
enum { FRAME_LIMIT = 60 };
enum exit_code { SUCCESS = 0, ENCOUNTING, WTF };

constexpr int GameFieldWidth = 16 * SnakeNamespace::BLOCK_SIZE;
constexpr int GameFieldHeigth = 20 * SnakeNamespace::BLOCK_SIZE;
const int GameFieldBit = 8;
	
class GameSession
{
	public:
	GameSession();
	
	bool is_window_open(void) { return window.isOpen(); };
	bool window_poll_event(sf::Event& event) { return window.pollEvent(event); };
	
	void step();
	
	void draw_game(void);
	void finish_game(const int err_code);
		
	void turn_snake(int direction) { snake.turn(direction); };

	private:	
	void generate_food();
	bool wait_for_any_key_press(void);
	
	sf::RenderWindow window;
		   
	SnakeNamespace::Snake snake;
	std::vector<sf::ConvexShape> borders;
    sf::ConvexShape food;	

	std::array<std::pair<sf::SoundBuffer, sf::Sound>, SnakeNamespace::ERROR_ENC> effects;

	//FIXME
	sf::Text score_text, game_end_text;
	std::string score_str;
	int score_int;
	
	sf::Font font;
};

GameSession::GameSession(): window(sf::VideoMode(GameFieldWidth, GameFieldHeigth, GameFieldBit), "Snake"), snake(sf::Vector2f(GameFieldWidth/2, GameFieldHeigth/2)), score_int(0), food()
{
	generate_food();
    window.setFramerateLimit(FRAME_LIMIT);

	//effects[SnakeNamespace::SPACE_ENC].first.loadFromFile("/home/samir_d/cpp_projects/snake/zap10.ogg");
	effects[SnakeNamespace::FOOD_ENC].first.loadFromFile("/home/samir_d/cpp_projects/snake/shot.ogg");
	effects[SnakeNamespace::BORD_ENC].first.loadFromFile("/home/samir_d/cpp_projects/snake/confusion.ogg");
	effects[SnakeNamespace::SELF_ENC].first.loadFromFile("/home/samir_d/cpp_projects/snake/confusion.ogg");
	effects[SnakeNamespace::FIELD_ENC].first.loadFromFile("/home/samir_d/cpp_projects/snake/confusion.ogg");

	for (int i = SnakeNamespace::SPACE_ENC; i != SnakeNamespace::ERROR_ENC; i++)
		effects[i].second.setBuffer(effects[i].first);
		
	sf::ConvexShape block;
	//SnakeNamespace::make_game_block(block);
	sf::Color color = sf::Color::Transparent; int size = SnakeNamespace::BLOCK_SIZE;

	block.setPointCount(4);
	block.setFillColor(color);
	block.setOutlineColor(sf::Color::Red);
	block.setOutlineThickness(1);
	block.setPoint(0, sf::Vector2f(0, 0));
	block.setPoint(1, sf::Vector2f(size, 0));
	block.setPoint(2, sf::Vector2f(size, size));
	block.setPoint(3, sf::Vector2f(0, size));
		
	//TODO patterns
	for (int y = 3 * SnakeNamespace::BLOCK_SIZE; y < (GameFieldHeigth - GameFieldHeigth %  SnakeNamespace::BLOCK_SIZE); y += SnakeNamespace::BLOCK_SIZE)
	{
		block.setPosition(0, y);
		borders.push_back(block);
		block.setPosition(GameFieldWidth - SnakeNamespace::BLOCK_SIZE, y);
		borders.push_back(block);
	}
	
	for (int x = 0 ; x < (GameFieldWidth - GameFieldWidth % SnakeNamespace::BLOCK_SIZE); x+= SnakeNamespace::BLOCK_SIZE)
	{		
		block.setPosition(x, 3 * SnakeNamespace::BLOCK_SIZE);
		borders.push_back(block);
		block.setPosition(x, GameFieldHeigth - SnakeNamespace::BLOCK_SIZE);
		borders.push_back(block);
	}

	font.loadFromFile("/home/samir_d/cpp_projects/snake/SHPinscher-Regular.otf");
	 	
	score_text.setCharacterSize(20);
	score_text.setFont(font);
	score_text.setFillColor(sf::Color::Red);
	score_text.setPosition(sf::Vector2f(GameFieldWidth/2, SnakeNamespace::BLOCK_SIZE));
	
	game_end_text = score_text;
	game_end_text.setPosition(sf::Vector2f(SnakeNamespace::BLOCK_SIZE, SnakeNamespace::BLOCK_SIZE));
	
    score_str = "Score = ";
	score_str += std::to_string(score_int);
	score_text.setString(score_str.c_str());	
		
	food.setPointCount(4);
	food.setFillColor(sf::Color::Green);
	food.setOutlineColor(sf::Color::Red);
	food.setOutlineThickness(1);

	food.setPoint(0, sf::Vector2f( 0, 0 ));
	food.setPoint(1, sf::Vector2f( SnakeNamespace::BLOCK_SIZE, 0 ));
	food.setPoint(2, sf::Vector2f( SnakeNamespace::BLOCK_SIZE, SnakeNamespace::BLOCK_SIZE ));
	food.setPoint(3, sf::Vector2f( 0, SnakeNamespace::BLOCK_SIZE ));	
}

bool GameSession::wait_for_any_key_press(void) 
{
	sf::Event event;
	while (window.waitEvent(event))
	{ 
		if (event.type == sf::Event::EventType::KeyPressed && event.key.code == sf::Keyboard::Escape) //TODO rename?
			return true;
	}
	return false;
}

void GameSession::draw_game(void) 
{	
	window.clear();
	
	window.draw(score_text);
	window.draw(game_end_text);
	window.draw(food);
	
	for (auto iter: snake.getBody()) 
		window.draw(iter);
			
	for (auto iter: borders)
		window.draw(iter);

	window.display();
};

void GameSession::finish_game(const int err_code)
{
	std::cerr << err_code << std::endl;	
	
	switch (err_code)
	{
		case SUCCESS:
		{
			game_end_text.setString("NICE GAME\n");
			break;
		}
		case ENCOUNTING:
		{
			game_end_text.setString("AHAHA\n");
			break;
		}
		case WTF:
		{
			game_end_text.setString("WTF!!!\n");
			break;
		}
		default:
		{
			exit(err_code);
		}
	}
	window.draw(game_end_text);
	window.display();
	
	if (wait_for_any_key_press())
		window.close();	
	
	exit(err_code);
}

void GameSession::step(void)
{
	int move_ret_code = snake.move(food.getPosition(), borders, sf::Vector2f(GameFieldWidth, GameFieldHeigth));
	effects[move_ret_code].second.play();
	
	switch (move_ret_code)
	{
		case SnakeNamespace::SPACE_ENC:
		{
			break;
		}
		case SnakeNamespace::FOOD_ENC:
		{				
			score_int = snake.getBody().size() - 1;
			score_str = "Score = ";
			score_str += std::to_string(score_int);
			score_text.setString(score_str.c_str());
			generate_food();
			break;
		}
		case SnakeNamespace::FIELD_ENC:
		{	
			//TODO teleport
			break;
		}
		case SnakeNamespace::SELF_ENC:
		case SnakeNamespace::BORD_ENC:
		{
			finish_game(ENCOUNTING);
			break;
		}
		default:
		{
			exit(1);
		}
	}	
	
	//effects[move_ret_code].second.stop();		
}

void GameSession::generate_food(void)
{
	int rand_x;
	int rand_y;
	
	rand_x = rand() % (GameFieldWidth - SnakeNamespace::BLOCK_SIZE);
	rand_x -= rand_x % SnakeNamespace::BLOCK_SIZE;
	rand_y = rand() % (GameFieldHeigth - SnakeNamespace::BLOCK_SIZE);
	rand_y -= rand_y % SnakeNamespace::BLOCK_SIZE;
	
	
	//TODO snake check
	for (auto it: borders)//FIXME
		while ((it).getPosition().x == rand_x && (it).getPosition().y == rand_y || rand_y <= 5 * SnakeNamespace::BLOCK_SIZE) 
		{	
			rand_x = rand() % (GameFieldWidth - SnakeNamespace::BLOCK_SIZE);
			rand_x -= rand_x % SnakeNamespace::BLOCK_SIZE;
			rand_y = rand() % (GameFieldHeigth - SnakeNamespace::BLOCK_SIZE);
			rand_y -= rand_y % SnakeNamespace::BLOCK_SIZE;
		}	
	
	for (auto it: snake.getBody())
		while ((it).getPosition().x == rand_x && (it).getPosition().y == rand_y)
		{
			rand_x = rand() % (GameFieldWidth - SnakeNamespace::BLOCK_SIZE);
			rand_x -= rand_x % SnakeNamespace::BLOCK_SIZE;
			rand_y = rand() % (GameFieldHeigth - SnakeNamespace::BLOCK_SIZE);
			rand_y -= rand_y % SnakeNamespace::BLOCK_SIZE;
		}
	
	food.setPosition(rand_x, rand_y);
}


int main()
{
	//TODO selection
	const int game_speed = 100;
	
	GameSession game_session;
    
    while (game_session.is_window_open())
    {
        sf::Event event;
        while (game_session.window_poll_event(event))
        {
			if (event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code)
				{
					case(sf::Keyboard::Left):
					case(sf::Keyboard::Right):
					case(sf::Keyboard::Up):
					case(sf::Keyboard::Down):
					{
						game_session.turn_snake(event.key.code);
						break;
					}					
					case(sf::Keyboard::Escape):
					{	
						game_session.finish_game(SUCCESS); //TODO error code enum	
						break;
					}
					
					default:
					{
						break;
					}
				}
				break; // every press at once NOTICE! you may turn twice and make an forbidden turn
			}
		}

		game_session.step();
		game_session.draw_game();		
		
		sf::sleep(sf::milliseconds(game_speed));
	}
		
    return 0;
	
}
