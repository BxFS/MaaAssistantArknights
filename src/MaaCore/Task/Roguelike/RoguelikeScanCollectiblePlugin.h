#pragma once
#include "AbstractRoguelikeTaskPlugin.h"

namespace asst
{

class RoguelikeScanCollectiblePlugin : public AbstractRoguelikeTaskPlugin
{
public:
    using AbstractRoguelikeTaskPlugin::AbstractRoguelikeTaskPlugin;
    virtual ~RoguelikeScanCollectiblePlugin() override = default;
    virtual bool verify(AsstMsg msg, const json::value& details) const override;
    virtual bool load_params(const json::value& params) override;

protected:
    virtual bool _run() override;

private:
    virtual bool try_task(const char*) const;
};
}
