#include "Engine.h"
#include <ctime>

Engine::Engine() {
    m_Window.create(VideoMode::getDesktopMode(), "Particle Engine");
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void Engine::run() {
    std::cout << "Starting Particle unit tests..." << std::endl;
    Particle p(m_Window, 4, { static_cast<int>(m_Window.getSize().x) / 2, static_cast<int>(m_Window.getSize().y) / 2 });
    p.unitTests();
    std::cout << "Unit tests complete.  Starting engine..." << std::endl;

    Clock clock;

    while (m_Window.isOpen()) {
        float dt = clock.restart().asSeconds();

        input();
        update(dt);
        draw();
    }
}

void Engine::input() {
    Event event;
    while (m_Window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            m_Window.close();
        }
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
            m_Window.close();
        }
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            for (int i = 0; i < 5; ++i) {
                int numPoints = 25 + (std::rand() % 26);
                m_particles.emplace_back(m_Window, numPoints, mousePos);
            }
        }
    }
}

void Engine::update(float dtAsSeconds) {
    for (auto it = m_particles.begin(); it != m_particles.end(); ) {
        if (it->getTTL() > 0.0f) {
            it->update(dtAsSeconds);
            ++it;
        }
        else {
            it = m_particles.erase(it);
        }
    }
}

void Engine::draw() {
    m_Window.clear();
    for (const auto& particle : m_particles) {
        m_Window.draw(particle);
    }
    m_Window.display();
}
