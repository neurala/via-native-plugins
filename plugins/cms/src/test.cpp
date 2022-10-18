#include <exception>
#include <fstream>
#include <cstdio>

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

#include "cmsMultispectralLink.h"

template <typename Type, typename Archive>
auto deserialize(Archive & archive, const char * property) {
    Type value;
    puts(property);
    archive(cereal::make_nvp(property, value));
    return value;
}

int main() {
    std::ifstream file("CMSConfiguration.json");
    cereal::JSONInputArchive archive(file);

    (deserialize<int>(archive, "blackLevel"));
    (deserialize<double>(archive, "exposureTime"));
    (deserialize<double>(archive, "frameRate"));
    (deserialize<double>(archive, "gain"));
    (deserialize<bool>(archive, "manufacturerSensorCorrection"));
    // (deserialize<int>(archive, "pixelClock"));
    (deserialize<int>(archive, "shutterMode"));
    (deserialize<int>(archive, "triggerMode"));

    return 0;

    try {
        auto cms = cmsMultiSpectralLink();

        (10.0); // Make this configurable.

        if (cms.getCmsInfos()->getSNCMS() == "") {
            puts("XML File Missing in ResourcesCMS");
        }

        const auto id = cms.getCmsInfos()->getTypeCamera();
        const auto idstr = id.c_str();

        printf("%s\n", idstr);
    } catch (const std::exception & exception) {
        const auto error = exception.what();
        printf("%s\n", error);
    } return 0;
}
