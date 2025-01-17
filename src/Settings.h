#pragma once
#include "PCH.h"

struct Settings
{
	using iSetting = AutoTOML::iSetting;
	using bSetting = AutoTOML::bSetting;
	using fSetting = AutoTOML::fSetting;

	static void load()
	{
		try {
			const auto table = toml::parse_file("Data/SKSE/Plugins/CombatAI.toml"s);
			for (const auto& setting : AutoTOML::ISetting::get_settings()) {
				setting->load(table);
			}
		} catch (const toml::parse_error& e) {
			std::ostringstream ss;
			ss
				<< "Error parsing file \'" << *e.source().path << "\':\n"
				<< '\t' << e.description() << '\n'
				<< "\t\t(" << e.source().begin << ')';
			logger::error(ss.str());
			throw std::runtime_error("failed to load settings"s);
		}
	}

	static inline iSetting DodgingModIndex{ "Dodging"s, "ModIndex"s, 0 };
};

