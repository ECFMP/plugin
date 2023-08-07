#include "ConcreteRangeToDestinationFilter.h"
#include "euroscope/EuroscopeAircraft.h"

namespace ECFMP::FlowMeasure {
    ConcreteRangeToDestinationFilter::ConcreteRangeToDestinationFilter(int range) noexcept : range(range)
    {}

    auto ConcreteRangeToDestinationFilter::Range() const noexcept -> int
    {
        return range;
    }

    bool ConcreteRangeToDestinationFilter::ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft
    ) const noexcept
    {
        return ((int) ceil(aircraft.RangeToDestination())) <= range;
    }
}// namespace ECFMP::FlowMeasure
