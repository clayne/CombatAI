#include "UselessFenixUtils.h"
#include "CombatBehaviorTrees.h"
#include "CombatBehaviorConditions.h"
#include "Hooks.h"

template <int ID, int offset = 0x0>
void writebytes(const std::string_view& data)
{
	REL::safe_write(REL::ID(ID).address() + offset, data.data(), data.size());
}

void MainUpdateHook::Update(RE::Main* a_this, float a2)
{
	_Update(a_this, a2);

	DebugAPI::Update();
}

void AdvanceHook::update(char** context)
{
	Movement::Advance::hooked_interrupting(context);

	_update(context);
}

void AdvanceHook::set_two_radiuses(RE::Actor* me, RE::Actor* he, float* inner_R, float* outer_R)
{
	_set_two_radiuses(me, he, inner_R, outer_R);

	Movement::Advance::fix_radiuses(me, he, inner_R, outer_R);
}

void SurroundHook::update(char** context)
{
	Movement::Surround::hooked_interrupting(context);

	_update(context);
}

void apply_Movement()
{
	// CloseMovement_ctor: SkyrimSE.exe+7D97D0
	apply_func<46731>(uintptr_t(CombatBehaviorTreeCloseMovement__ctor));

	using namespace Movement;

	// Fallback
	{
		// get_FallbackDistance: SkyrimSE.exe+844FE0
		apply_func<49718>(std::uintptr_t(Fallback::get_FallbackDistance));

		// get_FallbackWaitTime: SkyrimSE.exe+7D782A
		SKSE::GetTrampoline().write_call<5>(REL::ID(46713).address() + 0xea, std::uintptr_t(Fallback::get_FallbackWaitTime));

		// call Character__get_FallbackSpeed: SkyrimSE.exe+7D7639
		apply_call<46712, 0x269>(std::uintptr_t(Fallback::get_FallbackSpeed));

		{  // SkyrimSE.exe+7D7654: set max speed
			constexpr REL::ID funcOffset(46712);
			const char data[] = "\x41\xB9\x04\x00\x00\x00";  // mov     r9d, 4
			REL::safe_write(funcOffset.address() + 0x284, data, 6);
		}
		{  // SkyrimSE.exe+7D85AD: set max speed
			constexpr REL::ID funcOffset(46720);
			const char data[] = "\xBA\x04\x00\x00\x00";  // mov     edx, 4
			REL::safe_write(funcOffset.address() + 0x3AD, data, 5);
		}

		// not check headings
		// SkyrimSE.exe+7D74FC: jmp SkyrimSE.exe+7D7518
		writebytes<46712, 0x12c>("\xeb\x1a");
		// SkyrimSE.exe+7D753C: jmp SkyrimSE.exe+7D7558
		writebytes<46712, 0x16c>("\xeb\x1a");

		// not check is_moving_from
		// SkyrimSE.exe+7D7518: jmp SkyrimSE.exe+7D752C
		writebytes<46712, 0x148>("\xeb\x12");

		// not check sub_1405E3250
		// SkyrimSE.exe+7D752C: jmp SkyrimSE.exe+7D753C
		writebytes<46712, 0x15c>("\xeb\x08");
	}

	// Circle
	{
		// call Character__get_CircleAngle: SkyrimSE.exe+7D8499
		apply_call<46720, 0x299>(std::uintptr_t(Circle::get_CircleAngle));

		// SkyrimSE.exe+7D84E9
		// do not circle_angle = -circle_angle
		// nop * 4
		writebytes<46720, 0x2E9>("\x0F\x1F\x40\x00");

		// SkyrimSE.exe+7D876E
		// 1 iteration on loop
		// cmp     r14d, 1
		writebytes<46720, 0x56E>("\x41\x83\xFE\x01");

		// SkyrimSE.exe+7D85AD
		// set max speed1
		// mov edx, 4
		writebytes<46720, 0x3AD>("\xBA\x04\x00\x00\x00");

		// SkyrimSE.exe+7D877D
		// set max speed2
		// mov r9d, 4
		writebytes<46720, 0x57D>("\x41\xB9\x04\x00\x00\x00");

		// SkyrimSE.exe+7D84AA
		// not check fCombatCircleAngleMax
		// nop * 6
		writebytes<46720, 0x2AA>("\x66\x0F\x1F\x44\x00\x00");

		// SkyrimSE.exe+7D8566
		// not check heading1
		// nop * (7 + 6)
		writebytes<46720, 0x366>("\x0F\x1F\x80\x00\x00\x00\x00");
		writebytes<46720, 0x36D>("\x66\x0F\x1F\x44\x00\x00");

		// SkyrimSE.exe+7D8376
		// not check sub_1405E3250
		// nop * 8
		writebytes<46720, 0x176>("\x0F\x1F\x84\x00\x00\x00\x00\x00");
		writebytes<46720, 0x182>("\x0F\x1F\x84\x00\x00\x00\x00\x00");

		{
			// smaller circle min radius
			
			// SkyrimSE.exe+7D83E8
			uintptr_t ret_addr = REL::ID(46720).address() + 0x1e8;

			struct Code : Xbyak::CodeGenerator
			{
				Code(uintptr_t ret_addr)
				{
					movss(xmm2, dword[rcx + 0x1A0]);
					mov(eax, 0x3e99999a);  // 0.3
					movd(xmm3, eax);
					mulss(xmm2, xmm3);
					
					mov(rax, ret_addr);
					jmp(rax);
				}
			} xbyakCode{ ret_addr };
			add_trampoline<5, 46720, 0x1e0>(&xbyakCode);  // SkyrimSE.exe+7D83E0
		}
	}

	AdvanceHook::Hook();
	SurroundHook::Hook();
}

void apply_Attacks() {
	// SkyrimSE.exe+80DBEC
	uintptr_t ret_addr_nofail = REL::ID(48148).address() + 0xec;

	// SkyrimSE.exe+80DBBB
	uintptr_t ret_addr_fail = REL::ID(48148).address() + 0xbb;

	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t ret_addr_fail, uintptr_t ret_addr_nofail, uintptr_t should_interrupt)
		{
			Xbyak::Label L__fail;

			test(eax, 0x0FFFFFFFB);
			jz(L__fail);

			sub(rsp, 0x20);
			mov(rax, should_interrupt);
			call(rax);
			add(rsp, 0x20);
			test(al, al);
			jne(L__fail);

			mov(rax, ret_addr_nofail);
			jmp(rax);

			L(L__fail);
			mov(rax, ret_addr_fail);
			jmp(rax);
		}
	} xbyakCode{ ret_addr_fail, ret_addr_nofail, uintptr_t(Attack::should_interrupt) };
	add_trampoline<5, 48148, 0xb4>(&xbyakCode);  // SkyrimSE.exe+80DBB4
}

void apply_hooks()
{
	SKSE::AllocTrampoline(1 << 10);

	MainUpdateHook::Hook();

	// Melee_ctor: SkyrimSE.exe+80DF60
	apply_func<48151>(uintptr_t(CombatBehaviorTreeMelee__ctor));

	// Block_ctor: SkyrimSE.exe+7d4620
	apply_func<46640>(uintptr_t(CombatBehaviorTreeBlock__ctor));

	// chose attacks & blocks
	SKSE::GetTrampoline().write_call<5>(REL::ID(48139).address() + 0x2ae, std::uintptr_t(Attack::get_thisattack_chance));

	apply_Movement();

	apply_Attacks();

	apply_inlined<46640, 0x86, 0x60, 4>();    // CombatBehaviorIdle
	apply_inlined<46640, 0x24C, 0x21A, 5>();  // CombatBehaviorContextBlock
	apply_inlined<48151, 0x1A1, 0x170, 4>();  // CombatBehaviorContextMelee
}
