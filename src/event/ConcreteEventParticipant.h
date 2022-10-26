#pragma once
#include "EventParticipant.h"

namespace FlowSdk::Event {
    class ConcreteEventParticipant : public EventParticipant
    {
        public:
        ConcreteEventParticipant(int cid, const char* originAirfield, const char* destinationAirfield);
        [[nodiscard]] auto Cid() const -> int override;
        [[nodiscard]] auto OriginAirfield() const -> const char* override;
        [[nodiscard]] auto DestinationAirfield() const -> const char* override;

        private:
        // The cid of the member
        int cid;

        // The origin airfield
        const char* originAirfield;

        // The destination airfield
        const char* destinationAirfield;
    };
}// namespace FlowSdk::Event