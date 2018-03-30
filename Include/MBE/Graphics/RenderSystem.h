#pragma once

/// @file
/// @brief The Class mbe::RenderSystem

#include <map>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <cassert>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <MBE/Graphics/RenderComponent.h>
#include <MBE/Graphics/RenderManager.h>
#include <MBE/Core/EventManager.h>
#include <MBE/Core/EntityCreatedEvent.h>
#include <MBE/Core/EntityRemovedEvent.h>

using mbe::event::EntityCreatedEvent;
using mbe::event::EntityRemovedEvent;

//namespace mbe
//{
//	/// @brief Takes care of drawing all entities with a mbe::RenderComponent in the correct order
//	/// @details A mbe::Entity can be registered by raising the mbe::event::EntityCreatedEvent.
//	/// Similarly, it can be unregistered by raising the mbe::event::RenderNodeRemovedEvent.
//	/// @note Deleted entities will be removed automatically. Therefore, manually raising the
//	/// mbe::event::EntityRemovedEvent is only necessary when the mbe::Entity should not be drawn but stay alive
//	/// e.g. in order to add it again later.
//	class RenderSystem
//	{
//	public:
//		typedef std::function<void(std::vector<Entity::HandleID> &)> ZOrderAssignmentFunction;
//
//	public:
//		/// @brief Constructor
//		/// @param window A reference to the sf::RenderWindow that will be used to draw in
//		/// @param eventManager A reference to the mbe::EventManager that will be used to listen out
//		/// for a mbe::event::EntityCreatedEvent and mbe::event::RenderNodeRemovedEvent.
//		RenderSystem(sf::RenderWindow & window, EventManager & eventManager);
//
//		/// @brief Default destructor
//		~RenderSystem();
//
//		/// @brief Draws all the registered entites
//		/// @details Entities are only drawn when visible on screen.
//		void Render();
//		
//		void SetZOrderAssignmentFunction(RenderComponent::RenderLayer layer, ZOrderAssignmentFunction function);
//
//		void SetView(RenderComponent::RenderLayer renderLayer, const sf::View & view);
//		// This function should be prefered over SetView() since the existsing view can be modified
//		sf::View & GetView(RenderComponent::RenderLayer renderLayer);
//		// Const overload
//		const sf::View & GetView(RenderComponent::RenderLayer renderLayer) const;
//
//		inline const sf::RenderWindow & GetRenderWindow() const { return window; }
//
//	private:
//		void AddRenderEntity(Entity::HandleID renderEntityId);
//		void RemoveRenderEntity(Entity::HandleID renderEntityId);
//
//		// Removes expires render entities
//		void Refresh();
//
//		// Culling
//		bool IsVisible(const Entity & entity, const sf::View & view);
//
//		// Performs an insertion sort
//		// Assumes that render nodes exist for all render node ids
//		static void SortByZOrder(std::vector<Entity::HandleID> & renderNodeIdList);
//
//	private:
//		sf::RenderWindow & window;
//
//		std::array<detail::RenderLayer, RenderComponent::RenderLayer::LayerCount> renderLayers; // The index is important // will be default initialised
//
//		std::array<ZOrderAssignmentFunction, RenderComponent::RenderLayer::LayerCount> zOrderAssignmentFunctions; // default initialised to an empty function
//
//		// Refernce to the event manager
//		EventManager & eventManager;
//
//		// SubscriptionId to unsubscribe the subscribed events
//		EventManager::SubscriptionID renderEntityCreatedSubscription;
//		EventManager::SubscriptionID renderEntityRemovedSubscription;
//	};
//
//} // namespace mbe

namespace mbe
{

	// Adds the ability to add zOrder sorting functions
	// Connects the entity component system and event manager with the render manager

	class RenderSystem
	{
	public:
		typedef std::function<void(std::vector<Entity::HandleID> &)> ZOrderAssignmentFunction;


	public:
		/// @brief Constructor
		/// @param window A reference to the sf::RenderWindow that will be used to draw in
		/// @param eventManager A reference to the mbe::EventManager that will be used to listen out
		/// for a mbe::event::EntityCreatedEvent and mbe::event::RenderNodeRemovedEvent.
		RenderSystem(sf::RenderWindow & window, EventManager & eventManager, RenderManager & renderManager);
		
		/// @brief Destructor
		~RenderSystem();
		
		/// @brief Draws all the registered entites
		/// @details Entities are only drawn when visible on screen.
		void Render();
			
		void SetZOrderAssignmentFunction(RenderObject::RenderLayer renderLayer, ZOrderAssignmentFunction function);

		inline const sf::RenderWindow & GetRenderWindow() const { return window; }

		inline const RenderManager & GetRenderManager() const { return renderManager; }
		
	private:
		void AddRenderEntity(Entity::HandleID entityId);
		//void RemoveRenderEntity(Entity::HandleID entityId);
		
		// Removes expires render entities
		void Refresh();
		
	private:
		sf::RenderWindow & window;
		
		// Default initialised to an empty function when calling [] operator for non-existing function
		std::unordered_map<RenderObject::RenderLayer, ZOrderAssignmentFunction> zOrderAssignmentFunctionDictionary;

		// Default initialised with an empty std::vector when calling [] operator for non-existing renderEntityList
		std::unordered_map<RenderObject::RenderLayer, std::vector<Entity::HandleID>> renderEntityDictionary;

		// Dictionary of the render data instances it manages
		//std::unordered_map<Entity::HandleID, RenderObject> renderDataDictionary;
		
		EventManager & eventManager;
		RenderManager & renderManager;
		
		// SubscriptionId to unsubscribe the subscribed events
		EventManager::SubscriptionID entityCreatedSubscription;
		EventManager::SubscriptionID entityRemovedSubscription;
	};

} // namespace mbe