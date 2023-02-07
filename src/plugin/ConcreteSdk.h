#pragma once
#include "flow-sdk/Sdk.h"

namespace FlowSdk::Plugin {

    class ConcreteSdk : public Sdk
    {
        public:
        ConcreteSdk(
                std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> notifiedListeners,
                std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> activatedListeners,
                std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> expiredListeners,
                std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> withdrawnListeners,
                std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure, FlowMeasure::FlowMeasure>> reissuedListeners);
        [[nodiscard]] auto FlowMeasureNotifiedListeners() const noexcept
                -> const EventListeners<FlowMeasure::FlowMeasure>& override;
        [[nodiscard]] auto FlowMeasureActivatedListeners() const noexcept
                -> const EventListeners<FlowMeasure::FlowMeasure>& override;
        [[nodiscard]] auto FlowMeasureExpiredListeners() const noexcept
                -> const EventListeners<FlowMeasure::FlowMeasure>& override;
        [[nodiscard]] auto FlowMeasureWithdrawnListeners() const noexcept
                -> const EventListeners<FlowMeasure::FlowMeasure>& override;
        [[nodiscard]] auto FlowMeasureReissuedListeners() const noexcept
                -> const EventListeners<FlowMeasure::FlowMeasure, FlowMeasure::FlowMeasure>& override;

        private:
        // The "notified" listeners
        std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> notifiedListeners;

        // The "activated" listeners
        std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> activatedListeners;

        // The "expired" listeners
        std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> expiredListeners;

        // The "withdrawn" listeners
        std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure>> withdrawnListeners;

        // The "reissued" listeners
        std::unique_ptr<EventListeners<FlowMeasure::FlowMeasure, FlowMeasure::FlowMeasure>> reissuedListeners;
    };
}// namespace FlowSdk::Plugin
