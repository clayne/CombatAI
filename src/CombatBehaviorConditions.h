#pragma once

float get_Dummy_prop(RE::Actor*, RE::Actor*);
bool should_interruptBlock(RE::Actor* a);
bool should_interruptAttack(RE::Actor* a);

float get_OffensiveBash_prop(RE::Actor*, RE::Actor*);
float get_Block_prop(RE::Actor* me, RE::Actor* he);
float get_BashAfterAttack_prop(RE::Actor* me, RE::Actor*);
float get_DefensiveBash_prop(RE::Actor* me, RE::Actor* he);
float get_BlockType_prop(RE::Actor*, RE::Actor*);
float get_Attack_prop(RE::Actor*, RE::Actor*);
float get_SpecialAttack_prop(RE::Actor*, RE::Actor*);
