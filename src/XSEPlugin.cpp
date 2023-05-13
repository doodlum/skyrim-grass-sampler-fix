
#include <d3d11.h>

#include "RE/BSGraphics.h"
#include "RE/BSGraphicsTypes.h"

struct SamplerStatesCollection
{
	#define AddressMode 6
	#define FilterMode 5
	ID3D11SamplerState* states[AddressMode][FilterMode];

	static SamplerStatesCollection* GetSingleton()
	{
		REL::Relocation<SamplerStatesCollection*> instance{ REL::RelocationID(524751, 411366) };
		return instance.get();
	}
};

struct Hooks
{
	struct Grass_SetDirtyStates
	{
		static void thunk(bool a_compute)
		{
			func(a_compute);

			// Override grass sampler

			auto manager = RE::BSRenderManager::GetSingleton();
			auto context = manager->GetRuntimeData().context;

			context->PSSetSamplers(0, 1, &SamplerStatesCollection::GetSingleton()->states[0][3]);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	static void Install()
	{
		stl::write_thunk_call<Grass_SetDirtyStates>(REL::RelocationID(75479, 77265).address() + 0x61);
	}
};

bool Load()
{
	Hooks::Install();

	return true;
}