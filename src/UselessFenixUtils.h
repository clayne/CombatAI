#pragma once

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
