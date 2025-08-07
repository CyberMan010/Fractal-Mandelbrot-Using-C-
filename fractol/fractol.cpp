#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_ITER = 200;

double zoom = 200.0;            // ﬂ· „« ﬂ»—° ﬂ· „« ⁄„· “Ê„ √ﬂ —
double offsetX = -WIDTH / 2.0;  // ≈“«Õ… ··Ì”«—
double offsetY = -HEIGHT / 2.0; // ≈“«Õ… ··√⁄·Ï

//  ÕÊÌ· »ﬂ”· ≈·Ï —ﬁ„ „—ﬂ»
std::complex<double> pixelToComplex(int x, int y) {
    double real = (x + offsetX) / zoom;
    double imag = (y + offsetY) / zoom;
    return std::complex<double>(real, imag);
}

// Õ”«» ⁄œœ «· ﬂ—«—«  ··Â—Ê»
int mandelbrot(std::complex<double> c) {
    std::complex<double> z = 0;
    int iter = 0;
    while (abs(z) <= 2.0 && iter < MAX_ITER) {
        z = z * z + c;
        iter++;
    }
    return iter;
}

//  ÕÊÌ· ⁄œœ «· ﬂ—«—«  ≈·Ï ·Ê‰
sf::Color getColor(int iter) {
    if (iter == MAX_ITER) return sf::Color::Black;
    return sf::Color(5 * iter, 10 * iter, 15 * iter); // √·Ê«‰ „ œ—Ã…
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Explorer");
    sf::Image image;
    image.create(WIDTH, HEIGHT);
    sf::Texture texture;
    sf::Sprite sprite;

    // √Ê· —”„
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            std::complex<double> c = pixelToComplex(x, y);
            int iter = mandelbrot(c);
            image.setPixel(x, y, getColor(iter));
        }
    }

    texture.loadFromImage(image);
    sprite.setTexture(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // ≈€·«ﬁ
            if (event.type == sf::Event::Closed)
                window.close();

            // “ÊÊÊÊ„ »«·„«Ê”
            if (event.type == sf::Event::MouseWheelScrolled) {
                double factor = event.mouseWheelScroll.delta > 0 ? 1.1 : 0.9;

                // ‰Õ”» „—ﬂ“ «·“Ê„ ÕÊ· „ƒ‘— «·„«Ê”
                int mouseX = event.mouseWheelScroll.x;
                int mouseY = event.mouseWheelScroll.y;

                double worldX = (mouseX + offsetX) / zoom;
                double worldY = (mouseY + offsetY) / zoom;

                zoom *= factor;

                offsetX = worldX * zoom - mouseX;
                offsetY = worldY * zoom - mouseY;

                //  ÕœÌÀ «·’Ê—…
                for (int x = 0; x < WIDTH; x++) {
                    for (int y = 0; y < HEIGHT; y++) {
                        std::complex<double> c = pixelToComplex(x, y);
                        int iter = mandelbrot(c);
                        image.setPixel(x, y, getColor(iter));
                    }
                }
                texture.loadFromImage(image);
                sprite.setTexture(texture);
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
