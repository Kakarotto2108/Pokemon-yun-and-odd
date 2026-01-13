#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp> // Ajouté pour les Vector2u/Vector2f utilisés par SFML 3
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "lib/TileMap.hpp"
#include "lib/Pnj.hpp"
#include "lib/Player.hpp"
#include "lib/MessageBox.hpp" // Assurez-vous que ce fichier existe
#include "lib/Inventory.hpp"
#include "lib/Item.hpp"
#include "lib/Zone.hpp"

int main() {
    // CORRECTION SFML 3: sf::VideoMode utilise désormais une liste d'initialisation {width, height}.
    sf::RenderWindow window(sf::VideoMode({640, 480}), "Pokemon Yon/Oll");
    window.setFramerateLimit(120);

    // Création du joueur avec textures
    Player player(
        "assets/sprite/pnj/player_face.png",
        "assets/sprite/pnj/player_l.png",
        "assets/sprite/pnj/player_r.png",
        "assets/sprite/pnj/player_up.png",
        "Kakarot"
    );
    // Liste de PNJ
    std::vector<std::unique_ptr<Pnj>> pnjs;

    Item pokeball("Poké ball", ItemPocket::Balls, "Un objet semblable à une capsule, qui capture les Pokémon sauvages. Il suffit pour cela de le lancer comme une balle.", true);
    Item superCanne("Super canne", ItemPocket::KeyItems, "Une canne à pêche rudimentaire. Elle permet de pêcher des Pokémon aquatiques.", false);
    Item chaussures("Ch. de sport", ItemPocket::KeyItems, "Des chaussures confortables, idéales pour se déplacer rapidement.", false);

    pnjs.push_back(std::make_unique<Pnj>(
        "assets/sprite/pokemon_outbattle/Archeodong_cote.png",
        "assets/sprite/pokemon_outbattle/Archeodong_face.png",
        sf::Vector2i(7,3), 1, "Arch ! Archéo !"
    ));


    pnjs.push_back(std::make_unique<Pnj>(
        "assets/sprite/pnj/48049.png",
        "assets/sprite/pnj/48050.png",
        sf::Vector2i(4,2), 0,
        "Bonjour ! Je m'appelle Camille, je suis super gentille, et surtout j'aime les choux de Bruxelles",
        chaussures
    ));

    const float delayTime = 0.15f;
    sf::Clock moveClock;

    
    Zone zone1(1, std::move(pnjs));
    Zone zone2(2, {});

    std::vector<Zone*> zones = {&zone1, &zone2};
    // CORRECTION SFML 3: sf::FloatRect utilise Vector2f position et Vector2f size.
    // window.getSize() retourne un Vector2u, nous le castons en Vector2f.
    sf::View cameraView(sf::FloatRect({0.f, 0.f}, (sf::Vector2f)window.getSize()));
    sf::View uiView(sf::FloatRect({0.f, 0.f}, (sf::Vector2f)window.getSize()));
    
    // CORRECTION SFML 3: setSize prend un seul argument Vector2f.
    cameraView.setSize((sf::Vector2f)window.getSize());

    MessageBox messageBox({600, 90}, {-5.0, 8.0});

    while (window.isOpen()) {
        Zone* currentZone = nullptr;

        for (auto& zone : zones) {
            if (zone->getId() == player.m_currentZone) {
                currentZone = zone;
                break; // on a trouvé la zone, inutile de continuer
            }
        }
        
        // CORRECTION SFML 3: pollEvent retourne un std::optional<sf::Event> et ne prend pas d'argument.
        while (const auto event = window.pollEvent()) {
            
            // CORRECTION SFML 3: La vérification du type d'événement se fait avec event->is<type>()
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        if (moveClock.getElapsedTime().asSeconds() >= delayTime) {
            // NOTE: Si handleInput utilise l'ancien API d'événement, il faudra le corriger aussi.

            player.handleInput(window, *currentZone, delayTime, messageBox);
            moveClock.restart();
        }

        player.update();

        // Caméra suit joueur
        cameraView.setCenter(player.getDrawPosition());
        window.setView(cameraView);

        // Affichage
        window.clear();

        // --- Dessin de la map et du monde ---
        window.setView(cameraView);
        currentZone->drawAll(window, player);

        // --- Dessin UI ---
        window.setView(uiView);  // <- IMPORTANT
        messageBox.draw(window, player.m_name);

        window.display();
    }

    return 0;
};
