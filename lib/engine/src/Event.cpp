#include "Event.hpp"

Event<int, int> GameEvents::OnPlayerMove;
Event<std::string> GameEvents::OnDialogueTrigger;
Event<WorldEntity*> GameEvents::OnEntityDestroyed;
Event<> GameEvents::Ev1;
Event<> GameEvents::Ev2;
Event<> GameEvents::Ev3;
Event<> GameEvents::SaveGame;
Event<> GameEvents::YesChoice;
Event<> GameEvents::NoChoice;

