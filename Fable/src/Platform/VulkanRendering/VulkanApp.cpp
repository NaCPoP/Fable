#include "fbpch.h"
#include "VulkanApp.h"

#ifdef FB_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}
#endif

// TEMP
static void ThrowError(VkResult err, std::string message)
{
	if (err != VK_SUCCESS)
	{
		throw std::runtime_error(message);
	}
}

namespace Fable
{
	VulkanApp::VulkanApp(GLFWwindow* window) : m_Window(window)
	{
		Init();
	}

    VulkanApp::~VulkanApp()
    {

    }

	VulkanApp* VulkanApp::CreateVulkanApp(GLFWwindow* window)
	{
		return new VulkanApp(window);
	}

    void VulkanApp::Shutdown()
    {
		vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
		vkDestroyDevice(m_Device, nullptr);
	#ifdef FB_DEBUG
		auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
		vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
	#endif
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
    }

	void VulkanApp::Init()
	{
		VkResult err;

		// Create Vk Instance
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

	#ifdef FB_DEBUG
		const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };

		// ______________________________________________________________________________________________
		// FIX THIS
		const char* extensions[] = { glfwExtensions[0],
			glfwExtensions[1],
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
		// ______________________________________________________________________________________________

		createInfo.enabledLayerCount = 1;
		createInfo.ppEnabledLayerNames = validationLayers;
		createInfo.enabledExtensionCount = glfwExtensionCount + 1;
		createInfo.ppEnabledExtensionNames = extensions;
		
		err = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		ThrowError(err, "Failed to create debug instance");

		auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");

		VkDebugUtilsMessengerCreateInfoEXT debugReport = {};
		debugReport.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugReport.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugReport.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugReport.pfnUserCallback = debugCallback;
		debugReport.pUserData = nullptr;

		err = vkCreateDebugUtilsMessengerEXT(m_Instance, &debugReport, nullptr, &m_DebugMessenger);
		ThrowError(err, "Failed to create debug instance");
	#else

		err = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		ThrowError(err, "Failed to create instance");

	#endif // FB_DEBUG

		// GLFW Create Surface
		err = glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface);
		ThrowError(err, "GLFW failed to create window surface");

		// Selecting Physical Device
		uint32_t gpuCount = 0;
		err = vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr);
		ThrowError(err, "Failed to find GPU");

		VkPhysicalDevice* gpus = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpuCount);
		err = vkEnumeratePhysicalDevices(m_Instance, &gpuCount, gpus);
		ThrowError(err, "Failed to find GPU");

		int useGPU = 0;
		for (uint32_t i = 0; i < gpuCount; i++)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(gpus[i], &properties);
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				useGPU = i;
				break;
			}
		}

		m_PhysicalDevice = gpus[useGPU];
		free(gpus);

		// Select Queue Family
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies);

		for (uint32_t i = 0; i < queueFamilyCount; i++)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, m_Surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}
		}

		free(queueFamilies);

		// Create Logical Device
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		uint32_t uniqueQueueFamily[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamily)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.emplace_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
		deviceCreateInfo.enabledExtensionCount = 1;
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	#ifdef FB_DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(sizeof(validationLayers));
		createInfo.ppEnabledLayerNames = validationLayers;
	#endif

		err = vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device);
		ThrowError(err, "Failed to create logical device");

		vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);

		// Window Surface
		VkWin32SurfaceCreateInfoKHR windowCreateInfo{};
		windowCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		windowCreateInfo.hwnd = glfwGetWin32Window(m_Window);
		windowCreateInfo.hinstance = GetModuleHandle(nullptr);

		err = vkCreateWin32SurfaceKHR(m_Instance, &windowCreateInfo, nullptr, &m_Surface);
		ThrowError(err, "Failed to create window surface");

		// Swapchain
		SwapChainSupportDetails swapchainDetails;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, m_Surface, &swapchainDetails.capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			swapchainDetails.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &formatCount, swapchainDetails.formats.data());
		}

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			swapchainDetails.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &presentModeCount, swapchainDetails.presentModes.data());
		}
		
		if (swapchainDetails.capabilities.currentExtent.width == UINT32_MAX)
		{
			int width, height;
			glfwGetFramebufferSize(m_Window, &width, &height);

			m_ActualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			m_ActualExtent.width = std::clamp(m_ActualExtent.width, swapchainDetails.capabilities.minImageExtent.width, swapchainDetails.capabilities.maxImageExtent.width);
			m_ActualExtent.height = std::clamp(m_ActualExtent.height, swapchainDetails.capabilities.minImageExtent.height, swapchainDetails.capabilities.maxImageExtent.height);
		}
		else
		{
			m_ActualExtent = swapchainDetails.capabilities.currentExtent;
		}

		VkSurfaceFormatKHR surfaceFormat = swapchainDetails.formats[0];
		VkPresentModeKHR presentMode = swapchainDetails.presentModes[0];
		VkExtent2D extent = m_ActualExtent;

		uint32_t imageCount = swapchainDetails.capabilities.minImageCount + 1;
		if (swapchainDetails.capabilities.maxImageCount > 0 && imageCount > swapchainDetails.capabilities.maxImageCount)
		{
			imageCount = swapchainDetails.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = m_Surface;
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainInfo.imageExtent = extent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily)
		{
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainInfo.queueFamilyIndexCount = 2;
			swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainInfo.queueFamilyIndexCount = 0;
			swapchainInfo.pQueueFamilyIndices = nullptr;
		}

		swapchainInfo.preTransform = swapchainDetails.capabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.clipped = VK_TRUE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

		err = vkCreateSwapchainKHR(m_Device, &swapchainInfo, nullptr, &m_Swapchain);
		ThrowError(err, "Failed to create swapchain!");

		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, nullptr);
		m_SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, m_SwapchainImages.data());

		m_SwapchainImageFormat = surfaceFormat.format;
		m_SwapchainExtent = extent;
	}
}