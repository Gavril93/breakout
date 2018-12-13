#pragma warning (disable:4996)
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;
using namespace sf;

bool flag = true;
bool isGame = true;

class Entity
{
public:
	int health;
	float x, y;
	int w, h;
	bool life;
	Texture texture;
	Sprite sprite;
	Entity(Image &image, int Health, int X, int Y, int W, int H)
	{
		x = X, y = Y, w = W, h = H;
		health = Health;
		life = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}
	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}
	virtual void update(float time) = 0;
};

class Player : public Entity{
public:
	int score;
	float speed;
	float currentFrame;
	Player(Image &image, int health, float X,float Y,int W, int H) : Entity(image, health, X, Y, W, H)
	{
		score = 0;
		currentFrame = 0;
		sprite.setTextureRect(IntRect(0, 0, 150, 42));
		sprite.setPosition(450, 450);
	}
	void animation(float time)
	{
		currentFrame += 0.005*time;
		if (currentFrame > 3) currentFrame -= 3;
		sprite.setTextureRect(IntRect(150 * int(currentFrame), 0, 150, 42));
	}
	void checkDivine()
	{
		if (x > 850)
		{
			x = 850;
		}
		if (x < 0)
		{
			x = 0;
		}
	}
	void update(float time)
	{
		checkDivine();
		sprite.setPosition(x, y);
	}
};

class Ball : public Entity
{
public:
	float speedX, speedY;
	float dx, dy;
	Ball(Image &image, int health, int X, int Y, int W, int H) : Entity(image, health, X, Y ,W, H)
	{
		speedX = 0.2;
		speedY = 0.2;
		sprite.setPosition(507, 420);
	}
	void ballMove(float time)
	{
		if (x < 0 || x>1000 -w)
		{
			speedX = -speedX;
		}
		if (y < 0)
		{
			speedY = -speedY;
		}
		x += speedX;
		y += speedY;
	}
	void update(float time)
	{
		ballMove(time);
		sprite.setPosition(x,y);
	}
};

bool menu(RenderWindow & window)
{
	float CursorX = 0;
	float CursorY = 0;
	Image imageCursor;
	imageCursor.loadFromFile("images/Cursor.png");
	Texture textureCursor;
	textureCursor.loadFromImage(imageCursor);
	Sprite spriteCursor;
	spriteCursor.setTexture(textureCursor);
	spriteCursor.setScale(0.15f, 0.15f);
	Image play_image, helpdesk_image, exit_image;
	play_image.loadFromFile("images/playButton.png");
	play_image.createMaskFromColor(Color::White);
	helpdesk_image.loadFromFile("images/helpButton.png");
	helpdesk_image.createMaskFromColor(Color::White);
	exit_image.loadFromFile("images/exitButton.png");
	exit_image.createMaskFromColor(Color::White);
	Texture menu_texture1, menu_texture2, menu_texture3, menu_background, helpdesk_texture;
	menu_texture1.loadFromImage(play_image);
	menu_texture2.loadFromImage(helpdesk_image);
	menu_texture3.loadFromImage(exit_image);
	helpdesk_texture.loadFromFile("images/helpdesk.png");
	menu_background.loadFromFile("images/background.png");
	Sprite menu1(menu_texture1), menu2(menu_texture2), menu3(menu_texture3), menuBack(menu_background), helpdesk(helpdesk_texture);
	bool isMenu = true;
	int menuNum = 0;
	menu1.setPosition(window.getSize().x / 2 -50, window.getSize().y / 2 - 200);
	menu2.setPosition(window.getSize().x / 2 -50, window.getSize().y / 2 - 100);
	menu3.setPosition(window.getSize().x / 2 -50, window.getSize().y / 2);
	menuBack.setPosition(0, 0);
	while (isMenu)
	{
		ShowCursor(0);
		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		CursorX = pos.x;
		CursorY = pos.y;
		spriteCursor.setPosition(pos.x, pos.y);
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		if (IntRect(window.getSize().x / 2, window.getSize().y / 2 - 200, 128, 43).contains(Mouse::getPosition(window))) 
		{
			menu1.setColor(Color::Red);
			menuNum = 1;
		}
		if (IntRect(window.getSize().x / 2, window.getSize().y / 2 - 100, 128, 43).contains(Mouse::getPosition(window)))
		{
			menu2.setColor(Color::Red);
			menuNum = 2;
		}
		if (IntRect(window.getSize().x / 2, window.getSize().y / 2, 128, 43).contains(Mouse::getPosition(window)))
		{
			menu3.setColor(Color::Red);
			menuNum = 3;
		}
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) { isMenu = false; flag = false; }
			if (menuNum == 2) 
			{ 
				window.draw(helpdesk);
				window.display();
				while (!Keyboard::isKeyPressed(Keyboard::BackSpace)); 
			}
			if (menuNum == 3) { isMenu = false; flag = false; isGame = false; exit(0); }
		} 
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			exit(0);
		}
		window.draw(menuBack);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.draw(spriteCursor);
		window.display();
	}
	return isMenu;
}

bool startGame()
{
	ifstream fin("highScore.txt");
	sf::RenderWindow window(sf::VideoMode(1000, 500), "Arcanoid");
	if (flag)
	{
		sf::RenderWindow menuWindow(sf::VideoMode(1000, 500), "Menu");
		menu(menuWindow);
	}
	Font font;
	font.loadFromFile("C:/WINDOWS/Fonts/arial.ttf");
	Text textEnd("", font, 50);
	textEnd.setColor(Color::Red);
	Text textScore("", font, 20);
	textScore.setColor(Color::Red);
	Text textHighScore("", font, 20);
	textHighScore.setColor(Color::Red);
	Text textPause("", font, 50);
	textPause.setColor(Color::Red);
	int countLines = 5;
	int countColumns = 7;
	int countBlocks = 0;
	int countDescr = 0;
	int highScore = 0;
	bool pause = false;
	bool gameIsStart = false;
	bool gameEnd = false;
	bool recordScore = false;
	string line;

	Image ball_image;
	ball_image.loadFromFile("images/ball1.png");
	ball_image.createMaskFromColor(Color::Black);
	Ball ball(ball_image, 2, 507, 420, 33, 27);

	Image block_image;
	block_image.loadFromFile("images/block.png");
	Texture block_texture;
	block_texture.loadFromImage(block_image);
	Sprite block[1000];

	Image roller_image;
	roller_image.loadFromFile("images/roller.png");
	roller_image.createMaskFromColor(Color::Black);
	Player roller(roller_image, 3, 450, 450, 150, 42);

	Clock clock;

	for (int i = 1; i < countLines; i++)
	{
		for (int j = 1; j < countColumns; j++)
		{
			block[countBlocks].setTexture(block_texture);
			block[countBlocks].setPosition(j * 128, i * 43);
			countBlocks++;
		}
	}

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 600;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || !isGame)
				window.close();
		}
		if (gameIsStart && !pause)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				roller.animation(time);
				roller.sprite.move(0.2*time, 0);
				roller.x += 0.2*time;
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				roller.animation(time);
				roller.sprite.move(-0.2*time, 0);
				roller.x -= 0.2*time;
			}
			ball.update(time);
			roller.update(time);
		}
		std::ostringstream playerScore;
		playerScore << roller.score;
		textScore.setString("Score: " + playerScore.str());
		textScore.setPosition(0, 0);
		
		ostringstream playerHighScore;
		if (fin.is_open())
		{
			while (getline(fin, line))
			{
				highScore = stoi(line);
			}
		}
		playerHighScore << line;
		textHighScore.setString("HighScore: " + playerHighScore.str());
		textHighScore.setPosition(450, 0);
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			gameIsStart = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Tilde))
		{
			return true;
		}
		if (Keyboard::isKeyPressed(Keyboard::F1))
		{
			pause = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::F2))
		{
			pause = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			flag = true;
			return true;
		}
		window.clear();
		window.draw(roller.sprite);
		window.draw(ball.sprite);
		if (ball.getRect().intersects(roller.getRect()))
		{
			ball.speedY = -ball.speedY;
		}
		if (ball.y > 500 - ball.h)
		{
			gameIsStart = false;
			gameEnd = true;
		}
		if (gameEnd)
		{
			textEnd.setString("Game Over!");
			textEnd.setPosition(window.getSize().x / 2, window.getSize().y / 2);
			if (roller.score > highScore && roller.score!=0)
			{
				ofstream fout("highScore.txt");
				highScore = roller.score;
				std::ostringstream highScore;
				highScore << roller.score;
				textHighScore.setString("HighScore: " + highScore.str());
				if (fout.is_open() && !recordScore)
				{
					fout << roller.score;
					recordScore = true;
				}
			}
		}
		for (int i = 0; i < countBlocks; i++)
		{
			if (ball.getRect().intersects(block[i].getGlobalBounds()))
			{
				block[i].setPosition(-100, -100); 
				ball.speedY = -ball.speedY;
				roller.score += 50;
				countDescr++;
			}
		}
		if (countBlocks == countDescr)
		{
			gameIsStart = false;
		}
		for (int i = 0; i < countBlocks; i++)
		{
			window.draw(block[i]);
		}
		if (pause)
		{
			textPause.setString("Pause");
			textPause.setPosition(window.getSize().x / 2, window.getSize().y / 2);
			window.draw(textPause);
		}
		window.draw(textEnd);
		window.draw(textScore);
		window.draw(textHighScore);
		window.display();
	}
}

void gameRunning() {
	if (startGame()) { gameRunning(); }
}

int main()
{
	gameRunning();
	return 0;
}