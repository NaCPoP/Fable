#pragma once

#ifdef FB_PLATFORM_WINDOWS

extern Fable::Application* Fable::CreateApplication();

int main(int argc, const char** argv)
{
	auto app = Fable::CreateApplication();

	try
	{
		app->Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	delete app;
}

#endif // FB_PLATFORM_WINDOWS
