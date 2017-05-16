/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#include <core/mw/Publisher.hpp>
#include <core/mw/CoreNode.hpp>

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
template <class _MESSAGETYPE, class _CALLBACK>
class DummyPublisher:
    public core::mw::CoreNode
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
    core::mw::Publisher<MessageType> _publisher;

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
            core::os::Thread::sleep(Configuration::PUBLISHER_RETRY_DELAY);
        }

        core::os::Thread::sleep(core::os::Time::ms(500));

        return true;
    }   // onLoop
};
}
