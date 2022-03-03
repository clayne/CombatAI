#pragma once

float get_Dummy_prop(RE::Actor*, RE::Actor*);
bool should_interruptBlock(RE::Actor* a);
bool should_interruptAttack(RE::Actor* a);

float get_OffensiveBash_prop(RE::Actor*, RE::Actor*);
float get_Block_prop(RE::Actor* me, RE::Actor* he);
float get_BashAfterAttack_prop(RE::Actor* me, RE::Actor*);
float get_DefensiveBash_prop(RE::Actor* me, RE::Actor* he);
float get_BlockType_prop(RE::Actor*, RE::Actor*);
float get_Attack_prop(RE::Actor*, RE::Actor*);
float get_SpecialAttack_prop(RE::Actor*, RE::Actor*);

bool has_stamina(RE::Actor* a);

namespace Movement
{
	namespace Fallback
	{
		float get_FallbackDistance(RE::Character* me);
		float get_FallbackSpeed(RE::Character*);
		uint32_t should_Fallback(RE::Character*);
	}

	namespace Circle
	{
		float get_CircleAngle(RE::Character* me);
		uint32_t should_Circle(RE::Character*);
	}

	namespace Advance
	{
		void hooked_interrupting(char** context);
		uint32_t should(RE::Character*);
	}

	uint32_t should_surround(RE::Character*);
	uint32_t low_stamina(RE::Character*);
	uint32_t is_indanger(RE::Character*);
	uint32_t is_wantattack(RE::Character*);
	bool is_outofrange(void*);
}
