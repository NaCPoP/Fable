#include "fbpch.h"
#include "VulkanApp.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	(void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
	fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
	return VK_FALSE;
}

namespace Fable
{
	VulkanApp::VulkanApp()
	{
		Init();
	}

    VulkanApp::~VulkanApp()
    {

    }

	VulkanApp* VulkanApp::VulkanCreate()
	{
		return new VulkanApp();
	}

    void VulkanApp::Shutdown()
    {
		vkDestroyInstance(m_Instance, nullptr);
    }

	void VulkanApp::Init()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

	#ifdef FB_DEBUG
		const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
		createInfo.enabledLayerCount = 1;
		createInfo.ppEnabledLayerNames = layers;
	#else
		if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vk instance!");
		}
	#endif // FB_DEBUG
	}
}