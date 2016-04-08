#pragma once

#include <Core/MW/Subscriber.hpp>
#include <Core/MW/CoreNode.hpp>

#include <sensors/SubscriberConfiguration.hpp>

#include <Configuration.hpp>
#include <Board.hpp>

/*
 * struct X {
   static bool
   callback(
      const common_msgs::XYZ_i16& msg,
      Core::MW::Node* node
   )
   {
    ccc++;

    return true;
   }
   };

   using XYZ_i16Subscriber   = sensors::DummySubscriber<common_msgs::XYZ_i16, X>;

 */


namespace common {
	template <class _MESSAGETYPE, class _CALLBACK>
	class DummySubscriber:
		public Core::MW::CoreNode
	{
public:
		using MessageType = _MESSAGETYPE;
		using Callback    = _CALLBACK;

public:
		DummySubscriber(
				const char* name
		) :
			CoreNode::CoreNode(name)
		{
			_workingAreaSize = 256;
		}

		virtual
		~DummySubscriber()
		{
			teardown();
		}

public:
		SubscriberConfiguration configuration;

private:
		bool
		onPrepareMW()
		{
			_subscriber.set_callback(Callback::callback);
			this->subscribe(_subscriber, configuration.topic);

			return true;
		}

		bool
		onLoop()
		{
			if (!this->spin(Configuration::SUBSCRIBER_SPIN_TIME)) {
				Board::led.toggle();
			}

			return true;
		}

private:
		Core::MW::Subscriber<MessageType, Configuration::SUBSCRIBER_QUEUE_LENGTH> _subscriber;
	};
}
