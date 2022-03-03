#include "CombatBehaviorNodes.h"

namespace Utils
{
	RE::BGSAttackData* get_attackData(RE::Actor* a) {
		if (!a->currentProcess || !a->currentProcess->high)
			return nullptr;
		return a->currentProcess->high->attackData.get();
	}

	float two_segments(float x, float Lx, float Mx, float Rx, float Ly, float My, float Ry)
	{
		float k;

		if (x <= Mx) {
			k = (x - Lx) / (Mx - Lx);
			if (k < 0.0f)
				k = 0.0f;
			return (My - Ly) * k + Ly;
		} else {
			k = (x - Mx) / (Rx - Mx);
			if (k > 1.0)
				k = 1.0f;
			return (Ry - My) * k + My;
		}
	}
}

void NiMemFree_14134A498(void* a1, uint64_t a2)
{
	return _generic_foo<102158, void, void*, uint64_t>(a1, a2);  // SkyrimSE.exe+134A498
}

RE::Character* CombatAI__get_he()
{
	return _generic_foo<46265, RE::Character*>();  // SkyrimSE.exe+7c7b20
}

RE::Character* CombatAI__get_me()
{
	return _generic_foo<46264, RE::Character*>();  // SkyrimSE.exe+7C7A40
}
