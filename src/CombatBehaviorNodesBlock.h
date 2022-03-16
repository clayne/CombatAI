#pragma once 
#include "CombatBehaviorNodesGeneral.h"

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorBash);   // 141694D80 141694DD8
using NodeBlockBash = CombatBehaviorTreeNodeObject_CombatBehaviorBash_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorBlock);  // 141694D80 141694DD8
using NodeBlockBlock = CombatBehaviorTreeNodeObject_CombatBehaviorBlock_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorBlockAttack);  // 141694D80 141694DD8
using NodeBlockBlockAttack = CombatBehaviorTreeNodeObject_CombatBehaviorBlockAttack_;

class CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextBlock_ : public CombatBehaviorTreeNode  // 141694C20
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
static_assert(sizeof(CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextBlock_) == 0x38);

class CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextBlock_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_ : public CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextBlock_
{
public:
	inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextBlock_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_;

	void* destroy(char need_freeself) override;
	CombatBehaviorTreeControl* act(CombatBehaviorTreeControl* control) override;

	static CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextBlock_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_* createnew();

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

	inline static const Code xbyakCode{ REL::ID(46640).address() + 0x1E5 };
	using createnewImpl_t = CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextBlock_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_* (*)();
	inline static const createnewImpl_t createnewImpl = xbyakCode.getCode<createnewImpl_t>();
};
static_assert(sizeof(CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextBlock_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_) == 0x38);
using NodeBlockContext = CombatBehaviorTreeCreateContextNode2_CombatBehaviorContextBlock_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorEquipContext_NiPointer_CombatInventoryItem_const_CombatBehaviorEquipContext____void____CombatBehaviorContextMelee__ATTACK_TYPE_;
