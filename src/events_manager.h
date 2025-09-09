#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "utils/singleton.h"
#include "events.h"

#define EV_BIND(obj, func) [obj](auto* data){ (obj)->func(data); }

template<typename EventData>
using EventCallback = std::function<void(EventData*)>;

class EventsManager : public Singleton<EventsManager>
{
public:

	template<typename EventData = void>
	void RegisterEventCallback(EventType event, EventCallback<EventData> callback)
	{
		m_EventsCallbacks[event].push_back(
			[callback](void* data) { callback(static_cast<EventData*>(data)); }
		);
	}

	template<typename EventData>
	void TriggerEvent(EventType event, EventData data)
	{
		for (const EventCallback<void>& cb : m_EventsCallbacks[event])
		{
			cb(&data);
		}
	}

private:
	std::unordered_map<EventType, std::vector<EventCallback<void>>> m_EventsCallbacks;
};