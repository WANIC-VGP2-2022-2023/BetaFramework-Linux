//------------------------------------------------------------------------------
//
// File Name:	EventManager.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EventManager.h"

//------------------------------------------------------------------------------

namespace Beta
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for Event
	// Params:
	//   type = The type of event that for which we are listening.
	Event::Event(const std::string& type)
		: type(type)
	{
	}

	Event::~Event()
	{
	}

	// Constructor for ListenerBase
	// Params:
	//   source = The source of the event, typically a game object component.
	//   type   = The type of event that for which we are listening.
	ListenerBase::ListenerBase(const BetaObject* source, const std::string& type)
		: source(source), type(type)
	{
	}

	// EventManager Constructor
	EventManager::EventManager()
		: BetaObject("Module:EventManager")
	{
	}

	// EventManager Destructor
	EventManager::~EventManager()
	{
		Shutdown();
	}

	// Decreases timers on all delayed events, sends events that are ready.
	// Params:
	//   dt = The change in time since the previous frame.
	void EventManager::Update(float dt)
	{
		for (auto it = delayedEvents.Begin(); it != delayedEvents.End(); )
		{
			// Decrease delay
			(*it)->delay -= dt;

			// Send event now if ready
			if ((*it)->delay <= 0.0f)
			{
				SendEvent((*it)->event, (*it)->source);
				delete (*it);
				it = delayedEvents.Erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	// Destroys all listeners and delayed events.
	void EventManager::Shutdown()
	{
		// Destroy all events
		size_t numDelayed = delayedEvents.Size();
		for (unsigned i = 0; i < numDelayed; ++i)
		{
			delete delayedEvents[i];
			delayedEvents[i] = nullptr;
		}
		delayedEvents.Clear();

		// Destroy all listeners
		size_t numListeners = listeners.Size();
		for (unsigned i = 0; i < numListeners; ++i)
		{
			delete listeners[i];
			listeners[i] = nullptr;
		}
		listeners.Clear();
	}

	// Sends an event to all registered listeners.
	// Params:
	//	 event  = A pointer to the event being sent.
	//   source = The source of the event, typically a game object component.
	//   delay  = How long to wait before sending the event.
	void EventManager::SendEvent(const Event* event, const BetaObject* source, float delay)
	{
		// Add to delayed events if delay > 0.0f
		if (delay > 0.0f)
		{
			delayedEvents.PushBack(new DelayedEvent(event, source, delay));
		}
		// Else, send immediately
		else
		{
			size_t numListeners = listeners.Size();
			for (size_t i = 0; i < numListeners; ++i)
			{
				ListenerBase* listener = listeners[i];

				// If source and type match
				if (listener->source == source && listener->type == event->type)
				{
					// Call handler on destination object with event as argument
					listener->CallHandler(event);
				}
			}

			// Dispose of event
			delete event;
		}
	}

	// Constructor for DelayedEvent.
	// Params:
	//	 event  = A pointer to the event being sent.
	//   source = The source of the event, typically a game object component.
	//   delay  = How long to wait before sending the event.
	EventManager::DelayedEvent::DelayedEvent(const Event* event, const BetaObject* source, float delay)
		: event(event), source(source), delay(delay)
	{
	}
}
