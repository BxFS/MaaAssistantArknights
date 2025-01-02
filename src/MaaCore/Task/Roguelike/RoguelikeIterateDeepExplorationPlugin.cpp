#include "RoguelikeIterateDeepExplorationPlugin.h"

#include "Config/TaskData.h"
#include "Controller/Controller.h"
#include "Task/ProcessTask.h"
#include "Utils/Logger.hpp"

bool asst::RoguelikeIterateDeepExplorationPlugin::load_params([[maybe_unused]] const json::value& params)
{
    LogTraceFunction;

    auto iterateDE = params.find<bool>("deep_exploration_auto_iterate");
    return iterateDE.value_or(false);
}

bool asst::RoguelikeIterateDeepExplorationPlugin::verify(AsstMsg msg, const json::value& details) const
{
    if (msg != AsstMsg::SubTaskStart || details.get("subtask", std::string()) != "ProcessTask") {
        return false;
    }

    if (!RoguelikeConfig::is_valid_theme(m_config->get_theme())) {
        Log.error("Roguelike name doesn't exist!");
        return false;
    }

    const std::string roguelike_name = m_config->get_theme() + "@";
    const auto& mode = m_config->get_mode();
    const std::string& task = details.get("details", "task", "");
    std::string_view task_view = task;
    if (task_view.starts_with(roguelike_name)) {
        task_view.remove_prefix(roguelike_name.length());
    }
    if (task_view == "Roguelike@StartExplore" && mode == RoguelikeMode::Exploration) {
        return true;
    }
    else {
        return false;
    }
}

bool asst::RoguelikeIterateDeepExplorationPlugin::_run()
{
    LogTraceFunction;

    // 检查主题，读深入调查数量
    if (m_config->get_theme() == "Phantom" || m_config->get_theme() == "Mizuki") {
        deep_exploration_count = 12;
    }
    else if (m_config->get_theme() == "Sami") {
        deep_exploration_count = 13;
    }
    else {
        deep_exploration_count = 0;
    }

    if (deep_exploration_count > 0) {
        ProcessTask(*this, { m_config->get_theme() + "@Roguelike@ChooseDeepExploration" }).run();
    }

    while (deep_exploration_count > 0) {
        deep_exploration_count--;
        if (!ProcessTask(*this, { m_config->get_theme() + "@Roguelike@DeepExplorationRewardMiss" }).run()) {
            break;
        }
    }
    // todo: 深入调查目标识别+策略适配
    // todo: 全部结束直接停止
    // todo: actually complain in wpf
    if (ProcessTask(*this, { m_config->get_theme() + "@Roguelike@DeepExplorationComplete" }).run()) {
        m_task_ptr->set_enable(false);
    }

    return true;
}
