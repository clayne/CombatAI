#include "UselessFenixUtils.h"
#include "DebugAPI.h"
#include <CombatAIController.h>
#include <chrono>
#include <thread>
#include <mutex>
#include "CombatBehaviorConditions.h"
#include "Settings.h"

#define TYPE_SPEED 1
#define TYPE_DODGE 2

#define TYPE_GHOST 1
#define TYPE_HITFRAME 2

#define TYPE_NORMAL 1
#define TYPE_FALSE 2
#define TYPE_TRUE 3

//#define CUSTOM_TYPES

#ifdef CUSTOM_TYPES
#	define DODGE_TYPE TYPE_SPEED
#	define FALLBACK_TYPE TYPE_FALSE
#	define CIRCLE_TYPE TYPE_TRUE
#	define ATTACK_TYPE TYPE_TRUE
#	define BLOCK_TYPE TYPE_FALSE
#	define DODGE_GHOST_TYPE TYPE_HITFRAME
#else
#	define DODGE_TYPE TYPE_DODGE
#	define FALLBACK_TYPE TYPE_NORMAL
#	define CIRCLE_TYPE TYPE_NORMAL
#	define ATTACK_TYPE TYPE_NORMAL
#	define BLOCK_TYPE TYPE_NORMAL
#	define DODGE_GHOST_TYPE TYPE_GHOST
#endif

#define DODGE_ID 1
#define DODGE_SPEED 1.647f

using PA = Utils::PolarAngle;

bool has_manyStamina(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kStamina);
	auto total = get_total_av(a, RE::ActorValue::kStamina);
	return total >= 100.0f && total * 0.5 <= cur;
}

bool has_enoughStamina(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kStamina);
	auto total = get_total_av(a, RE::ActorValue::kStamina);
	return total >= 25.0f && total * 0.25 <= cur;
}

bool is_powerattacking(RE::Actor* a)
{
	return _generic_foo<37639, bool, RE::Actor*>(a);
}

bool is_staying(RE::Actor* a) {
	return !a->actorState1.running && !a->actorState1.walking && !a->actorState1.sprinting;
}

bool has_stamina(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kStamina);
	auto total = get_total_av(a, RE::ActorValue::kStamina);
	return total <= 20.0f || cur >= 15.0f;
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

bool is_bashing(RE::Actor* a)
{
	if (a->actorState1.meleeAttackState == RE::ATTACK_STATE_ENUM::kNone)
		return false;

	auto adata = Utils::get_attackData(a);
	if (!adata)
		return false;

	return adata->data.flags.any(RE::AttackData::AttackFlag::kBashAttack);
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

void interruptattack(RE::Actor* me)
{
	me->NotifyAnimationGraph("attackStop");
	me->NotifyAnimationGraph("blockStop");
}

bool is_AttackEnded(RE::Actor* a)
{
	auto state = a->actorState1.meleeAttackState;
	return state != RE::ATTACK_STATE_ENUM::kDraw && state != RE::ATTACK_STATE_ENUM::kSwing && state != RE::ATTACK_STATE_ENUM::kNone;
}

bool has_hp(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kHealth);
	auto total = get_total_av(a, RE::ActorValue::kHealth);
	return total <= 20.0f || cur >= 15.0f;
}

bool has_enough_hp(RE::Actor* a)
{
	auto cur = a->GetActorValue(RE::ActorValue::kHealth);
	auto total = get_total_av(a, RE::ActorValue::kHealth);
	return total >= 25.0f && total * 0.25 <= cur;
}

float getav_pers(RE::Actor* a, RE::ActorValue av)
{
	auto cur = a->GetActorValue(av);
	auto total = get_total_av(a, av);
	if (total < 0.0001f)
		total = 0.0001f;
	if (cur < 0.0f)
		cur = 0.0f;
	return cur / total;
}

bool want_attack_light(RE::Actor* me, RE::Actor* he)
{
	if (!has_stamina(me))
		return false;

	if (is_staggered(he) || is_AttackEnded(he) || !has_enough_hp(he))
		return true;

	if (is_attacking(he))
		return false;

	if (!has_enough_hp(me))
		return false;

	auto offencive = (getav_pers(me, RE::ActorValue::kStamina) + 2 * getav_pers(me, RE::ActorValue::kHealth)) / 3.0f;
	return offencive >= 0.25f;
}

float get_Reach(RE::Actor* a)
{
	return _generic_foo<37588, float, RE::Actor*>(a);
}

float get_dist2(RE::Actor* a, RE::Actor* b)
{
	return _generic_foo<46058, float, RE::Actor*, RE::Actor*>(a, b);
}

float get_combat_reach(RE::Actor* a)
{
	return get_Reach(a) + 30.0f;
}

bool Character__is_moving_from(RE::Actor* a, RE::NiPoint3* p)
{
	return _generic_foo<46062, bool, RE::Actor*, RE::NiPoint3*>(a, p);  // SkyrimSE.exe+7BE7E0
}

void Actor__SetIsGhost_1405D25E0(RE::Actor* a, bool val) {
	return _generic_foo<36287, void, RE::Actor*, bool>(a, val);  // SkyrimSE.exe+5D25E0
}

namespace Movement
{
	using PA = Utils::PolarAngle;
	const float default_angle = 50.0f;

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

	namespace Dodging
	{
		template <float amount>
		void immune_start(RE::Actor* a)
		{
			if (*Settings::DodgingModIndex == -1) {
				damageav(a, RE::ACTOR_VALUE_MODIFIERS::kTemporary, RE::ActorValue::kSpeedMult, amount, nullptr);
				Actor__SetIsGhost_1405D25E0(a, true);
			}
			if (*Settings::DodgingModIndex == 0) {
				a->SetGraphVariableBool("bInIframe", true);
				a->SetGraphVariableBool("bIframeActive", true);
			}
			if (*Settings::DodgingModIndex == 1) {
				Actor__SetIsGhost_1405D25E0(a, true);
			}
		}

		template <float amount>
		void immune_end(RE::Actor* a)
		{
			if (*Settings::DodgingModIndex == -1) {
				damageav(a, RE::ACTOR_VALUE_MODIFIERS::kTemporary, RE::ActorValue::kSpeedMult, -amount, nullptr);
				Actor__SetIsGhost_1405D25E0(a, false);
			}
			if (*Settings::DodgingModIndex == 0) {
				a->SetGraphVariableBool("bInIframe", false);
				a->SetGraphVariableBool("bIframeActive", false);
			}
			if (*Settings::DodgingModIndex == 1) {
				Actor__SetIsGhost_1405D25E0(a, false);
			}
		}

		void dodge_circle(RE::Actor* me, CircleDirestions dir)
		{
			if (*Settings::DodgingModIndex == -1) {
				// himself
			}
			if (*Settings::DodgingModIndex == 0) {
				me->NotifyAnimationGraph(dir == CircleDirestions::Left ? "TKDodgeRight" : "TKDodgeLeft");
			}
			if (*Settings::DodgingModIndex == 1) {
				me->SetGraphVariableInt("DodgeID", DODGE_ID);
				me->SetGraphVariableFloat("DodgeSpeed", DODGE_SPEED);
				me->NotifyAnimationGraph("RollStart");
			}
		}

		void dodge_back(RE::Actor* a)
		{
			if (*Settings::DodgingModIndex == -1) {
				// himself
			}
			if (*Settings::DodgingModIndex == 0) {
				a->NotifyAnimationGraph("TKDodgeBack");
			}
			if (*Settings::DodgingModIndex == 1) {
				a->SetGraphVariableInt("DodgeID", DODGE_ID);
				a->SetGraphVariableFloat("DodgeSpeed", DODGE_SPEED);
				a->NotifyAnimationGraph("RollStart");
			}
		}
	}

	static std::set<uint32_t> buffed = std::set<uint32_t>();
	std::mutex buffed_mutex;

	template <int time = 100, float amount = 500.0f>
	void buff_speed(RE::Actor* a)
	{
		std::thread t([a]() {
			buffed_mutex.lock();
			if (buffed.count(a->formID)) {
				buffed_mutex.unlock();
				return;
			}
			buffed.insert(a->formID);

			Dodging::immune_start<amount>(a);

			buffed_mutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
			buffed_mutex.lock();

			Dodging::immune_end<amount>(a);

			buffed.erase(a->formID);
			buffed_mutex.unlock();
		});
		t.detach();
	}
	
	RE::NiPoint3 rotateZ(float r, const RE::NiPoint3& rotation)
	{
		RE::NiPoint3 ans;

		float gamma = -rotation.z + 3.1415926f / 2;
		float cos_g = cos(gamma);
		float sin_g = sin(gamma);

		ans.x = r * cos_g;
		ans.y = r * sin_g;
		ans.z = 0.0f;

		return ans;
	}

	template <glm::vec4 color = RED>
	void draw_ray(RE::Actor* attacker, float r, PA a)
	{
		RE::NiPoint3 hit_pos, rotation;
		hit_pos = attacker->data.location;
		rotation = attacker->data.angle;
		PA z = rotation.z * 180.0f / PI;
		z = z.add(a);
		rotation.z = z * PI / 180.0f;
		hit_pos += rotateZ(r, rotation);
		draw_line<color>(attacker->data.location, hit_pos);
	}

	template <glm::vec4 color = RED>
	void draw_ray_point(RE::Actor* attacker, float r, PA a)
	{
		RE::NiPoint3 hit_pos, rotation;
		hit_pos = attacker->data.location;
		rotation = attacker->data.angle;
		PA z = rotation.z * 180.0f / PI;
		z = z.add(a);
		rotation.z = z * PI / 180.0f;
		hit_pos += rotateZ(r, rotation);
		draw_line<color>(hit_pos + RE::NiPoint3{0.0f, 0.0f, 1.0f}, hit_pos);
	}

	template <glm::vec4 color = RED>
	void draw_attack(RE::Actor* attacker, float r, float angle)
	{
		draw_ray<color>(attacker, r, 0);
		draw_ray<color>(attacker, r, angle);
		draw_ray<color>(attacker, r, 360.0f - angle);

		draw_circleZ<color>(attacker->data.location, r);
	}

	bool isInDanger(RE::Actor* me, AttackInfo* info = nullptr)
	{
		auto he = me->currentCombatTarget.get().get();
		if (!he)
			return false;

		auto R = get_combat_reach(he);
		auto r2 = get_dist2(me, he);

		RE::BGSAttackData* attackdata = Utils::get_attackData(he);
		auto angle = get_angle_he_me(me, he, attackdata);

		float attackAngle = attackdata ? attackdata->data.strikeAngle : 50.0f;

		if (info) {
			info->R = R;
			info->r = sqrt(r2);
			info->reflected = angle < 0.0f;
			info->me = abs(angle);
			info->attackAngle = attackAngle;
		}

		if (is_blocking(he) || !is_attacking(he))
			return false;

		auto attackState = he->GetAttackState();
		if (attackState != RE::ATTACK_STATE_ENUM::kSwing && attackState != RE::ATTACK_STATE_ENUM::kDraw)
			return false;

		if (abs(angle) > attackAngle)
			return false;

		if (r2 > R * R && (!is_powerattacking(he) || r2 > 500.0f * 500.0f))
			return false;

#ifdef DEBUG
		//draw_attack(he, R, attackAngle);
		//draw_line(me->data.location, he->data.location);
#endif  // DEBUG

		return true;
	}
	
	namespace Circle
	{
		static float _get_circle_angle(float attackAngle, float angle, bool notreflected)
		{
			angle = fmaxf(attackAngle + angle + attackAngle * 0.2f, 20.0f);
			if (notreflected)
				angle = -angle;
			return angle;
		}

		void rotate(const RE::NiPoint3& me, const RE::NiPoint3& he, RE::NiPoint3& new_pos, float angle)
		{
			auto he_me = me - he;
			auto angle_sin = sin(angle);
			auto angle_cos = cos(angle);
			new_pos.x = he_me.x * angle_cos - he_me.y * angle_sin + he.x;
			new_pos.y = he_me.y * angle_cos + he_me.x * angle_sin + he.y;
			new_pos.z = he_me.z + he.z;
		}

		bool check_angle(RE::Actor* me, RE::Actor* he, const AttackInfo& info, float me_angle)
		{
			auto angle = _get_circle_angle(info.attackAngle, me_angle, !info.reflected) / 180.0f * PI;

			RE::NiPoint3 new_pos;
			rotate(me->GetPosition(), he->GetPosition(), new_pos, angle);

			bool ans = check_collisions(me, &me->data.location, &new_pos);

#ifdef DEBUG
			//draw_line<GRN, 1000>(me->GetPosition(), new_pos);
#endif  // DEBUG

			return ans;
		}

		float get_CircleAngle(const AttackInfo& info, CircleDirestions dir)
		{
			auto me_angle = info.me;
			if (info.reflected == (dir == CircleDirestions::Left))
				me_angle = -info.me;
			else
				me_angle = info.me;
			return _get_circle_angle(info.attackAngle, me_angle, dir == CircleDirestions::Right);
		}

		CircleDirestions choose_moving_direction_circle(const AttackInfo* const info, RE::Actor* a)
		{
			auto he = a->currentCombatTarget.get().get();
			if (!he)
				return CircleDirestions::None;

			const float DIST_BORDER = 100.0f;

			const float r = info->r;
			const float me = info->me;

			if (PA::dist(r, info->attackAngle - me) <= DIST_BORDER && check_angle(a, he, *info, -me)) {
				return info->reflected ? CircleDirestions::Left : CircleDirestions::Right;
			} else if (PA::dist(r, info->attackAngle + me) <= DIST_BORDER && check_angle(a, he, *info, me)) {
				return info->reflected ? CircleDirestions::Right : CircleDirestions::Left;
			}

			return CircleDirestions::None;
		}

		float get_CircleAngle([[maybe_unused]] RE::Character* me)
		{
#if CIRCLE_TYPE == TYPE_FALSE
			return 0.0f;
#endif
#if CIRCLE_TYPE == TYPE_TRUE
			return 90.0f;
#endif
#if CIRCLE_TYPE == TYPE_NORMAL
			AttackInfo info;
			if (!isInDanger(me, &info)) {
				bool left;
				if (is_blocking(CombatAI__get_he())) {
					left = info.reflected;
				} else
					left = rand() % 2 == 0;
				return left ? default_angle : -default_angle;
			}

			auto dir = choose_moving_direction_circle(&info, me);
			if (dir != CircleDirestions::Left && dir != CircleDirestions::Right)
				return default_angle;

			return get_CircleAngle(info, dir);
#endif
		}

		template <bool change = false>
		uint32_t should_danger_alwaysDanger(RE::Character* me, RE::Actor*, const AttackInfo& info)
		{
			auto dir = choose_moving_direction_circle(&info, me);
			if (dir == CircleDirestions::Left || dir == CircleDirestions::Right) {
				if (change) {
					interruptattack(me);
					buff_speed<300>(me);
					Dodging::dodge_circle(me, dir);
#ifdef DEBUG
					draw_line<BLU, 1000>(me->GetPosition(), 3);
#endif  // DEBUG

				}

				return true;
			}

			return false;
		}

		uint32_t should_danger([[maybe_unused]] RE::Character* me)
		{
#if CIRCLE_TYPE == TYPE_FALSE
			return false;
#endif
#if CIRCLE_TYPE == TYPE_TRUE
			if (isInDanger(me)) {
				interruptattack(me);
				buff_speed<300>(me);
			}
			return true;
#endif
#if CIRCLE_TYPE == TYPE_NORMAL
			if (is_powerattacking(me) && !is_juststarted_attacking(me))
				return false;

			if (is_bashing(me))
				return false;

			auto he = me->currentCombatTarget.get().get();
			if (!he)
				return false;

			AttackInfo info;
			if (!isInDanger(me, &info))
				return false;

			return should_danger_alwaysDanger<true>(me, he, info);
#endif
		}

		uint32_t should_normal([[maybe_unused]] RE::Character* me)
		{
#if CIRCLE_TYPE == TYPE_FALSE
			return false;
#endif
#if CIRCLE_TYPE == TYPE_TRUE
			return true;
#endif
#if CIRCLE_TYPE == TYPE_NORMAL
			auto he = me->currentCombatTarget.get().get();
			if (!he)
				return false;

			bool isblocking = is_blocking(he);

			if (isblocking && abs(get_angle_he_me(me, he, Utils::get_attackData(he))) < 60.0f)
				return true;

			if (!isblocking && rand() % 100 > 70)
				return true;

			return false;
#endif
		}
	}

	namespace Fallback
	{
		float get_FallbackWaitTime(RE::Character*)
		{
			return 0.0f;
		}

		float get_FallbackDistance(const AttackInfo& info)
		{
			// info.r subs after
			return fmaxf(info.R - 137.76f, 60.0f);
		}
	
		float get_FallbackDistance(RE::Character* me)
		{
			AttackInfo info;
			if (!isInDanger(me, &info))
				return 80.0f;

			return get_FallbackDistance(info);
		}

		float get_FallbackSpeed(RE::Character*)
		{
			return 2.0f;
		}

		template <bool change = false>
		uint32_t should_alwaysDanger(RE::Character* a, RE::Actor* he, const AttackInfo& info)
		{
			const float DIST_BORDER = 60.0f;

			const float r = info.r;
			const float R = info.R;
			const PA me = info.me;
			float back_dist = R - r;

#ifdef DEBUG
			//draw_attack<GRN>(he, R - DIST_BORDER, 50.0f);
#endif  // DEBUG

			if (!is_powerattacking(he) && back_dist <= DIST_BORDER) {
				RE::NiPoint3 he_me = a->GetPosition() - he->GetPosition(), new_pos;
				auto he_me_len = he_me.Unitize();
				auto walk_distance = get_FallbackDistance(info) - he_me_len;
				new_pos = he_me * walk_distance + a->GetPosition();

				if (check_collisions(a, &a->data.location, &new_pos)) {
					if (change) {
#ifdef DEBUG
						draw_line<ORA, 2000>(a->GetPosition(), 2);
						draw_line<GRN, 2000>(a->GetPosition(), new_pos);
#endif  // DEBUG
						interruptattack(a);
						buff_speed<200>(a);
						Dodging::dodge_back(a);
					}
					return true;
				} else {
#ifdef DEBUG
					//draw_line<RED, 2000>(a->GetPosition(), new_pos);
#endif  // DEBUG
				}
			}

			return false;
		}

		uint32_t should([[maybe_unused]] RE::Character* me)
		{
#if FALLBACK_TYPE == TYPE_FALSE
			return false;
#endif
#if FALLBACK_TYPE == TYPE_TRUE
			return true;
#endif
#if FALLBACK_TYPE == TYPE_NORMAL
			if (is_powerattacking(me) && !is_juststarted_attacking(me))
				return false;

			if (is_bashing(me))
				return false;

			AttackInfo info;
			if (!isInDanger(me, &info))
				return false;

			auto he = me->currentCombatTarget.get().get();
			if (!he)
				return false;

			return should_alwaysDanger<true>(me, he, info);
#endif
		}

		uint32_t should_normal([[maybe_unused]] RE::Character* me)
		{
#if FALLBACK_TYPE == TYPE_FALSE
			return false;
#endif
#if FALLBACK_TYPE == TYPE_TRUE
			return true;
#endif
#if FALLBACK_TYPE == TYPE_NORMAL
			auto state = me->actorState1.meleeAttackState;
			return state == RE::ATTACK_STATE_ENUM::kHit || state == RE::ATTACK_STATE_ENUM::kNextAttack || state == RE::ATTACK_STATE_ENUM::kSwing;
#endif
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

		void fix_radiuses(RE::Actor* me, RE::Actor* he, float* inner_R, float* outer_R)
		{
			if (!want_attack_light(me, he)) {
				*inner_R = (*inner_R + 50.0f) * 2.0f;
				*outer_R = (*outer_R + 50.0f) * 2.0f;
			} else {
				//*inner_R = fmaxf(*inner_R, 50.0f);
				//*outer_R = fmaxf(*outer_R, 200.0f);
			}
		}

		bool sub_1407D62A0(void* a1)
		{
			return _generic_foo<46699, bool, void*>(a1);  // SkyrimSE.exe+7D62A0
		}

		bool should_fallback_to_ranged(void* a1)
		{
			return sub_1407D62A0(a1) && get_dist2(CombatAI__get_he(), CombatAI__get_me()) > 500.0f * 500.0f;
		}
	}

	bool CombatBehaviorContextCloseMovement__is_outofrange(void* context)
	{
		return _generic_foo<46696, bool, void*>(context);  // SkyrimSE.exe+7D5C80
	}

	bool is_outofrange(void* context) {
		if (!CombatBehaviorContextCloseMovement__is_outofrange(context))
			return false;

		return has_enoughStamina(CombatAI__get_me()) && get_dist2(CombatAI__get_me(), CombatAI__get_he()) > 200.0f * 200.0f;
	}

	uint32_t low_stamina(RE::Character* me)
	{
		auto ans = !has_enoughStamina(me);

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
			draw_line<RED, 2000>(me->GetPosition(), 1);
		else
			draw_line<GRN, 2000>(me->GetPosition(), 1);
#endif  // DEBUG


		return ans;
	}

	namespace Surround
	{
		uint32_t should(RE::Actor*)
		{
			return true;
		}

		bool should_interrupt(RE::Actor* a)
		{
			return isInDanger(a);
		}

		void hooked_interrupting(char** context)
		{
			if (should_interrupt(CombatAI__get_me())) {
				char* path = *context;
				*(path + 0x14) = 5;
			}
		}
	}
}

namespace Attack
{
	uint32_t dontwant([[maybe_unused]] RE::Character* me)
	{
#if ATTACK_TYPE == TYPE_FALSE
		return true;
#endif
#if ATTACK_TYPE == TYPE_TRUE
		return false;
#endif
#if ATTACK_TYPE == TYPE_NORMAL
		auto he = CombatAI__get_he();

		if (!has_stamina(me))
			return true;

		if (is_staggered(he) || is_AttackEnded(he))
			return false;

		if (is_blocking(he) && abs(get_angle_he_me(me, he, Utils::get_attackData(he))) < 30.0f)
			return true;

		if (Movement::is_indanger(me))
			return true;

		if (!has_enoughStamina(me) && has_enough_hp(he))
			return true;

		if (Character__is_moving_from(he, &me->data.location)) {
			auto dist = get_dist2(me, he);
			if (dist > 100.0f * 100.0f)
				return true;
		}

		return false;
#endif
	}

	float get_offensive(RE::Actor* a) {
		return a->GetActorValue(RE::ActorValue::kStamina) + a->GetActorValue(RE::ActorValue::kHealth);
	}

	float get_thisattack_chance(RE::Actor* me, RE::Actor* he, RE::BGSAttackData* adata)
	{
		float ans = adata->data.attackChance;
		if (adata->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
			if (get_offensive(me) < get_offensive(he))
				ans = 0.0f;

			if (is_staggered(he))
				ans = 0.0f;

			if (!has_manyStamina(me))
				ans = 0.0f;

		} else {
			if (is_blocking(he) && abs(get_angle_he_me(me, he, Utils::get_attackData(he))) < 60.0f)
				ans = 0.0f;
		}

		if (adata->data.flags.any(RE::AttackData::AttackFlag::kBashAttack)) {
			if (is_staggered(he))
				ans = 0.0f;
		}

		return ans;
	}

	bool should_interrupt() {
		auto me = CombatAI__get_me();

		if (Movement::is_indanger(me) && (!is_powerattacking(me) || is_staying(me))) {
			interruptattack(me);
			return true;
		}

		return false;
	}
}

namespace Block
{
	uint32_t wantbash([[maybe_unused]] RE::Character* me)
	{
#if BLOCK_TYPE == TYPE_FALSE
		return false;
#endif
#if BLOCK_TYPE == TYPE_TRUE
		return true;
#endif
#if BLOCK_TYPE == TYPE_NORMAL
		auto he = CombatAI__get_he();
		
		if (is_blocking(he) && abs(get_angle_he_me(me, he, Utils::get_attackData(he))) < 60.0f)
			return false;

		if (Character__is_moving_from(he, &me->data.location)) {
			auto dist = get_dist2(me, he);
			if (dist > 100.0f * 100.0f)
				return false;
		}

		return (has_enoughStamina(me) && is_powerattacking(he)) || (has_manyStamina(me) && !is_attacking(he));
#endif
	}

	uint32_t wantblock([[maybe_unused]] RE::Character* me)
	{
#if BLOCK_TYPE == TYPE_FALSE
		return false;
#endif
#if BLOCK_TYPE == TYPE_TRUE
		return true;
#endif
#if BLOCK_TYPE == TYPE_NORMAL
		if (!has_stamina(me))
			return false;

		auto he = me->currentCombatTarget.get().get();
		if (!he)
			return false;

		Movement::AttackInfo info;
		if (!Movement::isInDanger(me, &info))
			return false;

		if (Movement::Circle::should_danger_alwaysDanger(me, he, info) || Movement::Fallback::should_alwaysDanger(me, he, info))
			return false;
		
		return true;
#endif
	}

	float get_BashAfterBlock_prop(RE::Actor* me, RE::Actor*)
	{
		if (has_manyStamina(me))
			return 1.0f;
		else
			return 0.0f;
	}
}
