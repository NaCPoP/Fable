#include "fbpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "Fable/Application.h"
#include <Platform/VulkanRendering/VulkanContext.h>
#include "Platform/VulkanRendering/VulkanBackend/VulkanUtilities.h"

namespace Fable
{
	ImGuiLayer::ImGuiLayer()
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		
		// TODO: TEMP
		auto context = static_cast<VulkanContext*>(&app.GetContext());

		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForVulkan(window, true);

		ImGui_ImplVulkan_InitInfo m_ImGuiInfo = {};
		m_ImGuiInfo.Instance = context->m_Instance;
		m_ImGuiInfo.PhysicalDevice = context->m_PhysicalDevice;
		m_ImGuiInfo.Device = context->m_Device;
		m_ImGuiInfo.Queue = context->m_GraphicsQueue;
		m_ImGuiInfo.DescriptorPool = context->m_ImguiPool;
		m_ImGuiInfo.Subpass = 0;
		m_ImGuiInfo.MinImageCount = 2;
		m_ImGuiInfo.ImageCount = 2;
		m_ImGuiInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		m_ImGuiInfo.Allocator = NULL;

		ImGui_ImplVulkan_Init(&m_ImGuiInfo, context->m_RenderPass);

		// FONTS
		VkCommandBuffer cmd = VulkanUtilities::beginSingleTimeCommands(context->m_CommandPool, context->m_Device);
		ImGui_ImplVulkan_CreateFontsTexture(cmd);
		VulkanUtilities::endSingleTimeCommands(cmd, context->m_CommandPool, context->m_Device, context->m_GraphicsQueue);

		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		// TODO: TEMP
		auto context = static_cast<VulkanContext*>(&app.GetContext());


		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), context->getCmdBuffer());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}