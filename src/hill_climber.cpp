#include "hill_climber.h"

void HillClimber::draw(
	sf::RenderTarget& target,
	sf::RenderStates states
) const {
	texture.loadFromImage(result);
	sprite.setTexture(texture);

	states.transform *= getTransform();
	target.draw(sprite, states);
}

double HillClimber::pixel_similatity(
	const sf::Color &a,
	const sf::Color &b
) const {

	double pixel_sim = 0;

	pixel_sim += abs(a.r - b.r);
	pixel_sim += abs(a.g - b.g);
	pixel_sim += abs(a.b - b.b);
	pixel_sim += abs(a.a - b.a);

	return 1 - (pixel_sim / (255 * 4.0f));
}

double HillClimber::apply(
	const sf::Rect<unsigned> &rect,
	const sf::Color color
) const {

	double similarity = 0;

	for (unsigned y = 0; y != height; ++y) {
		for (unsigned x = 0; x != width; ++x) {

			sf::Color original_color(original.getPixel(x, y));
			sf::Color new_color;

			if (
				x >= rect.left
				&& x < rect.left + rect.width
				&& y >= rect.top
				&& y < rect.top + rect.height
			) {
				new_color = color;
			} else {
				new_color = result.getPixel(x, y);
			}

			similarity += pixel_similatity(original_color, new_color);
		}
	}

	return similarity / (float) width * (float) height;
}

sf::Rect<unsigned> HillClimber::create_random_rect() const {

	sf::Vector2u top_left(rand() % width, rand() % height);
	sf::Vector2u bottom_right(rand() % width, rand() % height);

	if (top_left.x > bottom_right.x) {
		std::swap(top_left.x, bottom_right.x);
	}
	if (top_left.y > bottom_right.y) {
		std::swap(top_left.y, bottom_right.y);
	}

	return {
		top_left.x,
		top_left.y,
		bottom_right.x - top_left.x,
		bottom_right.y - top_left.y
	};
}

HillClimber::HillClimber(const std::string &path) {

	original.loadFromFile(path);

	width  = original.getSize().x;
	height = original.getSize().y;

	result.create(width, height, sf::Color::Black);
}

void HillClimber::step() {

	auto rect = create_random_rect();

	sf::Color color(
		rand() % 255,
		rand() % 255,
		rand() % 255
	);

	double cur_fitness = apply(rect, color);

	if (cur_fitness > fitness) {

		fitness = cur_fitness;

		for (unsigned y = rect.top; y != rect.top + rect.height; ++y) {
			for (unsigned x = rect.left; x != rect.left + rect.width; ++x) {
				result.setPixel(x, y, color);
			}
		}
	}
}

unsigned HillClimber::get_width() const {
	return width;
}

unsigned HillClimber::get_height() const {
	return height;
}
