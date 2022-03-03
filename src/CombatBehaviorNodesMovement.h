#pragma once 
#include "CombatBehaviorNodesGeneral.h"
                                    
class CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextCloseMovement_ : public CombatBehaviorTreeNode
{
public:
	inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextCloseMovement_;

	void* destroy(char need_freeself) override;
	void pop(AI_Tree_Control* control) override;
	void on_childfailed(AI_Tree_Control* control) override;
	void on_interrupted(AI_Tree_Control* control) override;
	void* SaveGame(AI_Tree_Control* control, RE::BGSSaveGameBuffer* buffer) override;
	void* LoadGame(AI_Tree_Control* control, RE::BGSLoadGameBuffer* buffer) override;
	bool __unk_8(AI_Tree_Control* control) override;
	RE::BSFixedString* __unk_9() override;
};
static_assert(sizeof(CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextCloseMovement_) == 0x28);

class CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_ : public CombatBehaviorTreeCreateContextNodeBase_CombatBehaviorContextCloseMovement_
{
public:
	inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_;

	void* destroy(char need_freeself) override;
	AI_Tree_Control* act(AI_Tree_Control* control) override;

	static CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_* createnew();
};
static_assert(sizeof(CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_) == 0x28);
using NodeCloseMovementContext = CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorCircleDistant);  // 141696210
using NodeCloseMovementCircleDistant = CombatBehaviorTreeNodeObject_CombatBehaviorCircleDistant_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorSurround);  // 141696160
using NodeCloseMovementSurround = CombatBehaviorTreeNodeObject_CombatBehaviorSurround_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorAdvance);  // 1416960B0
using NodeCloseMovementAdvance = CombatBehaviorTreeNodeObject_CombatBehaviorAdvance_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorFallbackToRanged);  // 141696000
using NodeCloseMovementFallbackToRanged = CombatBehaviorTreeNodeObject_CombatBehaviorFallbackToRanged_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorReposition);  // 1416962C0
using NodeCloseMovementReposition = CombatBehaviorTreeNodeObject_CombatBehaviorReposition_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorBackoff);  // 141696580
using NodeCloseMovementBackoff = CombatBehaviorTreeNodeObject_CombatBehaviorBackoff_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorCircle);  // 1416964D0
using NodeCloseMovementCircle = CombatBehaviorTreeNodeObject_CombatBehaviorCircle_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorFallback);  // 141696420
using NodeCloseMovementFallback = CombatBehaviorTreeNodeObject_CombatBehaviorFallback_;



