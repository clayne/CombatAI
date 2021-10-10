#include <xbyak/xbyak.h>
#include "UselessFenixUtils.h"
#include "CombatBehaviorTrees.h"
#include <CombatBehaviorConditions.h>

template <size_t BRANCH_TYPE, uint64_t ID, size_t offset = 0, bool call = false>
void add_trampoline(Xbyak::CodeGenerator* xbyakCode)
{
	constexpr REL::ID funcOffset = REL::ID(ID);
	auto funcAddr = funcOffset.address();
	auto size = xbyakCode->getSize();
	auto& trampoline = SKSE::GetTrampoline();
	auto result = trampoline.allocate(size);
	std::memcpy(result, xbyakCode->getCode(), size);
	if constexpr (!call)
		trampoline.write_branch<BRANCH_TYPE>(funcAddr + offset, (std::uintptr_t)result);
	else
		trampoline.write_call<BRANCH_TYPE>(funcAddr + offset, (std::uintptr_t)result);
}

void apply_Melee_ctor()
{
	// SkyrimSE.exe+80DF60
	const int ID = 48151, BRANCH_TYPE = 6;

	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t jump_addr)
		{
			push(rax);
			mov(rax, jump_addr);
			call(rax);
			pop(rax);
			ret();
		}
	} xbyakCode{ std::uintptr_t(CombatBehaviorTreeMelee__ctor) };
	add_trampoline<BRANCH_TYPE, ID>(static_cast<Xbyak::CodeGenerator*>(&xbyakCode));
}

void apply_Block_ctor()
{
	// SkyrimSE.exe+7d4620
	const int ID = 46640, BRANCH_TYPE = 6;

	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t jump_addr)
		{
			push(rax);
			mov(rax, jump_addr);
			call(rax);
			pop(rax);
			ret();
		}
	} xbyakCode{ std::uintptr_t(CombatBehaviorTreeBlock__ctor) };
	add_trampoline<BRANCH_TYPE, ID>(static_cast<Xbyak::CodeGenerator*>(&xbyakCode));
}

// at offset_end:
//   mov rax, rbx
//   ret
// at offset_nop:
//   nop * n
template<int ID, int offset_end, int offset_nop, int nop_len>
void apply_inlined() {
	static_assert(nop_len == 4 || nop_len == 5);

	constexpr REL::ID funcOffset(ID);
	const char data_end[] = "\x48\x89\xD8\xC3";
	REL::safe_write(funcOffset.address() + offset_end, data_end, 4);

	const auto data_nop = (nop_len == 4) ? "\x0F\x1F\x40\x00" : "\x0F\x1F\x44\x00\x00";
	REL::safe_write(funcOffset.address() + offset_nop, data_nop, nop_len);
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

			push(rax);
			push(rcx);
			mov(rcx, rbp);
			mov(rax, call_addr);
			call(rax);
			test(al, al);
			je(noint);
			pop(rcx);
			pop(rax);
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

void apply_hooks()
{
	SKSE::AllocTrampoline(1 << 10);

	apply_Block_ctor();
	apply_Melee_ctor();
	apply_inlined<46640, 0x24C, 0x21A, 5>();  // CombatBehaviorContextBlock
	apply_inlined<46640, 0x86, 0x60, 4>();       // CombatBehaviorIdle
	apply_inlined<48151, 0x1A1, 0x170, 4>();  // CombatBehaviorContextMelee
	apply_BlockAttackInterrupting();
	apply_AttackInterrupting();
}
