#pragma once
#include <xbyak/xbyak.h>

template <size_t BRANCH_TYPE, uint64_t ID, size_t offset = 0, bool call = false>
void add_trampoline(Xbyak::CodeGenerator* xbyakCode)
{
	constexpr REL::ID funcOffset = REL::ID(ID);
	auto funcAddr = funcOffset.address();
	auto size = xbyakCode->getSize();
	auto& trampoline = SKSE::GetTrampoline();
	auto result = trampoline.allocate(size);
	std::memcpy(result, xbyakCode->getCode(), size);
	if (!call)
		trampoline.write_branch<BRANCH_TYPE>(funcAddr + offset, (std::uintptr_t)result);
	else
		trampoline.write_call<BRANCH_TYPE>(funcAddr + offset, (std::uintptr_t)result);
}

template <int ID, int OFFSET>
void apply_call(std::uintptr_t func)
{
	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t jump_addr)
		{
			mov(rax, jump_addr);
			jmp(rax);
		}
	} xbyakCode{ func };
	add_trampoline<5, ID, OFFSET, true>(&xbyakCode);
}

template <int ID>
void apply_func(std::uintptr_t func)
{
	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t jump_addr)
		{
			mov(rax, jump_addr);
			jmp(rax);
		}
	} xbyakCode{ func };
	add_trampoline<6, ID>(&xbyakCode);
}

// at offset_end:
//   mov rax, rbx
//   ret
// at offset_nop:
//   nop * n
template <int ID, int offset_end, int offset_nop, int nop_len>
void apply_inlined()
{
	static_assert(nop_len == 4 || nop_len == 5);

	constexpr REL::ID funcOffset(ID);
	const char data_end[] = "\x48\x89\xD8\xC3";
	REL::safe_write(funcOffset.address() + offset_end, data_end, 4);

	const auto data_nop = (nop_len == 4) ? "\x0F\x1F\x40\x00" : "\x0F\x1F\x44\x00\x00";
	REL::safe_write(funcOffset.address() + offset_nop, data_nop, nop_len);
}

template <int id, typename T, typename... Args>
T _generic_foo(Args... args)
{
	using func_t = T(Args...);
	REL::Relocation<func_t> func{ REL::ID(id) };
	return func(std::forward<Args>(args)...);
}

template<typename T>
T* createnew_withname(T* node, const char* name)
{
	node->set_name(name);
	return node;
}

static inline const float PI = 3.141592653f;

namespace Utils
{
	RE::BGSAttackData* get_attackData(RE::Actor* a);
	
	float two_segments(float x, float Lx, float Mx, float Rx, float Ly, float My, float Ry);

	struct PolarAngle
	{
		float alpha;
		operator float() const { return alpha; }
		PolarAngle(float x = 0.0f) :
			alpha(x){};
		PolarAngle(const RE::NiPoint3& p) {
			const float y = p.y;
			if (y == 0.0) {
				if (p.x <= 0.0)
					alpha = PI * 1.5f;
				else
					alpha = PI * 0.5f;
			} else {
				alpha = atanf(p.x / y);
				if (y < 0.0)
					alpha += PI;
			}
			alpha = alpha * 180.0f / PI;
		}
		PolarAngle add(const PolarAngle& r) const {
			float ans = alpha + r.alpha;
			if (ans >= 360.0f)
				return { ans - 360.0f };
			else
				return { ans };
		}
		PolarAngle sub(const PolarAngle& r) const {
			return this->add({ 360.0f - r.alpha });
		}
		float to_normangle() const {
			if (alpha > 180.0f)
				return alpha - 360.0f;
			else
				return alpha;
		}
		float to_normangle_abs() const {
			return abs(to_normangle());
		}
		static bool ordered(PolarAngle alpha, PolarAngle beta, PolarAngle gamma)
		{
			gamma = gamma.sub(alpha);
			beta = beta.sub(alpha);
			return gamma >= beta;
		}
		static float dist(float r, PolarAngle alpha, PolarAngle beta)
		{
			//logger::info("dist: r={}, alpha={}, beta={}", r, alpha, beta);
			auto ans = r * (beta - alpha) / 180.0f * PI;
			//logger::info("ans={}", ans);
			return ans;
		}
	};
}

void NiMemFree_14134A498(void* a1, uint64_t a2);
RE::Character* CombatAI__get_he();
RE::Character* CombatAI__get_me();
