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
		auto& trampoline = SKSE::GetTrampoline();
		_update = trampoline.write_branch<5>(REL::ID(529964).address() + 0x9, update);  // SkyrimSE.exe+7DFF29
		_set_two_radiuses = trampoline.write_call<5>(REL::ID(46703).address() + 0x16F, set_two_radiuses);  // SkyrimSE.exe+7D659F
	}

private:
	static void update(char** context);
	static inline REL::Relocation<decltype(update)> _update;

	static void set_two_radiuses(RE::Actor* me, RE::Actor *he, float* inner_R, float* outer_R);
	static inline REL::Relocation<decltype(set_two_radiuses)> _set_two_radiuses;
};

void apply_hooks();
