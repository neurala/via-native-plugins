#include <exception>
#include <cstdio>

#include "cmsMultispectralLink.h"

int main() {
    try {
        auto cms = cmsMultiSpectralLink();

        cms.getCmsCamera()->setFrameRate(10.0); // Make this configurable.

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
