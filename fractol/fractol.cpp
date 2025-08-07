#include <SFML/Graphics.hpp>
#include <complex>
#include <thread>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_ITER = 500;

double zoom = 1000.0;
double offsetX = -WIDTH / 2.0;
double offsetY = -HEIGHT / 2.0;

sf::Image image;

std::complex<double> pixelToComplex(int x, int y) {
    double real = (x + offsetX) / zoom;
    double imag = (y + offsetY) / zoom;
    return { real, imag };
}

int mandelbrot(std::complex<double> c) {
    std::complex<double> z = 0;
    int iter = 0;
    while (std::abs(z) <= 2.0 && iter < MAX_ITER) {
        z = z * z + c;
        ++iter;
    }
    return iter;
}

sf::Color getColor(int iter) {
    if (iter == MAX_ITER) return sf::Color::Black;
    return sf::Color(10 * iter % 255, 5 * iter % 255, 15 * iter % 255);
}

void drawMandelbrot(int startY, int endY) {
    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            std::complex<double> c = pixelToComplex(x, y);
            int iter = mandelbrot(c);
            image.setPixel(x, y, getColor(iter));
        }
    }
}

void generateImageWithThreads(int threadCount = 4) {
    std::vector<std::thread> threads;
    int rowsPerThread = HEIGHT / threadCount;

    for (int i = 0; i < threadCount; ++i) {
        int startY = i * rowsPerThread;
        int endY = (i == threadCount - 1) ? HEIGHT : startY + rowsPerThread;
        threads.emplace_back(drawMandelbrot, startY, endY);
    }

    for (auto& t : threads) t.join();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fast Mandelbrot");
    image.create(WIDTH, HEIGHT);

    sf::Texture texture;
    sf::Sprite sprite;

    // √Ê· —”„
    generateImageWithThreads();
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Zoom with scroll
            if (event.type == sf::Event::MouseWheelScrolled) {
                double factor = (event.mouseWheelScroll.delta > 0) ? 1.2 : 0.8;

                int mouseX = event.mouseWheelScroll.x;
                int mouseY = event.mouseWheelScroll.y;

                double worldX = (mouseX + offsetX) / zoom;
                double worldY = (mouseY + offsetY) / zoom;

                zoom *= factor;

                offsetX = worldX * zoom - mouseX;
                offsetY = worldY * zoom - mouseY;

                generateImageWithThreads(); // ≈⁄«œ… «·—”„ »”—⁄…
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
