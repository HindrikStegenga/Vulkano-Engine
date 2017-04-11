//
// Created by hindrik on 8-4-17.
//

#ifndef VULKANOENGINE_RENDERTARGETOUTPUT_H
#define VULKANOENGINE_RENDERTARGETOUTPUT_H

#include "RenderMode.h"
#include <memory>

using std::unique_ptr;

class RenderTarget;

class RenderTargetOutput
{
private:
    unique_ptr<RenderMode> m_RenderMode;
public:
    RenderTargetOutput(RenderMode* renderMode);
    ~RenderTargetOutput() = default;

    RenderTargetOutput(RenderTargetOutput&&) = default;
    RenderTargetOutput(const RenderTargetOutput&) = delete;

    RenderTargetOutput& operator=(const RenderTargetOutput&) = delete;
    RenderTargetOutput&operator=(RenderTargetOutput&&) = default;

public:
    void render(float deltaTime) const;
    bool processAPI(float deltaTime) const;
};



#endif //VULKANOENGINE_RENDERTARGETOUTPUT_H