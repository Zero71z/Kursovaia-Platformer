#include <SFML/Graphics.hpp>
#include <conio.h>
#include <SFML/Audio.hpp>

using namespace sf;

float offsetX = 0, offsetY = 0;

const int H = 10;
const int W = 50;

const int ts = 35;

String TileMap[H] = {
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"A                                                A",
"A        o o                                     A",
"A                                      A         A",
"A       AAAAAA                         AAAA      A",
"A                                      A         A",
"AAAAA                           AAA    A      AAAA",
"A                     o o           A            A",
"A              AAAA                    AAAA      A",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
};

class Player {
public:
	float dx, dy;
	FloatRect rect;
	bool onGround, rig;
	Sprite sprite;
	float curFrame;
	bool life;
	int l;


	Player(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(1 * ts, 8 * ts, 50, 48);
		sprite.setTextureRect(IntRect(0, 0, 40, 50));


		dx = dy = 0;
		curFrame = 1;
		rig = true;
	
	}

	void update(float time) {
		rect.left += dx * time;
		Collision(0);

		if (!onGround)
			dy = dy + 0.0005 * time;

		rect.top += dy * time;

		onGround = false;
		Collision(1);

		curFrame += 0.005 * time;
		if (curFrame > 4)
			curFrame -= 3;

		if (dx > 0) {
			if (dy == 0)
				sprite.setTextureRect(IntRect(35 * int(curFrame), 0, 35, 45));
			else
				sprite.setTextureRect(IntRect(138, 0, 35, 45));

			rig = true;
		}
		if (dx < 0) {
			if (dy == 0)
				sprite.setTextureRect(IntRect(35 * int(curFrame) + 35, 0, -35, 45));
			else
				sprite.setTextureRect(IntRect(174, 0, -35, 45));

			rig = false;
		}
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
	}

	void Collision(float dir) {
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++) {
				if (TileMap[i][j] == 'A') {
					if (dx > 0 && dir == 0)
						rect.left = j * ts - rect.width;
					if (dx < 0 && dir == 0)
						rect.left = j * ts + ts;
					if (dy > 0 && dir == 1) {
						rect.top = i * ts - rect.height;
						dy = 0;
						onGround = true;
					}
					if (dy < 0 && dir == 1) {
						rect.top = i * ts + ts;
						dy = 0;
					}
				}

				if (TileMap[i][j] == 'o')
					TileMap[i][j] = ' ';
			}
	}
};
class Enemy {
public:
	float dx, dy;
	FloatRect rect;
	Sprite sprite;
	float curFrame;
	bool life;

	void set(Texture& image, int x, int y) {
		sprite.setTexture(image);
		sprite.setScale(0.35, 0.35);
		rect = FloatRect(x, y, ts, ts);

		dx = 0.1;
		curFrame = 0;
		life = true;
	}

	void update(float time) {
		rect.left += dx * time;

		Collision();

		if (life) {
			curFrame += time * 0.005;
			if (curFrame > 3)
				curFrame -= 2;

			if (dx > 0)
				sprite.setTextureRect(IntRect(165 * int(curFrame) + 165, 0, -165, 204));
			else if (dx < 0)
				sprite.setTextureRect(IntRect(165 * int(curFrame), 0, 165, 204));
		}
		else
			sprite.setTextureRect(IntRect(0, 0, 0, 0));

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}

	void Collision() {
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++)
				if (TileMap[i][j] == 'A')
				{
					if (dx > 0)
					{
						rect.left = j * ts - rect.width;
						dx *= -1;
					}
					else if (dx < 0)
					{
						rect.left = j * ts + ts;
						dx *= -1;
					}
				}
	}
};

class CollectableItem : public Sprite
{
public:
	CollectableItem(const sf::Texture& texture, const sf::Vector2f& position)
	{
		setTexture(texture);
		setPosition(position);
	}

	bool isCollected() const { return m_isCollected; }
	void collect() { m_isCollected = true; }

private:
	bool m_isCollected = false;
};


int main() {
	RenderWindow window(VideoMode(1000, 500), "The game");

	float Frame = 0;

	Texture Picture;
	Picture.loadFromFile("C:\\dendy_data\\chip_anime1.png");
	Player p(Picture);


	Texture t2;
	t2.loadFromFile("C:\\dendy_data\\MergedImages.png");
	Sprite plat(t2);

	Texture t3;
	t3.loadFromFile("C:\\dendy_data\\enemy.png");
	Enemy enemy;
	enemy.set(t3, 19 * ts, 8 * ts - 20);

	Music music;
	music.openFromFile("C:\\dendy_data\\1-title.mp3");
	music.play();
	
	SoundBuffer buffer;
	buffer.loadFromFile("C:\\dendy_data\\Jump.ogg");
	Sound sound(buffer);
	
	Clock clock;



	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (!_kbhit())
			if (p.dy == 0) {
				if (p.rig)
					p.sprite.setTextureRect(IntRect(0, 0, 35, 48));
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(35, 0, -35, 48));
			}

		if (Keyboard::isKeyPressed(Keyboard::Left))
			p.dx = -0.2;
		if (Keyboard::isKeyPressed(Keyboard::Right))
			p.dx = 0.2;

		if (Keyboard::isKeyPressed(Keyboard::Up))
			if (p.onGround) {
				p.dy = -0.4;
				p.onGround = false;
				sound.play();
			}

		if (p.rect.intersects(enemy.rect))
		{
			if (enemy.life) {
				if (p.dy > 0) { enemy.dx = 0; p.dy = -0.2; enemy.life = false; }
				else p.sprite.setColor(Color::Red);
			}
		}

		if (p.rect.left > 500 && p.rect.left < 2000)
			offsetX = p.rect.left - 500;
		if (p.rect.top > 200)
			offsetY = p.rect.top - 250;

		p.update(time);
		enemy.update(time);
		window.clear(Color::White);

		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++) {
				if (TileMap[i][j] == 'A')
					plat.setTextureRect(IntRect(105, 0, 35, 48));
				if (TileMap[i][j] == 'o')
					plat.setTextureRect(IntRect(147, 0, 35, 48));
				if (TileMap[i][j] == ' ')
					plat.setTextureRect(IntRect(35, 0, 35, 48));

				plat.setPosition(j * ts - offsetX, i * ts - offsetY);
				window.draw(plat);


			}
		window.draw(enemy.sprite);
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}