//
// Created by hindrik on 30-1-17.
//

#include <cstring>
#include <cstdlib>
#include "VulkanPlatform.h"
#include "VK_XCB_PLATFORM.h"


VkResult CreateXCBSurfaceKHR(VkInstance instance, VkXcbSurfaceCreateInfoKHR info, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR& surface) {
    auto func = (PFN_vkCreateXcbSurfaceKHR) vkGetInstanceProcAddr(instance, "vkCreateXcbSurfaceKHR");
    if (func != nullptr) {
        func(instance, &info, pAllocator, &surface);
        return VK_SUCCESS;
    } else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}


VulkanPlatform::VulkanPlatform()
{
        m_connection = xcb_connect(NULL, NULL);

        const xcb_setup_t      *setup  = xcb_get_setup (m_connection);
        xcb_screen_iterator_t   iter   = xcb_setup_roots_iterator (setup);
        xcb_screen_t           *screen = iter.data;

        m_window = xcb_generate_id (m_connection);



        uint32_t  eventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        uint32_t vals[] = { 0,0 };

        xcb_create_window (m_connection,                    /* Connection          */
                           XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                           m_window,                        /* m_window Id           */
                           screen->root,                  /* parent m_window       */
                           0, 0,                          /* x, y                */
                           WIDTH, HEIGHT,                      /* width, height       */
                           10,                            /* border_width        */
                           XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                           screen->root_visual,           /* visual              */
                           eventMask, vals );                     /* masks, not used yet */

    xcb_intern_atom_cookie_t wmDeleteCookie = xcb_intern_atom(
            m_connection, 0, (uint16_t) strlen("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW");
    xcb_intern_atom_cookie_t wmProtocolsCookie =
            xcb_intern_atom(m_connection, 0, (uint16_t) strlen("WM_PROTOCOLS"), "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *wmDeleteReply =
            xcb_intern_atom_reply(m_connection, wmDeleteCookie, NULL);
    xcb_intern_atom_reply_t *wmProtocolsReply =
            xcb_intern_atom_reply(m_connection, wmProtocolsCookie, NULL);

    m_wmDeleteWin = wmDeleteReply->atom;
    m_wmProtocols = wmProtocolsReply->atom;

    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_window,
                        wmProtocolsReply->atom, 4, 32, 1, &wmDeleteReply->atom);

        xcb_map_window (m_connection, m_window);
        xcb_flush (m_connection);
}

VulkanPlatform::~VulkanPlatform()
{
    if(m_connection != nullptr)
    {
        xcb_disconnect(m_connection);
        m_connection = nullptr;
    }
}

xcb_connection_t *VulkanPlatform::getConnection() {
    return m_connection;
}

xcb_window_t VulkanPlatform::getWindow() {
    return m_window;
}

bool VulkanPlatform::processAPI(float deltaTime) {

    xcb_generic_event_t *event;
    xcb_client_message_event_t *cm;

        event = xcb_wait_for_event(m_connection);

        switch (event->response_type & ~0x80) {
            case XCB_CLIENT_MESSAGE: {
                cm = (xcb_client_message_event_t *)event;

                if (cm->data.data32[0] == m_wmDeleteWin)
                    return false;

                break;
            }
        }

    free(event);

    return true;
}

VkResult VulkanPlatform::createSurface(VkInstance& instance, VkSurfaceKHR& surface) {

    VkXcbSurfaceCreateInfoKHR info = {};

    info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.connection = m_connection;
    info.window = m_window;

    return CreateXCBSurfaceKHR(instance, info, nullptr, surface);
}
