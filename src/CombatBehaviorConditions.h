#pragma once

namespace Movement
{
	namespace Fallback
	{
		float get_FallbackDistance(RE::Character* me);
		float get_FallbackSpeed(RE::Character*);
		uint32_t should(RE::Character*);
		uint32_t should_normal(RE::Character* me);
		float get_FallbackWaitTime(RE::Character*);
	}

	namespace Circle
	{
		float get_CircleAngle(RE::Character* me);
		uint32_t should_danger(RE::Character*);
		uint32_t should_normal(RE::Character* me);
	}

	namespace Advance
	{
		void hooked_interrupting(char** context);
		uint32_t should(RE::Character*);
		void fix_radiuses(RE::Actor* me, RE::Actor* he, float* inner_R, float* outer_R);
		bool should_fallback_to_ranged(void* a1);
	}

	namespace Surround
	{
		void hooked_interrupting(char** context);
		uint32_t should(RE::Actor*);
	}

	uint32_t low_stamina(RE::Character*);
	uint32_t is_indanger(RE::Character*);
	bool is_outofrange(void*);
}

namespace Attack
{
	uint32_t dontwant(RE::Character* a);
	float get_thisattack_chance(RE::Actor* me, RE::Actor* he, RE::BGSAttackData* adata);
	bool should_interrupt();
}

namespace Block
{
	uint32_t wantbash(RE::Character* a);
	uint32_t wantblock(RE::Character* a);
	float get_BashAfterBlock_prop(RE::Actor* me, RE::Actor*);
}
