/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/mw/Subscriber.hpp>
#include <core/mw/CoreNode.hpp>

#include <sensors/SubscriberConfiguration.hpp>

#include <Configuration.hpp>
#include <Board.hpp>

/*
 * struct X {
   static bool
   callback(
      const common_msgs::XYZ_i16& msg,
      core::mw::Node* node
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
    public core::mw::CoreNode
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
    core::mw::Subscriber<MessageType, Configuration::SUBSCRIBER_QUEUE_LENGTH> _subscriber;
};
}
