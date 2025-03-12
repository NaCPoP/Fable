#include "fbpch.h"
#include "VulkanApp.h"

#define MAX_FRAMES_IN_FLIGHT 2

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

	std::vector<char> VulkanApp::readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	VkShaderModule VulkanApp::createShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(m_Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}

	void VulkanApp::drawFrame()
	{
		vkWaitForFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(m_Device, m_Swapchain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			ThrowError(result, "Failed to aquire swapchain image!");
		}

		vkResetFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame]);

		vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);
		recordCommandBuffer(m_CommandBuffers[m_CurrentFrame], imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		
		VkSemaphore waitSemaphore[] = {m_ImageAvailableSemaphores[m_CurrentFrame]};
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount	= 1;
		submitInfo.pWaitSemaphores		= waitSemaphore;
		submitInfo.pWaitDstStageMask	= waitStages;
		submitInfo.commandBufferCount	= 1;
		submitInfo.pCommandBuffers		= &m_CommandBuffers[m_CurrentFrame];

		VkSemaphore signalSemaphores[]	= { m_RenderFinishedSemaphores[m_CurrentFrame]};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores	= signalSemaphores;

		VkResult err = vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]);
		ThrowError(err, "Failed to submit draw command to buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount	= 1;
		presentInfo.pWaitSemaphores		= signalSemaphores;

		VkSwapchainKHR swapchains[]		= { m_Swapchain };
		presentInfo.swapchainCount		= 1;
		presentInfo.pSwapchains			= swapchains;
		presentInfo.pImageIndices		= &imageIndex;
		presentInfo.pResults			= nullptr;

		vkQueuePresentKHR(m_PresentQueue, &presentInfo);
		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanApp::createInstance()
	{
		VkResult err;

		// Create Vk Instance
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		VkInstanceCreateInfo createInfo{};
		createInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledExtensionCount	= glfwExtensionCount;
		createInfo.ppEnabledExtensionNames	= glfwExtensions;

	#ifdef FB_DEBUG
		const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };

		// ______________________________________________________________________________________________
		// FIX THIS
		const char* extensions[] = { glfwExtensions[0],
			glfwExtensions[1],
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
		// ______________________________________________________________________________________________

		createInfo.enabledLayerCount		= 1;
		createInfo.ppEnabledLayerNames		= validationLayers;
		createInfo.enabledExtensionCount	= glfwExtensionCount + 1;
		createInfo.ppEnabledExtensionNames	= extensions;

		err = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		ThrowError(err, "Failed to create debug instance");

		auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");

		VkDebugUtilsMessengerCreateInfoEXT debugReport = {};
		debugReport.sType			= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugReport.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugReport.messageType		= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugReport.pfnUserCallback = debugCallback;
		debugReport.pUserData		= nullptr;

		err = vkCreateDebugUtilsMessengerEXT(m_Instance, &debugReport, nullptr, &m_DebugMessenger);
		ThrowError(err, "Failed to create debug instance");
	#else

		err = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		ThrowError(err, "Failed to create instance");

	#endif // FB_DEBUG
	}

	void VulkanApp::createSurface()
	{
		VkResult err = glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface);
		ThrowError(err, "GLFW failed to create window surface");
	}

	void VulkanApp::pickPhysicalDevice()
	{
		// Selecting Physical Device
		uint32_t gpuCount = 0;
		VkResult err = vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr);
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
	}

	void VulkanApp::createLogicalDevice()
	{
		// Select Queue Family
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies);

		for (uint32_t i = 0; i < queueFamilyCount; i++)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_Indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, m_Surface, &presentSupport);

			if (presentSupport) {
				m_Indices.presentFamily = i;
			}
		}

		free(queueFamilies);

		// Create Logical Device
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		uint32_t uniqueQueueFamily[] = { m_Indices.graphicsFamily.value(), m_Indices.presentFamily.value() };
		const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamily)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex	= queueFamily;
			queueCreateInfo.queueCount			= 1;
			queueCreateInfo.pQueuePriorities	= &queuePriority;

			queueCreateInfos.emplace_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType						= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos			= queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount		= queueCreateInfos.size();
		deviceCreateInfo.enabledExtensionCount		= 1;
		deviceCreateInfo.ppEnabledExtensionNames	= deviceExtensions;
		deviceCreateInfo.pEnabledFeatures			= &deviceFeatures;

		VkResult err = vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device);
		ThrowError(err, "Failed to create logical device");

		vkGetDeviceQueue(m_Device, m_Indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, m_Indices.presentFamily.value(), 0, &m_PresentQueue);
	}

	void VulkanApp::createSwapchain()
	{
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

		VkSurfaceFormatKHR surfaceFormat	= swapchainDetails.formats[0];
		VkPresentModeKHR presentMode		= swapchainDetails.presentModes[0];
		VkExtent2D extent					= m_ActualExtent;

		uint32_t imageCount = swapchainDetails.capabilities.minImageCount + 1;
		if (swapchainDetails.capabilities.maxImageCount > 0 && imageCount > swapchainDetails.capabilities.maxImageCount)
		{
			imageCount = swapchainDetails.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.sType				= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface			= m_Surface;
		swapchainInfo.minImageCount		= imageCount;
		swapchainInfo.imageFormat		= surfaceFormat.format;
		swapchainInfo.imageColorSpace	= surfaceFormat.colorSpace;
		swapchainInfo.imageExtent		= extent;
		swapchainInfo.imageArrayLayers	= 1;
		swapchainInfo.imageUsage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilyIndices[] = { m_Indices.graphicsFamily.value(), m_Indices.presentFamily.value() };

		if (m_Indices.graphicsFamily != m_Indices.presentFamily)
		{
			swapchainInfo.imageSharingMode		= VK_SHARING_MODE_CONCURRENT;
			swapchainInfo.queueFamilyIndexCount = 2;
			swapchainInfo.pQueueFamilyIndices	= queueFamilyIndices;
		}
		else
		{
			swapchainInfo.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
			swapchainInfo.queueFamilyIndexCount = 0;
			swapchainInfo.pQueueFamilyIndices	= nullptr;
		}

		swapchainInfo.preTransform		= swapchainDetails.capabilities.currentTransform;
		swapchainInfo.compositeAlpha	= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode		= presentMode;
		swapchainInfo.clipped			= VK_TRUE;
		swapchainInfo.oldSwapchain		= VK_NULL_HANDLE;

		VkResult err = vkCreateSwapchainKHR(m_Device, &swapchainInfo, nullptr, &m_Swapchain);
		ThrowError(err, "Failed to create swapchain!");

		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, nullptr);
		m_SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, m_SwapchainImages.data());

		m_SwapchainImageFormat = surfaceFormat.format;
		m_SwapchainExtent = extent;
	}

	void VulkanApp::createImageViews()
	{
		m_SwapchainImageViews.resize(m_SwapchainImages.size());

		for (size_t i = 0; i < m_SwapchainImages.size(); i++)
		{
			VkImageViewCreateInfo imageViewInfo{};
			imageViewInfo.sType								= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewInfo.image								= m_SwapchainImages[i];
			imageViewInfo.viewType							= VK_IMAGE_VIEW_TYPE_2D;
			imageViewInfo.format							= m_SwapchainImageFormat;
			imageViewInfo.components.r						= VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.g						= VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.b						= VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.a						= VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewInfo.subresourceRange.baseMipLevel		= 0;
			imageViewInfo.subresourceRange.levelCount		= 1;
			imageViewInfo.subresourceRange.baseArrayLayer	= 0;
			imageViewInfo.subresourceRange.layerCount		= 1;

			VkResult err = vkCreateImageView(m_Device, &imageViewInfo, nullptr, &m_SwapchainImageViews[i]);
			ThrowError(err, "Failed to create image views!");
		}
	}

	void VulkanApp::createRenderPass()
	{
		// Render Pass
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format			= m_SwapchainImageFormat;
		colorAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment	= 0;
		colorAttachmentRef.layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount	= 1;
		subpass.pColorAttachments		= &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass			= VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass			= 0;
		dependency.srcStageMask			= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask		= 0;
		dependency.dstStageMask			= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask		= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount	= 1;
		renderPassInfo.pAttachments		= &colorAttachment;
		renderPassInfo.subpassCount		= 1;
		renderPassInfo.pSubpasses		= &subpass;
		renderPassInfo.dependencyCount	= 1;
		renderPassInfo.pDependencies	= &dependency;

		VkResult err = vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass);
		ThrowError(err, "Failed to create render pass!");
	}

	void VulkanApp::createGraphicsPipeline()
	{
		// PIPELINE
		// Shader Modules
		auto vertShaderCode = readFile("C:/dev/Fable/Fable/src/Shaders/vert.spv");
		auto fragShaderCode = readFile("C:/dev/Fable/Fable/src/Shaders/frag.spv");

		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage	= VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module	= vertShaderModule;
		vertShaderStageInfo.pName	= "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage	= VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module	= fragShaderModule;
		fragShaderStageInfo.pName	= "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		// Dynamic State
		std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType				= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount	= static_cast<uint32_t>(dynamicStates.size());
		dynamicStateInfo.pDynamicStates		= dynamicStates.data();

		// Vertex Input
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount	= 0;
		vertexInputInfo.pVertexBindingDescriptions		= nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions	= nullptr;

		// Input Assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType						= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology					= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable	= VK_FALSE;

		// Viewports and Scissors
		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount	= 1;

		// Rasterizer
		VkPipelineRasterizationStateCreateInfo  rasterizer{};
		rasterizer.sType					= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable			= VK_FALSE;
		rasterizer.rasterizerDiscardEnable	= VK_FALSE;
		rasterizer.polygonMode				= VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth				= 1.0f;
		rasterizer.cullMode					= VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace				= VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable			= VK_FALSE;
		rasterizer.depthBiasConstantFactor	= 0.0f;
		rasterizer.depthBiasClamp			= 0.0f;
		rasterizer.depthBiasSlopeFactor		= 0.0f;

		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable	= VK_FALSE;
		multisampling.rasterizationSamples	= VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading		= 1.0f;
		multisampling.pSampleMask			= nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable		= VK_FALSE;

		// Color blending
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask			= VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable			= VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor	= VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor	= VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp			= VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor	= VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor	= VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp			= VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable		= VK_FALSE;
		colorBlending.logicOp			= VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount	= 1;
		colorBlending.pAttachments		= &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount			= 0;
		pipelineLayoutInfo.pushConstantRangeCount	= 0;

		VkResult err = vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);
		ThrowError(err, "Failed to create pipeline layout!");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType					= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount				= 2;
		pipelineInfo.pStages				= shaderStages;
		pipelineInfo.pVertexInputState		= &vertexInputInfo;
		pipelineInfo.pInputAssemblyState	= &inputAssembly;
		pipelineInfo.pViewportState			= &viewportState;
		pipelineInfo.pRasterizationState	= &rasterizer;
		pipelineInfo.pMultisampleState		= &multisampling;
		pipelineInfo.pDepthStencilState		= nullptr;
		pipelineInfo.pColorBlendState		= &colorBlending;
		pipelineInfo.pDynamicState			= &dynamicStateInfo;
		pipelineInfo.layout					= m_PipelineLayout;
		pipelineInfo.renderPass				= m_RenderPass;
		pipelineInfo.subpass				= 0;
		pipelineInfo.basePipelineHandle		= VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex		= -1;

		err = vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);
		ThrowError(err, "Failed to create graphics pipeline!");

		vkDestroyShaderModule(m_Device, vertShaderModule, nullptr);
		vkDestroyShaderModule(m_Device, fragShaderModule, nullptr);
	}

	void VulkanApp::createFramebuffers()
	{
		// Framebuffer
		m_SwapchainFrameBuffer.resize(m_SwapchainImageViews.size());
		for (size_t i = 0; i < m_SwapchainImageViews.size(); i++)
		{
			VkImageView attachments[] = { m_SwapchainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass		= m_RenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments	= attachments;
			framebufferInfo.width			= m_SwapchainExtent.width;
			framebufferInfo.height			= m_SwapchainExtent.height;
			framebufferInfo.layers			= 1;

			VkResult err = vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_SwapchainFrameBuffer[i]);
			ThrowError(err, "Failed to create frambuffer!");
		}
	}

	void VulkanApp::createCommandPool()
	{
		// Command Pool
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = m_Indices.graphicsFamily.value();

		VkResult err = vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool);
		ThrowError(err, "Failed to create command pool!");
	}

	void VulkanApp::createCommandBuffer()
	{
		m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t) m_CommandBuffers.size();

		VkResult err = vkAllocateCommandBuffers(m_Device, &allocInfo, m_CommandBuffers.data());
		ThrowError(err, "Failed to allocate command buffers!");
	}

	void VulkanApp::createSyncObjects()
	{
		m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(m_Device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create synchronization objects for a frame!");
			}
		}
	}

	void VulkanApp::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VkResult err = vkBeginCommandBuffer(m_CommandBuffers[m_CurrentFrame], &beginInfo);
		// Render Pass
		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass			= m_RenderPass;
		renderPassBeginInfo.framebuffer			= m_SwapchainFrameBuffer[imageIndex];
		renderPassBeginInfo.renderArea.offset	= { 0, 0 };
		renderPassBeginInfo.renderArea.extent	= m_SwapchainExtent;

		VkClearValue clearColor = { {{ 0.0f, 0.0f, 0.0f, 1.0f }} };

		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentFrame], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(m_CommandBuffers[m_CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
		// Viewports and Scissors
		VkViewport viewport{};
		viewport.x			= 0.0f;
		viewport.y			= 0.0f;
		viewport.width		= (float)m_SwapchainExtent.width;
		viewport.height		= (float)m_SwapchainExtent.height;
		viewport.minDepth	= 0.0f;
		viewport.maxDepth	= 1.0f;
		vkCmdSetViewport(m_CommandBuffers[m_CurrentFrame], 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapchainExtent;
		vkCmdSetScissor(m_CommandBuffers[m_CurrentFrame], 0, 1, &scissor);

		vkCmdDraw(m_CommandBuffers[m_CurrentFrame], 3, 1, 0, 0);

		vkCmdEndRenderPass(m_CommandBuffers[m_CurrentFrame]);

		err = vkEndCommandBuffer(m_CommandBuffers[m_CurrentFrame]);
		ThrowError(err, "Failed to record command buffer!");
	}

    void VulkanApp::Shutdown()
    {
		cleanUpSwapchain();

		vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
		vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
			vkDestroyFence(m_Device, m_InFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

		vkDestroyDevice(m_Device, nullptr);
	#ifdef FB_DEBUG
		auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
		vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
	#endif
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
    }

	void VulkanApp::recreateSwapchain()
	{
		vkDeviceWaitIdle(m_Device);
		cleanUpSwapchain();

		createSwapchain();
		createImageViews();
		createFramebuffers();
	}

	void VulkanApp::cleanUpSwapchain()
	{
		for (auto framebuffer : m_SwapchainFrameBuffer)
		{
			vkDestroyFramebuffer(m_Device, framebuffer, nullptr);
		}

		for (auto imageView : m_SwapchainImageViews)
		{
			vkDestroyImageView(m_Device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
	}

	void VulkanApp::Init()
	{
		createInstance();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapchain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createCommandBuffer();
		createSyncObjects();
	}
}