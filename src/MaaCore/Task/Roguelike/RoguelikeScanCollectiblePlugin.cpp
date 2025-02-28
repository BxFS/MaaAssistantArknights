#include "RoguelikeScanCollectiblePlugin.h"

#include "Config/TaskData.h"
#include "Controller/Controller.h"
#include "Task/ProcessTask.h"
#include "Utils/Logger.hpp"

bool asst::RoguelikeScanCollectiblePlugin::load_params([[maybe_unused]] const json::value& params)
{
    LogTraceFunction;

    auto SC = params.find<bool>("scan_collectible");
    return SC.value_or(false);
}

bool asst::RoguelikeScanCollectiblePlugin::verify(AsstMsg msg, const json::value& details) const
{
    if (msg != AsstMsg::SubTaskStart || details.get("subtask", std::string()) != "ProcessTask") {
        return false;
    }

    if (!RoguelikeConfig::is_valid_theme(m_config->get_theme())) {
        Log.error("Roguelike name doesn't exist!");
        return false;
    }

    const std::string roguelike_name = m_config->get_theme() + "@";
    const std::string& task = details.get("details", "task", "");
    std::string_view task_view = task;
    if (task_view.starts_with(roguelike_name)) {
        task_view.remove_prefix(roguelike_name.length());
    }
    if (task_view == "Roguelike@StartExplore") {
        return true;
    }
    else {
        return false;
    }
}

bool asst::RoguelikeScanCollectiblePlugin::_run()
{
    LogTraceFunction;

    return true;
}

bool asst::RoguelikeScanCollectiblePlugin::try_task(const char* task) const
{
    return ProcessTask(*this, { m_config->get_theme() + task }).run();
}
