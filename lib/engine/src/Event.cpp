#include "Event.hpp"

Event<int, int> GameEvents::OnPlayerMove;
Event<std::string> GameEvents::OnDialogueTrigger;
Event<WorldEntity*> GameEvents::OnEntityDestroyed;
Event<> GameEvents::SaveGame;

