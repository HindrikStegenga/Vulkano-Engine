//
// Created by hindrik on 30-1-17.
//

#include "Renderer.h"

#ifdef NDEBUG
    const bool enableValidationlayers = false;
#else
    const bool enableDebugLayers = true;
#endif


void Renderer::render(float deltaTime) {

}

bool Renderer::processAPI(float deltaTime) {
    return m_vk_core.processPlatformAPI(deltaTime);
}

Renderer::Renderer() : m_vk_core("Vulkano Engine", enabledKHRInstanceExtensions, enabledInstanceValidationLayers, enabledDeviceExtensions, enableDebugLayers)
{

}

Renderer::~Renderer() {

}