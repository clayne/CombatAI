#include "CombatBehaviorNodesBlock.h"

DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorBash, 46646);
DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorBlock, 46647);
DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorBlockAttack, 46648);

// 1407D4620 (46640)
// 1407D4805 (+0x1E5) -- 1407D46A9 (+0x24F)
// safewrite at 1407D46A6 (+0x24C)
//   mov rax, rbx
//   ret
// safewrite at 1407D483A (+0x21A)
//   nop * 5
// remember registers rbx, rcx, rdx, r8, r9
// ans in rbx
NodeBlockContext* NodeBlockContext::createnew()
{
	return (*createnewImpl)();
}
