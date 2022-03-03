#pragma once

class MainUpdateHook
{
public:
	static void Hook()
	{
		REL::Relocation<uintptr_t> hook{ REL::ID(35551) };

		auto& trampoline = SKSE::GetTrampoline();
		_Update = trampoline.write_call<5>(hook.address() + 0x11F, Update);  // SkyrimSE.exe+5AF4EF
	}

private:
	static void Update(RE::Main* a_this, float a2);
	static inline REL::Relocation<decltype(Update)> _Update;
};

class AdvanceHook
{
public:
	static void Hook()
	{
		REL::Relocation<uintptr_t> hook{ REL::ID(529964) };

		auto& trampoline = SKSE::GetTrampoline();
		_func = trampoline.write_branch<5>(hook.address() + 0x9, func);  // SkyrimSE.exe+7DFF29
	}

private:
	static void func(char** context);
	static inline REL::Relocation<decltype(func)> _func;
};

void apply_hooks();
