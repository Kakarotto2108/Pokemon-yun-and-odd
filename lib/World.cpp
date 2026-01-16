#include "World.hpp"

Monde::Monde() : m_currentZoneId(0) {}

void Monde::addZone(std::unique_ptr<Zone> zone) {
    m_zones[zone->getId()] = std::move(zone);
}

Zone& Monde::getZoneActuelle() {
    return *m_zones.at(m_currentZoneId);
}

void Monde::changerZone(int id, const sf::Vector2i& nouvellePos) {
    m_currentZoneId = id;
    // repositionnement du joueur fait par Jeu
}

void Monde::draw(sf::RenderWindow& window, const EntiteMonde& focus)
{
    getZoneActuelle().drawAll(window, focus);
}

void Monde::init()
{
    std::vector<std::unique_ptr<Pnj>> pnjs;
    std::vector<std::unique_ptr<Obj>> objects;
    std::vector<std::unique_ptr<Pnj>> emptyPnjs;
    std::vector<std::unique_ptr<Obj>> emptyObjs;

    Item pokeball("Poké ball", ItemPocket::Balls, "Un objet semblable à une capsule, qui capture les Pokémon sauvages. Il suffit pour cela de le lancer comme une balle.", true);
    Item superCanne("Super canne", ItemPocket::KeyItems, "Une canne à pêche rudimentaire. Elle permet de pêcher des Pokémon aquatiques.", false);
    Item chaussures("Ch. de sport", ItemPocket::KeyItems, "Des chaussures confortables, idéales pour se déplacer rapidement.", false);

    pnjs.push_back(std::make_unique<Pnj>(
    "assets/sprite/pnj/48049.png",
    "assets/sprite/pnj/48050.png",
    sf::Vector2i(4,2), 0,
    "Bonjour ! Je m'appelle Camille, je suis super gentille, et surtout j'aime les choux de Bruxelles",
    chaussures
    ));
    pnjs.push_back(std::make_unique<Pnj>(
    "assets/sprite/pokemon_outbattle/Archeodong_cote.png",
    "assets/sprite/pokemon_outbattle/Archeodong_face.png",
    sf::Vector2i(7,3), 1, "Arch ! Archéo !"
    ));
    objects.push_back(std::make_unique<Obj>(
    "assets/sprite/obj/IMG_1338.png",
    sf::Vector2i(5,0),
    "Cette télé est un cadeau de maman.",
    pokeball
    ));

    m_zones.push_back(std::make_unique<Zone>(1, std::move(pnjs), std::move(objects)));
    m_zones.push_back(std::make_unique<Zone>(
    2,
    std::move(emptyPnjs),
    std::move(emptyObjs)
    ));

    m_currentZoneId = 1;
}

