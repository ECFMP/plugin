#include "ECFMP/SdkFactory.h"
#include "ECFMP/SdkEvents.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "ECFMP/http/HttpClient.h"
#include "ECFMP/log/Logger.h"
#include "InternalSdk.h"
#include "InternalSdkEvents.h"
#include "api/ApiDataDownloadedEvent.h"
#include "api/ApiDataDownloader.h"
#include "api/ApiDataParser.h"
#include "api/ApiDataScheduler.h"
#include "api/EventDataParser.h"
#include "api/FlightInformationRegionDataParser.h"
#include "api/FlowMeasureDataParser.h"
#include "api/FlowMeasureFilterParser.h"
#include "api/FlowMeasureMeasureParser.h"
#include "eventbus/InternalEventBusFactory.h"
#include "flowmeasure/FlowMeasureStatusUpdates.h"
#include "log/LogDecorator.h"
#include "log/NullLogger.h"

namespace ECFMP::Plugin {
    struct SdkFactory::SdkFactoryImpl {
        void CreateApiDataScheduler()
        {
            // Set up data listeners
            auto apiDataParser = std::make_shared<Api::ApiDataParser>(
                    std::make_shared<Api::EventDataParser>(GetLogger(), GetEventBus()),
                    std::make_shared<Api::FlightInformationRegionDataParser>(GetLogger(), GetEventBus()),
                    std::make_shared<Api::FlowMeasureDataParser>(
                            std::make_unique<Api::FlowMeasureFilterParser>(GetLogger()),
                            std::make_unique<Api::FlowMeasureMeasureParser>(GetLogger()), GetLogger(), GetEventBus()
                    ),
                    GetLogger()
            );
            GetEventBus()->SubscribeAsync<Api::ApiDataDownloadedEvent>(apiDataParser);

            // Set up data downloader
            auto dataDownloader =
                    std::make_shared<Api::ApiDataDownloader>(std::move(httpClient), GetEventBus(), GetLogger());
            GetEventBus()->SubscribeAsync<Plugin::ApiDataDownloadRequiredEvent>(dataDownloader);

            // Set up data scheduler
            auto dataScheduler = std::make_shared<Api::ApiDataScheduler>(GetEventBus());
            GetEventBus()->SubscribeSync<Plugin::EuroscopeTimerTickEvent>(dataScheduler);
        }

        void RegisterEventListeners()
        {
            // Create the API data scheduler
            this->CreateApiDataScheduler();

            // Flow measure status updates - powers the event-driven nature.
            GetEventBus()->SubscribeAsync<Plugin::InternalFlowMeasuresUpdatedEvent>(
                    std::make_shared<FlowMeasure::FlowMeasureStatusUpdates>(GetEventBus(), GetLogger())
            );
        }

        auto CreateLogger() -> std::shared_ptr<Log::Logger>
        {
            if (logger == nullptr) {
                logger = std::make_unique<Log::NullLogger>();
            }

            return std::make_shared<Log::LogDecorator>(std::move(logger));
        }

        auto GetLogger() -> std::shared_ptr<Log::Logger>
        {
            if (!wrappedLogger) {
                wrappedLogger = CreateLogger();
            }

            return wrappedLogger;
        }

        auto GetEventBus() -> std::shared_ptr<EventBus::InternalEventBus>
        {
            if (!eventBus) {
                eventBus = EventBus::MakeEventBus();
            }

            return eventBus;
        }

        // For performing HTTP requests
        std::unique_ptr<Http::HttpClient> httpClient = nullptr;

        // For logging things that happen
        std::unique_ptr<Log::Logger> logger = nullptr;

        // A wrapper around the logger
        std::shared_ptr<Log::Logger> wrappedLogger = nullptr;

        // For broadcasting events
        std::shared_ptr<EventBus::InternalEventBus> eventBus = nullptr;
    };

    SdkFactory::SdkFactory() : impl(std::make_unique<SdkFactoryImpl>())
    {}

    SdkFactory::~SdkFactory() = default;

    auto SdkFactory::Build() -> SdkFactory
    {
        return {};
    }

    auto SdkFactory::WithHttpClient(std::unique_ptr<Http::HttpClient> http) -> SdkFactory&
    {
        if (impl->httpClient) {
            throw SdkConfigurationException("Duplicate HTTP client provided");
        }

        impl->httpClient = std::move(http);
        return *this;
    }

    auto SdkFactory::WithLogger(std::unique_ptr<Log::Logger> log) -> SdkFactory&
    {
        if (impl->logger) {
            throw SdkConfigurationException("Duplicate logger provided");
        }

        impl->logger = std::move(log);
        return *this;
    }

    auto SdkFactory::Instance() -> std::shared_ptr<Sdk>
    {
        if (!impl->httpClient) {
            throw SdkConfigurationException("No http client provided");
        }

        // Set up other event listeners
        impl->RegisterEventListeners();

        // Set up the SDK
        auto sdk = std::make_shared<InternalSdk>(impl->GetEventBus());
        impl->GetEventBus()->SubscribeAsync<Plugin::FlightInformationRegionsUpdatedEvent>(sdk);
        impl->GetEventBus()->SubscribeAsync<Plugin::EventsUpdatedEvent>(sdk);
        impl->GetEventBus()->SubscribeAsync<Plugin::FlowMeasuresUpdatedEvent>(sdk);

        return std::move(sdk);
    }
}// namespace ECFMP::Plugin
