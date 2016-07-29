namespace Magma
{
	class Engine;
	class RendererProxy;
	class IGame
	{
	public:
		virtual void Init(Engine& aEngine) = 0;
		virtual bool Update(float aDelta) = 0;
		virtual void OnResize(float aWidth, float aHeight) = 0;

	protected:
		virtual ~IGame(){}
	};
}