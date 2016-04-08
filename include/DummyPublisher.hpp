#include <Core/MW/Publisher.hpp>
#include <Core/MW/CoreNode.hpp>

#include <sensors/PublisherConfiguration.hpp>

#include <Configuration.hpp>

/*
 * struct Counter {
   static bool
   callback(
      common_msgs::XYZ_i16* msg
   )
   {
    static uint32_t x = 0;
    msg->x = x;
    x++;

    return true;
   }
   };

   using XYZ_i16DummyPublisher = sensors::DummyPublisher<common_msgs::XYZ_i16, Counter>;

 */

namespace common {
	template <class _MESSAGETYPE, class _CALLBACK >
	class DummyPublisher:
		public Core::MW::CoreNode
	{
public:
		using MessageType = _MESSAGETYPE;
		using Callback    = _CALLBACK;

public:
		DummyPublisher(
				const char* name
		) :
			CoreNode::CoreNode(name)
		{
			_workingAreaSize = 256;
		}

		virtual
		~DummyPublisher()
		{
			teardown();
		}

public:
		PublisherConfiguration configuration;

private:
		Core::MW::Publisher<MessageType> _publisher;

private:
		bool
		onPrepareMW()
		{
			this->advertise(_publisher, configuration.topic);

			return true;
		}

		bool
		onLoop()
		{
			MessageType* msgp;

			if (_publisher.alloc(msgp)) {
				if (!Callback::callback(msgp)) {
					return false;
				}

				if (!_publisher.publish(*msgp)) {
					return false;
				}
			} else {
				Core::MW::Thread::sleep(Configuration::PUBLISHER_RETRY_DELAY);
			}

			Core::MW::Thread::sleep(Core::MW::Time::ms(500));

			return true;
		} // onLoop
	};
}
