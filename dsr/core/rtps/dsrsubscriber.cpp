// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file cadenaSubscriber.cpp
 * This file contains the implementation of the subscriber functions.
 *
 * This file was generated by the tool fastcdrgen.
 */

#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/Domain.h>

#include "dsrsubscriber.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

DSRSubscriber::DSRSubscriber() : mp_participant(nullptr), mp_subscriber(nullptr) {}

DSRSubscriber::~DSRSubscriber() {
    //Domain::removeParticipant(mp_participant);
}

bool DSRSubscriber::init(eprosima::fastrtps::Participant *mp_participant_,
                         const char *topicName, const char *topicDataType,
                         std::function<void(Subscriber *sub)> f_) {
    mp_participant = mp_participant_;

    // Create Subscriber
    SubscriberAttributes Rparam;
    Rparam.topic.topicKind = NO_KEY;
    Rparam.topic.topicDataType = topicDataType; //Must be registered before the creation of the subscriber
    Rparam.topic.topicName = topicName;
    eprosima::fastrtps::rtps::Locator_t locator;
    IPLocator::setIPv4(locator, 239, 255, 0, 1);
    locator.port = 7900;
    Rparam.multicastLocatorList.push_back(locator);
    Rparam.qos.m_reliability.kind = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;
    Rparam.historyMemoryPolicy = eprosima::fastrtps::rtps::DYNAMIC_RESERVE_MEMORY_MODE; //PREALLOCATED_WITH_REALLOC_MEMORY_MODE;

    /*
    if (std::string_view(topicName) == "DSR") {
        // This would be better, but we sent a lots of messages to use it.
        //Wparam.topic.historyQos.kind = KEEP_ALL_HISTORY_QOS;

    }
     */
    Rparam.topic.historyQos.kind = KEEP_LAST_HISTORY_QOS;
    Rparam.topic.historyQos.depth = 20; // Adjust this value if we are losing  messages

    Rparam.topic.resourceLimitsQos.max_samples = 200;
    m_listener.participant_ID = mp_participant->getGuid();
    m_listener.f = f_;

    mp_subscriber = Domain::createSubscriber(mp_participant, Rparam, static_cast<SubscriberListener *>(&m_listener));
    if (mp_subscriber == nullptr)
        return false;
    return true;
}


eprosima::fastrtps::Subscriber *DSRSubscriber::getSubscriber() {
    return mp_subscriber;
}


///////////////////////////////////////////
/// Callbacks
///////////////////////////////////////////

void DSRSubscriber::SubListener::onSubscriptionMatched(Subscriber *sub, MatchingInfo &info) {
    (void) sub;
    if (info.status == eprosima::fastrtps::rtps::MATCHED_MATCHING) {
        n_matched++;
        std::cout << "Publisher[" << sub->getAttributes().topic.getTopicName() <<"] matched " << info.remoteEndpointGuid << std::endl;
    } else {
        n_matched--;
        std::cout << "Publisher[" << sub->getAttributes().topic.getTopicName() <<"] unmatched "  << info.remoteEndpointGuid << std::endl;
    }
}

void DSRSubscriber::SubListener::onNewDataMessage(Subscriber *sub) {
    f(sub);
}

void DSRSubscriber::run() {
    std::cout << "Waiting for Data, press Enter to stop the Subscriber. " << std::endl;
    while (true);
}
