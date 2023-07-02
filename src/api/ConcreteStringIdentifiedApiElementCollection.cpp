#include "ConcreteStringIdentifiedApiElementCollection.h"
#include "flow-sdk/ApiElement.h"
#include "flow-sdk/ApiElementIterator.h"
#include "flow-sdk/FlightInformationRegion.h"
#include "flow-sdk/FlowMeasure.h"

namespace FlowSdk::Api {

    template<typename T>
    auto ConcreteStringIdentifiedApiElementCollection<T>::Count() const noexcept -> size_t
    {
        const auto uniqueLock = std::unique_lock(lock);
        return elementsById.size();
    }

    template<typename T>
    void ConcreteStringIdentifiedApiElementCollection<T>::Add(const std::shared_ptr<T>& element) noexcept
    {
        const auto uniqueLock = std::unique_lock(lock);
        elementsById[static_cast<std::shared_ptr<ApiElement>>(element)->Id()] = element;
        elementsByIdentifier[static_cast<std::shared_ptr<StringIdentifiableApiElement>>(element)->Identifier()] =
                element;
    }

    template<typename T>
    auto ConcreteStringIdentifiedApiElementCollection<T>::Get(int id) const noexcept -> std::shared_ptr<const T>
    {
        const auto uniqueLock = std::unique_lock(lock);
        return elementsById.contains(id) ? elementsById.at(id) : nullptr;
    }

    template<typename T>
    auto ConcreteStringIdentifiedApiElementCollection<T>::GetByIdentifier(const std::string& identifier) const noexcept
            -> std::shared_ptr<const T>
    {
        const auto uniqueLock = std::unique_lock(lock);
        return elementsByIdentifier.contains(identifier) ? elementsByIdentifier.at(identifier) : nullptr;
    }

    template<typename T>
    auto ConcreteStringIdentifiedApiElementCollection<T>::begin() const -> ApiElementIterator<T>
    {
        return ApiElementIterator<T>(lock, elementsById.begin());
    }

    template<typename T>
    auto ConcreteStringIdentifiedApiElementCollection<T>::end() const -> ApiElementIterator<T>
    {
        return ApiElementIterator<T>(lock, elementsById.end());
    }

    template<typename T>
    auto ConcreteStringIdentifiedApiElementCollection<T>::cbegin() const -> ApiElementIterator<T>
    {
        return ApiElementIterator<T>(lock, elementsById.cbegin());
    }

    template<typename T>
    auto ConcreteStringIdentifiedApiElementCollection<T>::cend() const -> ApiElementIterator<T>
    {
        return ApiElementIterator<T>(lock, elementsById.cend());
    }

    template class FlowSdk::Api::ConcreteStringIdentifiedApiElementCollection<
            FlowSdk::FlightInformationRegion::FlightInformationRegion>;
    template class FlowSdk::Api::ConcreteStringIdentifiedApiElementCollection<FlowSdk::FlowMeasure::FlowMeasure>;
}// namespace FlowSdk::Api
