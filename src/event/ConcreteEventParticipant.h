#pragma once
#include "flow-sdk/EventParticipant.h"

namespace FlowSdk::Event {
    class ConcreteEventParticipant : public EventParticipant
    {
        public:
        ConcreteEventParticipant(int cid, std::string originAirfield, std::string destinationAirfield);
        [[nodiscard]] auto Cid() const noexcept -> int override;
        [[nodiscard]] auto OriginAirfield() const noexcept -> const std::string& override;
        [[nodiscard]] auto DestinationAirfield() const noexcept -> const std::string& override;

        private:
        // The cid of the member
        int cid;

        // The origin airfield
        std::string originAirfield;

        // The destination airfield
        std::string destinationAirfield;
    };
}// namespace FlowSdk::Event
