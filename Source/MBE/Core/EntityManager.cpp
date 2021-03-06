#include <MBE/Core/EntityManager.h>

using namespace mbe;

void EntityManager::Update(sf::Time frameTime)
{
	this->Refresh();

	for (auto & entity : entityList)
	{
		entity->Update(frameTime);
	}
}

void EntityManager::Refresh()
{
	// Loops through group
	for (auto & pair : entityGroups)
	{
		auto & groupedEntityList = pair.second;
		groupedEntityList.erase(
			std::remove_if(groupedEntityList.begin(), groupedEntityList.end(),
				[&](Entity::HandleID entityId)
		{
			// Get the entity
			// The entity will still exist since the only way to delete it would be to set it
			// inactive (make sure that inactive entities are deleted after this loop)
			Entity * entity = Entity::GetObjectFromID(entityId);

			// Remove entities that are:
			// Either marked to be deleted (IsActive() == false)
			// or has been removed from the group (IsInGroup(groupId) == false)
			return !entity->IsActive() || !entity->IsInGroup(pair.first); 
		}
		), groupedEntityList.end());
	}

	// Loops through every entity and removes it if its inactive
	entityList.erase(std::remove_if(std::begin(entityList), std::end(entityList), [](const std::unique_ptr<Entity> &entity)
	{
		return !entity->IsActive();
	}),
		std::end(entityList));
}

Entity & EntityManager::CreateEntity()
{
	// The raw new is needed for the entity manager to access the entities protected constructor
	Entity * rawEntity = new Entity(*this);
	// Make a new unique pointer
	std::unique_ptr<Entity> entity(rawEntity);

	// Add the entity to the default group (before moving it)
	//entity->AddToGroup("");

	// Move its content to the container
	entityList.emplace_back(std::move(entity));
	// The returned reference just for user convenience (note that the unique_ptr entity has already been moved thus its empty)
	return *rawEntity;
	// Implicetly delete the obsolete/empty unique pointer
}

void EntityManager::AddEntityToGroup(Entity::HandleID entityId, Entity::Group groupId)
{
	NormaliseIDString(groupId);

	entityGroups[groupId].push_back(entityId);
}

const std::vector<Entity::HandleID>& EntityManager::GetGroup(Entity::Group groupId) const
{
	NormaliseIDString(groupId);

	// This creates a new empty std::vector if a groupId is passed that does not exist
	// This may lead to unnecessary memory usage
	return entityGroups[groupId];
}