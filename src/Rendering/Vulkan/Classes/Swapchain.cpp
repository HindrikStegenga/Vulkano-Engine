//
// Created by hindrik on 26-3-17.
//

#include <limits>
#include "Swapchain.h"

Swapchain::Swapchain(uint32_t width, uint32_t height, VkCore& vkCore, vk_queue presentQueue, VkSurfaceKHR surface) :
    m_PresentQueue(presentQueue),
    m_Height(height),
    m_Width(width),
    m_VkCore(vkCore),
    m_Swapchain({m_VkCore.device(), vkDestroySwapchainKHR}),
    m_Surface({surface, m_VkCore.instance(), vkDestroySurfaceKHR})
{
    createSwapchain();
}


auto Swapchain::fillSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) -> vk_swapchain_details
{

    vk_swapchain_details details    = {};
    uint32_t formatCount            = 0;
    uint32_t presentModeCount       = 0;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.m_Capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

    if(formatCount != 0)
    {
        details.m_Formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.m_Formats.data());
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    if(presentModeCount != 0)
    {
        details.m_PresentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.m_PresentModes.data());
    }

    return details;
}

auto Swapchain::checkSwapChainDetails(const vk_swapchain_details &details) -> bool
{
    return !details.m_PresentModes.empty() && !details.m_Formats.empty();
}

auto Swapchain::pickSwapChainSurfaceFormat(const vk_swapchain_details& details) -> VkSurfaceFormatKHR
{

    if(details.m_Formats.size() == 1 && details.m_Formats[0].format == VK_FORMAT_UNDEFINED)
    {
        return VkSurfaceFormatKHR{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto& format : details.m_Formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    return details.m_Formats[0];
}

auto Swapchain::pickSwapChainPresentMode(const vk_swapchain_details& details) -> VkPresentModeKHR
{

    for(const auto& presentMode : details.m_PresentModes)
    {
        if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return presentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

auto Swapchain::pickSwapChainExtent2D(const vk_swapchain_details& details, uint32_t width, uint32_t height) -> VkExtent2D
{

    if(details.m_Capabilities.currentExtent.width != std::numeric_limits<uint32_t >::max())
    {
        return details.m_Capabilities.currentExtent;
    }

    VkExtent2D newExtent = { width, height };
    newExtent.width = std::max(details.m_Capabilities.minImageExtent.width, std::min(details.m_Capabilities.maxImageExtent.width, newExtent.width));
    newExtent.height = std::max(details.m_Capabilities.minImageExtent.height, std::min(details.m_Capabilities.maxImageExtent.height, newExtent.height));

    return newExtent;
}

auto Swapchain::createSwapchain() -> void
{
    vk_swapchain_details details = fillSwapChainDetails(m_VkCore.physicalDevice(), m_Surface);
    if(!checkSwapChainDetails(details))
        throw std::runtime_error("Error, something wrong with the swapchain details.");

    m_SurfaceFormat = pickSwapChainSurfaceFormat(details);
    m_PresentMode = pickSwapChainPresentMode(details);
    m_Extent = pickSwapChainExtent2D(details, m_Width, m_Height);

    uint32_t imageCount = details.m_Capabilities.minImageCount + 1;

    if (details.m_Capabilities.maxImageCount > 0 && imageCount > details.m_Capabilities.maxImageCount) {
        imageCount = details.m_Capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};

    createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext                    = nullptr;
    createInfo.minImageCount            = imageCount;
    createInfo.imageFormat              = m_SurfaceFormat.format;
    createInfo.imageColorSpace          = m_SurfaceFormat.colorSpace;
    createInfo.imageExtent              = m_Extent;
    createInfo.imageArrayLayers         = 1;
    createInfo.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode         = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount    = 0;
    createInfo.pQueueFamilyIndices      = nullptr;
    createInfo.preTransform             = details.m_Capabilities.currentTransform;
    createInfo.compositeAlpha           = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode              = m_PresentMode;
    createInfo.clipped                  = VK_TRUE;
    createInfo.oldSwapchain             = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_VkCore.device(), &createInfo, nullptr, m_Swapchain.reset()) != VK_SUCCESS) {
        throw std::runtime_error("Error, failed to create swapchain!");
    }
}





























