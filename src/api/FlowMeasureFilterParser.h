#pragma once
#include "FlowMeasureFilterParserInterface.h"
#include "flow-sdk/FlowMeasureFilters.h"
#include "flow-sdk/LevelRangeFilter.h"
#include "flow-sdk/MultipleLevelFilter.h"
#include "flow-sdk/RangeToDestinationFilter.h"

namespace FlowSdk {
    namespace FlowMeasure {
        class AirportFilter;
        class EventFilter;
        class RouteFilter;
    }// namespace FlowMeasure
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace FlowSdk

namespace FlowSdk::Api {
    class FlowMeasureFilterParser : public FlowMeasureFilterParserInterface
    {
        public:
        explicit FlowMeasureFilterParser(const std::shared_ptr<FlowSdk::Log::Logger>& logger);
        [[nodiscard]] auto Parse(const nlohmann::json& data, const InternalEventCollection& events) const
                -> std::unique_ptr<FlowMeasure::FlowMeasureFilters> override;

        private:
        [[nodiscard]] auto CreateAirportFilter(const nlohmann::json& data, const std::string& type) const
                -> std::shared_ptr<FlowMeasure::AirportFilter>;

        [[nodiscard]] auto
        CreateLevelRangeFilter(const nlohmann::json& data, FlowMeasure::LevelRangeFilterType type) const
                -> std::shared_ptr<FlowMeasure::LevelRangeFilter>;

        [[nodiscard]] auto CreateMultipleLevelFilter(const nlohmann::json& data) const
                -> std::shared_ptr<FlowMeasure::MultipleLevelFilter>;

        [[nodiscard]] auto CreateEventFilter(const nlohmann::json& data, const InternalEventCollection& events) const
                -> std::shared_ptr<FlowMeasure::EventFilter>;

        [[nodiscard]] auto CreateRouteFilter(const nlohmann::json& data) const
                -> std::shared_ptr<FlowMeasure::RouteFilter>;

        [[nodiscard]] auto CreateRangeToDestinationFilter(const nlohmann::json& data) const
                -> std::shared_ptr<FlowMeasure::RangeToDestinationFilter>;

        // Logger
        std::shared_ptr<FlowSdk::Log::Logger> logger;
    };
}// namespace FlowSdk::Api