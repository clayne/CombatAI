#include "CombatBehaviorNodesMelee.h"

DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorAttack, 48154);
DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorSpecialAttack, 48155);

// 14080DF60 (48151)
// 14080E09B (+0x13B) -- 14080E104 (+0x1A4)
// safewrite at 14080E101 (+0x1A1)
//   mov rax, rbx
//   ret
// safewrite at 14080E0D0 (+0x170)
//   nop * 4
// remember registers rbx, rcx, rdx, r8, r9
// ans in rbx
NodeMeleeContext* NodeMeleeContext::createnew()
{
	return createnewImpl();
}
