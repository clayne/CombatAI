#pragma once

#include <RE/B/BSFixedString.h>
#include <xbyak/xbyak.h>
#include "UselessFenixUtils.h"
#include "CombatAIController.h"

using NodeArray = RE::BSTArray<CombatBehaviorTreeNode*>;

class CombatBehaviorTreeNode
{
public:
	inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeNode;  // 141692510

	// add
	virtual void destroy(char need_freeself);
	virtual char* get_name();
	virtual AI_Tree_Control* act(AI_Tree_Control* control);
	virtual void pop(AI_Tree_Control* control);
	virtual void on_childfailed(AI_Tree_Control* control);
	virtual void on_interrupted(AI_Tree_Control* control);
	virtual void* SaveGame(AI_Tree_Control* control, RE::BGSSaveGameBuffer* buffer);
	virtual void* LoadGame(AI_Tree_Control* control, RE::BGSLoadGameBuffer* buffer);
	virtual bool __unk_8(AI_Tree_Control* control);
	virtual RE::BSFixedString* __unk_9();

	void init_childrens_parents(NodeArray& cont_node)
	{
		return _generic_foo<46305, void, CombatBehaviorTreeNode*, NodeArray&>(this, cont_node);
	}

	CombatBehaviorTreeNode* add_FirstChildren(CombatBehaviorTreeNode* child)
	{
		_generic_foo<46304, void, CombatBehaviorTreeNode*, CombatBehaviorTreeNode*>(this, child);
		return child;
	}

	void set_name(const char* _name) {
		name = _name;
	}

	// members
	RE::BSFixedString name;
	CombatBehaviorTreeNode* parent;
	CombatBehaviorTreeNode** childs;
	int childs_count;
	char __pad_24[4];
};
static_assert(sizeof(CombatBehaviorTreeNode) == 0x28);

#define DECLARE_CombatBehaviorTreeNodeObjectBase(T)                                           \
	class CombatBehaviorTreeNodeObjectBase_##T##_ : public CombatBehaviorTreeNode             \
	{                                                                                         \
	public:                                                                                   \
		inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeNodeObjectBase_##T##_; \
                                                                                              \
		void destroy(char need_freeself) override;                                            \
		void pop(AI_Tree_Control* control) override;                                          \
		void on_childfailed(AI_Tree_Control* control) override;                               \
		void on_interrupted(AI_Tree_Control* control) override;                               \
		void* SaveGame(AI_Tree_Control* control, RE::BGSSaveGameBuffer* buffer) override;     \
		void* LoadGame(AI_Tree_Control* control, RE::BGSLoadGameBuffer* buffer) override;     \
		bool __unk_8(AI_Tree_Control* control) override;                                      \
		RE::BSFixedString* __unk_9() override;                                                \
	};                                                                                        \
	static_assert(sizeof(CombatBehaviorTreeNodeObjectBase_##T##_) == 0x28);

#define DECLARE_CombatBehaviorTreeNodeObject_(T)                                               \
	class CombatBehaviorTreeNodeObject_##T##_ : public CombatBehaviorTreeNodeObjectBase_##T##_ \
	{                                                                                          \
	public:                                                                                    \
		inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeNodeObject_##T##_;      \
                                                                                               \
		void destroy(char need_freeself) override;                                             \
		AI_Tree_Control* act(AI_Tree_Control* control) override;                               \
                                                                                               \
		static CombatBehaviorTreeNodeObject_##T##_* createnew();                               \
	};                                                                                         \
	static_assert(sizeof(CombatBehaviorTreeNodeObject_##T##_) == 0x28);

#define DECLARE_CombatBehaviorTreeNodeObject(T)  \
	DECLARE_CombatBehaviorTreeNodeObjectBase(T); \
	DECLARE_CombatBehaviorTreeNodeObject_(T);

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorRepeat);                                // 14162FAC8 14162FB20
using NodeRepeat = CombatBehaviorTreeNodeObject_CombatBehaviorRepeat_;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorFallbackSelector_NextChildSelector__);  // 14162FB70 14162FBD0
using NodeSelector = CombatBehaviorTreeNodeObject_CombatBehaviorFallbackSelector_NextChildSelector___;

DECLARE_CombatBehaviorTreeNodeObjectBase(CombatBehaviorIdle);                              // 14162FCE8

#define DEFINE_CombatBehaviorTreeNodeObject_createnew(T, id)                              \
	CombatBehaviorTreeNodeObject_##T##_* CombatBehaviorTreeNodeObject_##T##_::createnew() \
	{                                                                                     \
		return _generic_foo<id, CombatBehaviorTreeNodeObject_##T##_*>();                  \
	}

class CombatBehaviorTreeNodeObject1_CombatBehaviorIdle_float_ : public CombatBehaviorTreeNodeObjectBase_CombatBehaviorIdle_  // 14162FD40
{
public:
	inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeNodeObject1_CombatBehaviorIdle_float_;

	// override (CombatBehaviorTreeNodeObjectBase_CombatBehaviorIdle_)
	void destroy(char need_freeself) override;
	AI_Tree_Control* act(AI_Tree_Control* control) override;

	static CombatBehaviorTreeNodeObject1_CombatBehaviorIdle_float_* createnew();

	// members
	float flt_28;
	char __pad_2C[4];

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

	inline static const Code xbyakCode{ REL::ID(46640).address() + 0x2B };
	using createnewImpl_t = CombatBehaviorTreeNodeObject1_CombatBehaviorIdle_float_* (*)();
	inline static const createnewImpl_t createnewImpl = xbyakCode.getCode<createnewImpl_t>();
};
static_assert(sizeof(CombatBehaviorTreeNodeObject1_CombatBehaviorIdle_float_) == 0x30);
using NodeIdle = CombatBehaviorTreeNodeObject1_CombatBehaviorIdle_float_;

class CombatBehaviorTreeConditionalNode_CombatBehaviorExpression_CombatBehaviorFunc1_bool____cdecl___float__CombatBehaviorExpression_CombatBehaviorFunc2_float____cdecl___Actor_____ptr64_Actor_____ptr64__CombatBehaviorTree__CombatBehaviorAttacker_CombatBehaviorTree__CombatBehaviorTarget_________ : public CombatBehaviorTreeNode
{
public:
	inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeConditionalNode_CombatBehaviorExpression_CombatBehaviorFunc1_bool____cdecl___float__CombatBehaviorExpression_CombatBehaviorFunc2_float____cdecl___Actor_____ptr64_Actor_____ptr64__CombatBehaviorTree__CombatBehaviorAttacker_CombatBehaviorTree__CombatBehaviorTarget_________;

	void destroy(char need_freeself) override;
	AI_Tree_Control* act(AI_Tree_Control* control) override;
	bool __unk_8(AI_Tree_Control* control) override;
	RE::BSFixedString* __unk_9() override;

	bool (*cmp_rnd)(float chance);
	float (*get_ActionChance)(RE::Character*, RE::PlayerCharacter*);
	uint64_t __unk_38;
	char c;
	char __pad_41[7];
};
static_assert(sizeof(CombatBehaviorTreeConditionalNode_CombatBehaviorExpression_CombatBehaviorFunc1_bool____cdecl___float__CombatBehaviorExpression_CombatBehaviorFunc2_float____cdecl___Actor_____ptr64_Actor_____ptr64__CombatBehaviorTree__CombatBehaviorAttacker_CombatBehaviorTree__CombatBehaviorTarget_________) == 0x48);
