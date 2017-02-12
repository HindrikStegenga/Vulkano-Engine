//
// Created by hindrik on 27-1-17.
//

#ifndef VULKANO_ENGINE_VULKANCORESTRUCTS_H
#define VULKANO_ENGINE_VULKANCORESTRUCTS_H

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

using std::vector;
using std::string;

struct vk_physical_device_info {

    VkPhysicalDevice                m_physical_device;
    vector<VkQueueFamilyProperties> m_queue_family_properties;
    uint32_t                        m_queue_family_count;

};


struct vk_layer_extension_properties {

    VkLayerProperties               m_layer_properties;
    vector<VkExtensionProperties>   m_extensions_properties;

};


struct vk_device_rating {

    VkPhysicalDevice    m_device;
    int32_t             m_rating;

};





#endif //VULKANO_ENGINE_VULKANCORESTRUCTS_H
