#pragma once
#include "ApiDataListenerTypes.h"
#include "InternalElementCollectionTypes.h"
#include "nlohmann/json_fwd.hpp"

namespace FlowSdk {
    namespace FlowMeasure {
        class Measure;
    }// namespace FlowMeasure
    namespace Log {
        class Logger;
    }// namespace Log
}// namespace FlowSdk

namespace FlowSdk::Api {
    class FlowMeasureMeasureParserInterface
    {
        public:
        virtual ~FlowMeasureMeasureParserInterface() = default;
        [[nodiscard]] virtual auto Parse(const nlohmann::json& data) const -> std::unique_ptr<FlowMeasure::Measure> = 0;
    };
}// namespace FlowSdk::Api