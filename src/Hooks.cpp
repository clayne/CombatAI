#include "UselessFenixUtils.h"
#include "CombatBehaviorTrees.h"
#include <CombatBehaviorConditions.h>
#include "Hooks.h"

void MainUpdateHook::Update(RE::Main* a_this, float a2)
{
	_Update(a_this, a2);

	DebugAPI::Update();
}

void AdvanceHook::func(char** context)
{
	Movement::Advance::hooked_interrupting(context);

	_func(context);
}

void apply_BlockAttackInterrupting()
{
	// SkyrimSE.exe+7D39BD
	const int ID = 46629, BRANCH_TYPE = 6;
	constexpr REL::ID funcOffset = REL::ID(ID);
	auto funcAddr = funcOffset.address();

	// SkyrimSE.exe+7D5200
	const int int_ID = 529951;
	constexpr REL::ID int_funcOffset = REL::ID(int_ID);
	auto int_funcAddr = int_funcOffset.address();
	
	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t call_addr, uintptr_t ret_addr, uintptr_t interrupt_addr)
		{
			Xbyak::Label noint;
			Xbyak::Label loc_1407D3A79;

			push(rax);
			push(rcx);
			mov(rcx, rbp);
			mov(rax, call_addr);
			call(rax);
			test(al, al);
			je(noint);
			pop(rcx);
			pop(rax);

			test(rsi, rsi);
			jz(loc_1407D3A79);
			lea(rcx, ptr[rsi + 0x20]);
			mov(eax, 0x0FFFFFFFF);
			xadd(ptr[rcx + 8], eax);
			dec(eax);
			test(eax, 0x3FF);
			jnz(loc_1407D3A79);
			mov(rax, ptr[rcx]);
			call(ptr[rax + 8]);

			L(loc_1407D3A79);
			lea(r11, ptr[rsp + 0x80]);
			mov(rbx, ptr[r11 + 0x20]);
			mov(rbp, ptr[r11 + 0x28]);
			mov(rsi, ptr[r11 + 0x30]);
			mov(rsp, r11);
			pop(r15);
			pop(r14);
			pop(rdi);
			mov(rax, interrupt_addr);
			jmp(rax);

			L(noint);
			pop(rcx);
			pop(rax);
			xorps(xmm0,	xmm0);
			ucomiss(xmm0, ptr[rdi + 0x0C]);
			mov(rax, ret_addr);
			jmp(rax);
		}
	} xbyakCode{ std::uintptr_t(should_interruptBlock), std::uintptr_t(funcAddr + 0xC4), std::uintptr_t(int_funcAddr) };
	add_trampoline<BRANCH_TYPE, ID, 0xBD>(static_cast<Xbyak::CodeGenerator*>(&xbyakCode));
}

void apply_AttackInterrupting()
{
	// SkyrimSE.exe+80DB00
	const int ID = 48148, BRANCH_TYPE = 6;
	constexpr REL::ID funcOffset = REL::ID(ID);
	auto funcAddr = funcOffset.address();

	// SkyrimSE.exe+80E7D0
	const int int_ID = 530054;
	constexpr REL::ID int_funcOffset = REL::ID(int_ID);
	auto int_funcAddr = int_funcOffset.address();

	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t call_addr, uintptr_t ret_addr, uintptr_t interrupt_addr)
		{
			Xbyak::Label noint;

			push(rax);
			push(rcx);
			mov(rax, call_addr);
			call(rax);
			test(al, al);
			je(noint);
			pop(rcx);
			pop(rax);
			mov(rdx, ptr[rsi]);
			lea(r11, ptr[rsp + 0x80]);
			mov(rbx, ptr[r11 + 0x10]);
			mov(rsi, ptr[r11 + 0x18]);
			mov(rsp, r11);
			pop(rdi);
			mov(rax, interrupt_addr);
			jmp(rax);

			L(noint);
			pop(rcx);
			pop(rax);
			
			mov(eax, ptr[rcx + 0xC0]);
			shr(eax, 0x1C);
			test(eax, 0x0FFFFFFFB);
			mov(rax, ret_addr);
			jmp(rax);
		}
	} xbyakCode{ std::uintptr_t(should_interruptAttack), std::uintptr_t(funcAddr + 0xB9), std::uintptr_t(int_funcAddr) };
	add_trampoline<BRANCH_TYPE, ID, 0xAB>(static_cast<Xbyak::CodeGenerator*>(&xbyakCode));
}

void apply_Movement()
{
	using namespace Movement;

	{	// Fallback
		// get_FallbackDistance: SkyrimSE.exe+844FE0
		apply_func<49718>(std::uintptr_t(Fallback::get_FallbackDistance));

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
		{  // SkyrimSE.exe+7D750B: not check heading1
			constexpr REL::ID funcOffset(46712);
			const char data[] = "\x0F\x1F\x80\x00\x00\x00\x00";  // nop * 7
			REL::safe_write(funcOffset.address() + 0x13B, data, 7);
		}
		{  // SkyrimSE.exe+7D7512: not check heading2
			constexpr REL::ID funcOffset(46712);
			const char data[] = "\x66\x0F\x1F\x44\x00\x00";  // nop * 6
			REL::safe_write(funcOffset.address() + 0x142, data, 6);
		}
	}
	{	// Circle
		// call Character__get_CircleAngle: SkyrimSE.exe+7D8499
		apply_call<46720, 0x299>(std::uintptr_t(Circle::get_CircleAngle));
		
		{	// SkyrimSE.exe+7D84E9: do not circle_angle = -circle_angle
			constexpr REL::ID funcOffset(46720);
			const char data[] = "\x0F\x1F\x40\x00";  // nop * 4
			REL::safe_write(funcOffset.address() + 0x2E9, data, 4);
		}
		{	// SkyrimSE.exe+7D876E: 1 iteration on loop
			constexpr REL::ID funcOffset(46720);
			const char data[] = "\x41\x83\xFE\x01";  // cmp     r14d, 1
			REL::safe_write(funcOffset.address() + 0x56E, data, 4);
		}
		{  // SkyrimSE.exe+7D85AD: set max speed1
			constexpr REL::ID funcOffset(46720);
			const char data[] = "\xBA\x04\x00\x00\x00";  // mov edx, 4
			REL::safe_write(funcOffset.address() + 0x3AD, data, 5);
		}
		{  // SkyrimSE.exe+7D877D: set max speed2
			constexpr REL::ID funcOffset(46720);
			const char data[] = "\x41\xB9\x04\x00\x00\x00";  // mov r9d, 4
			REL::safe_write(funcOffset.address() + 0x57D, data, 6);
		}
		{  // SkyrimSE.exe+7D84AA: not check fCombatCircleAngleMax
			constexpr REL::ID funcOffset(46720);
			const char data[] = "\x66\x0F\x1F\x44\x00\x00";  // nop * 6
			REL::safe_write(funcOffset.address() + 0x2AA, data, 6);
		}
		{  // SkyrimSE.exe+7D8566: not check heading1
			constexpr REL::ID funcOffset(46720);
			const char data[] = "\x0F\x1F\x80\x00\x00\x00\x00";  // nop * 7
			REL::safe_write(funcOffset.address() + 0x366, data, 7);
		}
		{  // SkyrimSE.exe+7D856D: not check heading2
			constexpr REL::ID funcOffset(46720);
			const char data[] = "\x66\x0F\x1F\x44\x00\x00";  // nop * 6
			REL::safe_write(funcOffset.address() + 0x36D, data, 6);
		}
	}
}

void apply_hooks()
{
	SKSE::AllocTrampoline(1 << 10);

	MainUpdateHook::Hook();
	AdvanceHook::Hook();

	// Melee_ctor: SkyrimSE.exe+80DF60
	apply_func<48151>(uintptr_t(CombatBehaviorTreeMelee__ctor));

	// Block_ctor: SkyrimSE.exe+7d4620
	apply_func<46640>(uintptr_t(CombatBehaviorTreeBlock__ctor));

	// CloseMovement_ctor: SkyrimSE.exe+7D97D0
	apply_func<46731>(uintptr_t(CombatBehaviorTreeCloseMovement__ctor));

	apply_Movement();

	apply_inlined<46640, 0x86, 0x60, 4>();    // CombatBehaviorIdle
	apply_inlined<46640, 0x24C, 0x21A, 5>();  // CombatBehaviorContextBlock
	apply_inlined<48151, 0x1A1, 0x170, 4>();  // CombatBehaviorContextMelee

	//apply_BlockAttackInterrupting();
	//apply_AttackInterrupting();
}
