#include "Particle.h"
#include <cstdlib>

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_A(2, numPoints)
{
    m_ttl = TTL;
    m_numPoints = numPoints;

    m_radiansPerSec = ((float)std::rand() / RAND_MAX) * M_PI;

    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(target.getSize().x, -1.0f * target.getSize().y);

    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    m_vx = (100 + std::rand() % 401) * (std::rand() % 2 ? 1 : -1);
    m_vy = (100 + std::rand() % 401) * (std::rand() % 2 ? 1 : -1);

    m_color1 = sf::Color::White;
    m_color2 = sf::Color(std::rand() % 256, std::rand() % 256, std::rand() % 256);

    float theta = ((float)std::rand() / RAND_MAX) * (M_PI / 2.0f);
    float dTheta = 2.0f * M_PI / (numPoints - 1);

    for (int j = 0; j < numPoints; ++j) {
        float r = 20 + (std::rand() % 61);
        float dx = r * cos(theta);
        float dy = r * sin(theta);
        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;
        theta += dTheta;
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const {
    sf::VertexArray lines(sf::TriangleFan, m_numPoints + 1);

    sf::Vector2i centerInt = target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);
    sf::Vector2f center(centerInt.x, centerInt.y);
    lines[0].position = center;
    lines[0].color = m_color1;

    for (int j = 1; j <= m_numPoints; ++j) {
    sf::Vector2f pt((float)m_A(0, (j - 1) % m_numPoints), (float)m_A(1, (j - 1) % m_numPoints));
    sf::Vector2i pixelPos = target.mapCoordsToPixel(pt, m_cartesianPlane);
    lines[j].position = sf::Vector2f(pixelPos.x, pixelPos.y);
    lines[j].color = m_color2;
}

    target.draw(lines, states);
}

void Particle::update(float dt) {
    m_ttl -= dt;

    rotate(dt * m_radiansPerSec);
    scale(SCALE);

    m_vy -= G * dt;

    float dx = m_vx * dt;
    float dy = m_vy * dt;

    translate(dx, dy);
}


void Particle::rotate(double theta) {
    sf::Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    Matrices::RotationMatrix R(theta);
    m_A = R * m_A;

    translate(temp.x, temp.y);
}

void Particle::scale(double c) {
    sf::Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    Matrices::ScalingMatrix S(c);
    m_A = S * m_A;

    translate(temp.x, temp.y);
}

void Particle::translate(double xShift, double yShift) {
    Matrices::TranslationMatrix T(xShift, yShift, m_A.getCols());
    m_A = T + m_A;
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}
