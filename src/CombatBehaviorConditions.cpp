#include "UselessFenixUtils.h"
#include "DebugAPI.h"
#include <CombatAIController.h>

using PA = Utils::PolarAngle;

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
	return total >= 25.0f && total * 0.25 <= cur;
}

bool is_powerattacking(RE::Actor* a)
{
	return _generic_foo<37639, bool, RE::Actor*>(a);
}

bool has_stamina(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kStamina);
	auto total = a->GetBaseActorValue(RE::ActorValue::kStamina);
	return total <= 20.0f || cur >= 15.0f;
}

float get_stamina_pers(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kStamina);
	auto total = a->GetBaseActorValue(RE::ActorValue::kStamina);
	if (total <= 0.00001f)
		return 1.0f;
	else
		return cur / total;
}

bool is_staggered(RE::Actor* a)
{
	return _generic_foo<46064, bool, RE::Actor*>(a);
}

bool check_collisions(RE::Actor* a, RE::NiPoint3* a_pos, RE::NiPoint3* a_new_pos)
{
	return _generic_foo<46050, bool, RE::Actor*, RE::NiPoint3*, RE::NiPoint3*, float, float>(a, a_pos, a_new_pos, 2.0f, 64.0f);
}

bool is_blocking(RE::Actor* a)
{
	return _generic_foo<36927, bool, RE::Actor*>(a);
}

bool is_attacking(RE::Actor* a)
{
	auto adata = Utils::get_attackData(a);
	if (!adata)
		return false;
	auto state = a->actorState1.meleeAttackState;
	return state == RE::ATTACK_STATE_ENUM::kDraw || state == RE::ATTACK_STATE_ENUM::kSwing || state == RE::ATTACK_STATE_ENUM::kHit;
}

bool is_juststarted_attacking(RE::Actor* a)
{
	auto adata = Utils::get_attackData(a);
	if (!adata)
		return false;
	auto state = a->actorState1.meleeAttackState;
	return state == RE::ATTACK_STATE_ENUM::kDraw;
}

bool is_moving(RE::Actor* a)
{
	auto state = a->actorState1;
	return state.running || state.walking || state.sprinting;
}

static float get_angle_he_me(RE::Actor* me, RE::Actor* he, RE::BGSAttackData* attackdata)
{
	auto he_me = PA(me->GetPosition() - he->GetPosition());
	auto head = PA(he->GetHeading(false) * 180.0f / PI);
	if (attackdata)
		head = head.add(attackdata->data.attackAngle);
	auto angle = he_me.sub(head).to_normangle();
	return angle;
}

float get_OffensiveBash_prop(RE::Actor* me, RE::Actor* he)
{
	if (is_blocking(he) && abs(get_angle_he_me(me, he, Utils::get_attackData(he))) < 60.0f)
		return 0.1f;

	if (has_manyStamina(me) && !is_staggered(he) && !is_attacking(he))
		return 1.0f;

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
	if (has_enoughStamina(me) && is_powerattacking(he) && !is_staggered(he))
		return 1.0f;
	else
		return 0.0f;
}

bool should_interruptBlock(RE::Actor* a)
{
	return is_powerattacking(a);
}

static void interruptattack(RE::Actor* me)
{
	//if (is_juststarted_attacking(me) && !is_blocking(me) && !is_powerattacking(me))
		me->NotifyAnimationGraph("attackStop");
}

bool should_interruptAttack(RE::Actor* me)
{
	auto he = me->currentCombatTarget.get().get();
	if (!he)
		return false;

	if (is_juststarted_attacking(he)) {
		interruptattack(me);
		return true;
	}

	return false;
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

static float angleKernel(float x) {
	const float M = 1.0f / 3.0f;
	if (x < M) {
		return 0.8f * 9.0f * x * x + 0.1f;
	} else {
		auto k = (x - M) / (1.0f - M);
		if (k > 1.0)
			k = 1.0f;
		return (1.0f - 0.9f) * k + 0.9f;
	}
}

float get_Attack_prop(RE::Actor* me, RE::Actor* he)
{
	if (!has_stamina(me))
		return 0.0f;

	if (is_staggered(he) || is_AttackEnded(he))
		return 1.0f;

	if (is_attacking(he))
		return 0.0f;

	auto angle = abs(get_angle_he_me(me, he, Utils::get_attackData(he)));

	if (is_blocking(he) && angle < 60.0f)
		return 0.1f;

	auto angle_factor = angleKernel(angle / 180.0f);

	auto pers = get_stamina_pers(me);
	auto stamina_factor = pers * pers;

	return stamina_factor * angle_factor;
}

float get_SpecialAttack_prop(RE::Actor* me, RE::Actor* he)
{
	return get_Attack_prop(me, he);
}

float get_Dummy_prop(RE::Actor* , RE::Actor* )
{
	return 0.0f;
}

float get_Reach(RE::Actor* a)
{
	return _generic_foo<37588, float, RE::Actor*>(a);
}

float get_dist(RE::Actor* a, RE::Actor* b)
{
	return _generic_foo<46058, float, RE::Actor*, RE::Actor*>(a, b);
}

namespace Movement
{
	using PA = Utils::PolarAngle;
	const float default_angle = 90.0f;

	enum class CircleDirestions
	{
		None,
		Left,
		Right,
		Back
	};

	struct AttackInfo
	{
		float R;
		float attackAngle;
		float r;
		PA me;
		bool reflected;
	};

	bool isInDanger(RE::Actor* me, AttackInfo* info = nullptr)
	{
		auto he = me->currentCombatTarget.get().get();
		if (!he)
			return false;

		auto R = get_Reach(he) * 1.5f + 28.0f;
		auto r = get_dist(me, he);

		RE::BGSAttackData* attackdata = Utils::get_attackData(he);
		auto angle = get_angle_he_me(me, he, attackdata);

		float attackAngle = attackdata ? attackdata->data.strikeAngle : 0.0f;

		auto attackState = he->GetAttackState();
		if (attackState != RE::ATTACK_STATE_ENUM::kSwing && attackState != RE::ATTACK_STATE_ENUM::kDraw)
			return false;

		if (abs(angle) > attackAngle && !is_powerattacking(he))
			return false;

		if (r > R * R && !is_powerattacking(he))
			return false;

		if (info) {
			info->R = R;
			info->r = sqrt(r);
			info->reflected = angle < 0.0f;
			info->me = abs(angle);
			info->attackAngle = attackAngle;
		}

		return true;
	}

	static float _get_circle_angle(float attackAngle, float me, bool notreflected)
	{
		float angle = attackAngle - me;
		if (notreflected)
			angle = -angle;
		return angle * 1.2f;
	}

	static CircleDirestions chose_moving_direction(const AttackInfo* const info, RE::Actor* a, bool isindanger)
	{
		if (!is_AttackEnded(a) && is_attacking(a) && !is_juststarted_attacking(a))
			return CircleDirestions::None;

		auto he = a->currentCombatTarget.get().get();
		if (!he)
			return CircleDirestions::None;

		const float BACK_SPEED_MULT = 1.0f;
		const float CIRCLE_SPEED_MULT = 1.0f;
		const float DIST_BORDER = 100.0f;

		const float r = info->r;
		const float R = info->R;
		const PA me = info->me;
		float back_dist = (R - r) * BACK_SPEED_MULT;
		float circle_dist = PA::dist(r, me, info->attackAngle) * CIRCLE_SPEED_MULT;

		RE::NiPoint3 he_me = a->GetPosition() - he->GetPosition(), new_pos;
		auto he_me_len = he_me.Unitize();

		if (!is_powerattacking(he) &&
			(isindanger && (back_dist < circle_dist && back_dist <= DIST_BORDER) ||
				!isindanger && (!has_stamina(a) && (info->me > 90.0f || info->r > 300.0f)))) {
			auto walk_distance = (isindanger ? info->R * 1.5f : 500.0f) + 120.0f - he_me_len;
			new_pos = he_me * walk_distance + a->GetPosition();
			if (check_collisions(a, &a->data.location, &new_pos)) {
				return CircleDirestions::Back;
			}
		}

		if (info->r < 120.0f ||
			(isindanger || is_powerattacking(he)) && circle_dist <= DIST_BORDER ||
			isindanger && !has_stamina(a) ||
			!isindanger && info->me < 90.0f && has_stamina(a) && is_blocking(he)) {
			auto angle = _get_circle_angle(isindanger ? default_angle : info->attackAngle, info->me, !info->reflected) * 180.0f / PI;
			auto angle_sin = sin(angle);
			auto angle_cos = cos(angle);
			he_me *= fmaxf(120.0f, he_me_len);
			new_pos.x = he_me.x * angle_cos - he_me.y * angle_sin + he->GetPositionX();
			new_pos.y = he_me.y * angle_cos + he_me.x * angle_sin + he->GetPositionY();
			new_pos.z = he_me.z + he->GetPositionZ();

			if (check_collisions(a, &a->data.location, &new_pos)) {
				return info->reflected ? CircleDirestions::Left : CircleDirestions::Right;
			}
		}

		return CircleDirestions::None;
	}

	namespace Circle
	{
		float get_CircleAngle(RE::Character* me)
		{
			AttackInfo info;
			if (!isInDanger(me, &info))
				return rand() % 2 == 0 ? default_angle : -default_angle;

			auto dir = chose_moving_direction(&info, me, true);
			if (dir != CircleDirestions::Left && dir != CircleDirestions::Right)
				return default_angle;

			return _get_circle_angle(info.attackAngle, info.me, dir == CircleDirestions::Right);
		}

		uint32_t should_Circle(RE::Character* me)
		{
			AttackInfo info;

			auto indanger = isInDanger(me, &info);

			auto dir = chose_moving_direction(&info, me, indanger);
			if (dir == CircleDirestions::Left || dir == CircleDirestions::Right) {
				interruptattack(me);
				return 1;
			}

			return 0;
		}
	}

	namespace Fallback
	{
		float get_FallbackDistance(RE::Character* me)
		{
			AttackInfo info;
			if (!isInDanger(me, &info))
				return 500.0f;

			return info.R * 1.5f;
		}

		float get_FallbackSpeed(RE::Character*)
		{
			return 2.0f;
		}

		uint32_t should_Fallback(RE::Character* me) {
			AttackInfo info;
			auto indanger = isInDanger(me, &info);
			if (chose_moving_direction(&info, me, indanger) == CircleDirestions::Back) {
				interruptattack(me);
				return 1;
			}

			return 0;
		}
	}

	namespace Advance
	{
		uint32_t should(RE::Character*)
		{
			return false;
		}

		bool should_interrupt(RE::Character* me)
		{
			return isInDanger(me);
		}

		void hooked_interrupting(char** context)
		{
			if (should_interrupt(CombatAI__get_me())) {
				char* path = *context;
				*(path + 0x14) = 5;
			}
		}
	}

	bool CombatBehaviorContextCloseMovement__is_outofrange(void* context)
	{
		return _generic_foo<46696, bool, void*>(context);  // SkyrimSE.exe+7D5C80
	}

	bool is_outofrange(void* context) {
		if (!CombatBehaviorContextCloseMovement__is_outofrange(context))
			return false;

		return has_enoughStamina(CombatAI__get_me());
	}

	uint32_t low_stamina(RE::Character* me)
	{
		auto ans = !has_stamina(me);

#ifdef DEBUG
		if (ans)
			draw_line<GRN, 1000>(me->GetPosition(), 3);
#endif  // DEBUG

		return ans;
	}

	uint32_t is_indanger(RE::Character* me)
	{
		auto ans = isInDanger(me);

#ifdef DEBUG
		if (ans)
			draw_line<RED, 1000>(me->GetPosition(), 1);
#endif  // DEBUG


		return ans;
	}

	uint32_t is_wantattack(RE::Character* me)
	{
		auto ans = has_enoughStamina(me);

#ifdef DEBUG
		if (ans)
			draw_line<YEL, 1000>(me->GetPosition(), 2);
#endif  // DEBUG

		return ans;
	}

	uint32_t should_surround(RE::Character*)
	{
		return true;
	}
}

float get_Block_prop(RE::Actor* me, RE::Actor* )
{
	if (!has_stamina(me))
		return 0.0f;

	Movement::AttackInfo info;
	if (!isInDanger(me, &info))
		return 0.0f;

	if (chose_moving_direction(&info, me, true) == Movement::CircleDirestions::None)
		return 1.0f;
	
	return 0.0f;
}
