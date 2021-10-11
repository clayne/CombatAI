#include "CombatBehaviorNodesGeneral.h"

DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorFallbackSelector_NextChildSelector__, 32407);
DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorRepeat, 32409);

// 1407D4620 (46640)
// 1407D464B (+0x2B) -- 1407D46A9 (+0x89)
// safewrite at 1407D46A6 (+0x86)
//   mov rax, rbx
//   ret
// safewrite at 1407D4680 (+0x60)
//   nop * 4
// remember registers rbx, rcx, rdx, r8, r9
// ans in rbx
CombatBehaviorTreeNodeObject1_CombatBehaviorIdle_float_* CombatBehaviorTreeNodeObject1_CombatBehaviorIdle_float_::createnew()
{
	return (*createnewImpl)();
}
