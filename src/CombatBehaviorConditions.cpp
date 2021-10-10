#include "UselessFenixUtils.h"

bool has_manyStamina(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kStamina);
	auto total = a->GetBaseActorValue(RE::ActorValue::kStamina);
	return total >= 150.0f && total * 0.75 <= cur;
}

bool has_enoughStamina(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kStamina);
	auto total = a->GetBaseActorValue(RE::ActorValue::kStamina);
	return total >= 150.0f && total * 0.25 <= cur;
}

bool is_powerattacking(RE::Actor* a)
{
	return _generic_foo<37639, bool, RE::Actor*>(a);
}

bool has_lowStamina(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kStamina);
	return cur <= 15.0f;
}

bool has_stamina(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kStamina);
	auto total = a->GetBaseActorValue(RE::ActorValue::kStamina);
	return total <= 20.0f || cur >= 15.0f;
}

bool is_staggered(RE::Actor* a)
{
	return _generic_foo<46064, bool, RE::Actor*>(a);
}

bool is_blocking(RE::Actor* a)
{
	return _generic_foo<36927, bool, RE::Actor*>(a);
}

bool is_attacking(RE::Actor* a)
{
	auto proc = a->currentProcess;
	if (!proc)
		return false;
	auto high = proc->high;
	if (!high)
		return false;
	auto adata = high->attackData.get();
	if (!adata)
		return false;
	auto state = a->actorState1.meleeAttackState;
	return state == RE::ATTACK_STATE_ENUM::kDraw || state == RE::ATTACK_STATE_ENUM::kSwing || state == RE::ATTACK_STATE_ENUM::kHit;
}

bool is_moving(RE::Actor* a)
{
	auto state = a->actorState1;
	return state.running || state.walking || state.sprinting;
}

float get_OffensiveBash_prop(RE::Actor* me, RE::Actor* he)
{
	if (has_manyStamina(me) && !is_staggered(he) && !is_attacking(he))
		return 1.0f;
	else
		return 0.0f;
}

float get_Block_prop(RE::Actor*, RE::Actor* he)
{
	if (is_attacking(he) && !is_powerattacking(he))
		return 1.0f;
	else
		return 0.0f;
}

float get_BashAfterAttack_prop(RE::Actor* me, RE::Actor*)
{
	if (has_manyStamina(me))
		return 1.0f;
	else
		return 0.0f;
}

float get_DefensiveBash_prop(RE::Actor* me, RE::Actor* he)
{
	if (has_enoughStamina(me) && is_powerattacking(he))
		return 1.0f;
	else
		return 0.0f;
}

bool should_interruptBlock(RE::Actor* a)
{
	return is_powerattacking(a);
}

bool should_interruptAttack(RE::Actor* a)
{
	return is_attacking(a);
}

float get_BlockType_prop(RE::Actor* me, RE::Actor*)
{
	if (!has_stamina(me))
		return 0.0f;

	return 1.0f;
}

bool is_AttackEnded(RE::Actor* a)
{
	auto state = a->actorState1.meleeAttackState;
	return state != RE::ATTACK_STATE_ENUM::kDraw && state != RE::ATTACK_STATE_ENUM::kSwing && state != RE::ATTACK_STATE_ENUM::kNone;
}

bool should_attack(RE::Actor*, RE::Actor* he)
{
	//return !is_attacking(he) || is_AttackEnded(he);
	return is_AttackEnded(he);
}

float get_Attack_prop(RE::Actor* me, RE::Actor* he)
{
	if (!has_stamina(me))
		return 0.0f;

	if (is_staggered(he) || is_AttackEnded(he))
		return 1.0f;

	if (is_attacking(he))
		return 0.0f;

	return 0.2f;
}

float get_SpecialAttack_prop(RE::Actor* me, RE::Actor* he)
{
	return get_Attack_prop(me, he);
}

float get_Dummy_prop(RE::Actor* , RE::Actor* )
{
	return 0.0f;
}
