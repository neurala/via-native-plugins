#include <neurala/plugin/detail/PluginBindings.h>
#include <neurala/plugin/detail/PluginManager.h>

// extern "C" NeuralaPluginExitFunction initMe(NeuralaPluginManager* pluginManager,
// NeuralaPluginStatus* status)
// {
// 	auto& pm = *static_cast<neurala::PluginManager*>(pluginManager);
// 	*status = pm.registerPlugin<neurala::Input>("Input", neurala::Version(1, 0));
// 	if (*status != NeuralaPluginStatus::success) return nullptr;
// 	*status = pm.registerPlugin<neurala::Output>("Output", neurala::Version(1, 0));
// 	if (*status != NeuralaPluginStatus::success) return nullptr;
// 	return [] { return 0; };
// }
