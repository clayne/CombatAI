#pragma once 
#include "CombatBehaviorNodesGeneral.h"

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorAttack);  // 14169E548 14169E5A0
using NodeMeleeAttack = CombatBehaviorTreeNodeObject_CombatBehaviorAttack_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorSpecialAttack);  // 14169E498 14169E4F0
using NodeMeleeSpecialAttack = CombatBehaviorTreeNodeObject_CombatBehaviorSpecialAttack_;

class CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextMelee_ : public CombatBehaviorTreeNode  // 141694C20
{
public:
	inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextBlock_;

	void* destroy(char need_freeself) override;
	void pop(CombatBehaviorTreeControl* control) override;
	void* SaveGame(CombatBehaviorTreeControl* control, RE::BGSSaveGameBuffer* buffer) override;
	void* LoadGame(CombatBehaviorTreeControl* control, RE::BGSLoadGameBuffer* buffer) override;
	bool __unk_8(CombatBehaviorTreeControl* control) override;
	RE::BSFixedString* __unk_9() override;

	void* (*_LocaleUpdate__GetLocaleT)(void* a1);
	int num_30;
	char __pad_34[4];
};
static_assert(sizeof(CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextMelee_) == 0x38);

class CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextMelee_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_ : public CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextMelee_
{
public:
	inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextMelee_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_;

	void* destroy(char need_freeself) override;
	CombatBehaviorTreeControl* act(CombatBehaviorTreeControl* control) override;

	static CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextMelee_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_* createnew();

private:
	struct Code : Xbyak::CodeGenerator
	{
		Code(uintptr_t call_loc)
		{
			push(rbx);
			push(rcx);
			push(rdx);
			push(r8);
			push(r9);
			sub(rsp, 8);

			mov(rax, call_loc);
			call(rax);

			add(rsp, 8);
			pop(r9);
			pop(r8);
			pop(rdx);
			pop(rcx);
			pop(rbx);
			ret();
		}
	};

	inline static const Code xbyakCode{ REL::ID(48151).address() + 0x13B };
	using createnewImpl_t = CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextMelee_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_* (*)();
	inline static const createnewImpl_t createnewImpl = xbyakCode.getCode<createnewImpl_t>();
};
static_assert(sizeof(CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextMelee_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_) == 0x38);
using NodeMeleeContext = CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextMelee_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_;
