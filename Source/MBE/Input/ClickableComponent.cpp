#include <MBE/Input/ClickableComponent.h>

using namespace mbe;

ClickableComponent::ClickableComponent(Entity & parentEntity) : Component(parentEntity)
{
	connectedClickableEntityIdList.insert(parentEntity.GetHandleID());
}

void ClickableComponent::AddConnectedClickableEntity(Entity::HandleID entityId)
{
	connectedClickableEntityIdList.insert(entityId);
}

void ClickableComponent::RemoveConnectedClickableEntity(Entity::HandleID entityId)
{
	connectedClickableEntityIdList.erase(entityId);
}

void ClickableComponent::RemoveExpiredConnectedClickableEntities()
{
	// Remove deleted entities from the 
	for (auto it = connectedClickableEntityIdList.cbegin(); it != connectedClickableEntityIdList.cend(); )
	{
		const auto * entityPtr = Entity::GetObjectFromID(*it);
		if (entityPtr == nullptr || entityPtr->IsActive() == false)
			it = connectedClickableEntityIdList.erase(it);
		else
			++it;
	}
}
