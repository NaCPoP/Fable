#include <Fable.h>

class Sandbox : public Fable::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Fable::Application* Fable::CreateApplication()
{
	return new Sandbox();
}