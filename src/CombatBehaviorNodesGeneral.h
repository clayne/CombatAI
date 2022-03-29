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
	virtual void* destroy(char need_freeself);
	virtual char* get_name();
	virtual CombatBehaviorTreeControl* act(CombatBehaviorTreeControl* control);
	virtual void pop(CombatBehaviorTreeControl* control);
	virtual void on_childfailed(CombatBehaviorTreeControl* control);
	virtual void on_interrupted(CombatBehaviorTreeControl* control);
	virtual void* SaveGame(CombatBehaviorTreeControl* control, RE::BGSSaveGameBuffer* buffer);
	virtual void* LoadGame(CombatBehaviorTreeControl* control, RE::BGSLoadGameBuffer* buffer);
	virtual bool __unk_8(CombatBehaviorTreeControl* control);
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

	CombatBehaviorTreeNode* set_name(const char* _name)
	{
		name = _name;
		return this;
	}

	void ctor_nonvirtual()
	{
		return _generic_foo<46301, void, CombatBehaviorTreeNode*>(this);  // SkyrimSE.exe+7C8C10
	}

	void dtor_nonvirtual() {
		return _generic_foo<46302, void, CombatBehaviorTreeNode*>(this);  // SkyrimSE.exe+7c8c40
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
                                                                                              \
		void* destroy(char need_freeself) override;                                            \
		void pop(CombatBehaviorTreeControl* control) override;                                          \
		void on_childfailed(CombatBehaviorTreeControl* control) override;                               \
		void on_interrupted(CombatBehaviorTreeControl* control) override;                               \
		void* SaveGame(CombatBehaviorTreeControl* control, RE::BGSSaveGameBuffer* buffer) override;     \
		void* LoadGame(CombatBehaviorTreeControl* control, RE::BGSLoadGameBuffer* buffer) override;     \
		bool __unk_8(CombatBehaviorTreeControl* control) override;                                      \
		RE::BSFixedString* __unk_9() override;                                                \
	};                                                                                        \
	static_assert(sizeof(CombatBehaviorTreeNodeObjectBase_##T##_) == 0x28);

#define DECLARE_CombatBehaviorTreeNodeObject_(T)                                               \
	class CombatBehaviorTreeNodeObject_##T##_ : public CombatBehaviorTreeNodeObjectBase_##T##_ \
	{                                                                                          \
	public:                                                                                    \
                                                                                               \
		void* destroy(char need_freeself) override;                                             \
		CombatBehaviorTreeControl* act(CombatBehaviorTreeControl* control) override;                               \
                                                                                               \
		static CombatBehaviorTreeNodeObject_##T##_* createnew();                               \
	};                                                                                         \
	static_assert(sizeof(CombatBehaviorTreeNodeObject_##T##_) == 0x28);

#define DECLARE_CombatBehaviorTreeNodeObject(T)  \
	DECLARE_CombatBehaviorTreeNodeObjectBase(T); \
	DECLARE_CombatBehaviorTreeNodeObject_(T);

#define DEFINE_CombatBehaviorTree_XXX__createnew(id, T) \
	T* T::createnew()                                   \
	{                                                   \
		return _generic_foo<id, T*>();                  \
	}

#define DEFINE_CombatBehaviorTreeNodeObject_createnew(T, id)                              \
DEFINE_CombatBehaviorTree_XXX__createnew(id, CombatBehaviorTreeNodeObject_##T##_)

class CombatBehaviorTreeWrapperNode : public CombatBehaviorTreeNode  // 1416937B8
{
public:
	inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeWrapperNode;

	void* destroy(char need_freeself) override;
	char* get_name() override;
	CombatBehaviorTreeControl* act(CombatBehaviorTreeControl* control) override;
	void pop(CombatBehaviorTreeControl* control) override;
	void on_childfailed(CombatBehaviorTreeControl* control) override;
	void on_interrupted(CombatBehaviorTreeControl* control) override;
	void* SaveGame(CombatBehaviorTreeControl* control, RE::BGSSaveGameBuffer* buffer) override;
	void* LoadGame(CombatBehaviorTreeControl* control, RE::BGSLoadGameBuffer* buffer) override;
	bool __unk_8(CombatBehaviorTreeControl* control) override;
	RE::BSFixedString* __unk_9() override;

	CombatBehaviorTreeNode* node;
};
static_assert(sizeof(CombatBehaviorTreeWrapperNode) == 0x30);


DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorRepeat);                                // 14162FAC8 14162FB20
using NodeRepeat = CombatBehaviorTreeNodeObject_CombatBehaviorRepeat_;


// Selectors  //

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorFallbackSelector_NextChildSelector_);  // 14162FB70 14162FBD0
using NodeNextChildSelector = CombatBehaviorTreeNodeObject_CombatBehaviorFallbackSelector_NextChildSelector__;

DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorChildSelector_ConditionalChildSelector_);  // 141695EF0 141695F50
using NodeConditionalChildSelector = CombatBehaviorTreeNodeObject_CombatBehaviorChildSelector_ConditionalChildSelector__;

// ^ Selectors ^ //

// Conditionals //

class CombatBehaviorTreeConditionalNode_CombatBehaviorExpression_CombatBehaviorFunc1_bool___float__CombatBehaviorExpression_CombatBehaviorFunc2_float___Actor_Actor__CombatBehaviorTree__CombatBehaviorAttacker_CombatBehaviorTree__CombatBehaviorTarget_____ : public CombatBehaviorTreeNode
{
public:
	
	void *destroy(char need_freeself) override;
	CombatBehaviorTreeControl* act(CombatBehaviorTreeControl* control) override;
	bool __unk_8(CombatBehaviorTreeControl* control) override;
	RE::BSFixedString* __unk_9() override;

	bool (*cmp_rnd)(float chance);
	float (*get_ActionChance)(RE::Character*, RE::PlayerCharacter*);
	uint64_t __unk_38;
	char c;
	char __pad_41[7];
};
static_assert(sizeof(CombatBehaviorTreeConditionalNode_CombatBehaviorExpression_CombatBehaviorFunc1_bool___float__CombatBehaviorExpression_CombatBehaviorFunc2_float___Actor_Actor__CombatBehaviorTree__CombatBehaviorAttacker_CombatBehaviorTree__CombatBehaviorTarget_____) == 0x48);

// ^ Conditionals ^ //

// ValueNodes //


class CombatBehaviorTreeValueNode_unsignedint_ : public CombatBehaviorTreeWrapperNode  // 1416941F0
{
public:

	void* destroy(char need_freeself) override;
	RE::BSFixedString* __unk_9() override;
	virtual uint32_t __unk10();
};
static_assert(sizeof(CombatBehaviorTreeValueNode_unsignedint_) == 0x30);

class CombatBehaviorTreeValueNode_float_ : public CombatBehaviorTreeWrapperNode  // 141696710
{
public:

	void* destroy(char need_freeself) override;
	RE::BSFixedString* __unk_9() override;
	virtual float __unk10(void);
};
static_assert(sizeof(CombatBehaviorTreeValueNode_float_) == 0x30);


// valuenode_1 1407DA2B0
class CombatBehaviorTreeValueNodeT_unsignedint_int_ : public CombatBehaviorTreeValueNode_unsignedint_
{
public:

	void* destroy(char need_freeself) override;
	uint32_t __unk10(void) override;

	int num;
	char pad[4];
};
static_assert(sizeof(CombatBehaviorTreeValueNodeT_unsignedint_int_) == 0x38);

// valuenode_14 1407DA3C0
class CombatBehaviorTreeValueNodeT_unsignedint_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorContextCloseMovement_bool_CombatBehaviorContextCloseMovement____void____ : public CombatBehaviorTreeValueNode_unsignedint_
{
public:
	
	void* destroy(char need_freeself) override;
	uint32_t __unk10(void) override;

	bool(__fastcall* func)(void /* CombatBehaviorContextCloseMovement */ *);
};
static_assert(sizeof(CombatBehaviorTreeValueNodeT_unsignedint_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorContextCloseMovement_bool_CombatBehaviorContextCloseMovement____void____) == 0x38);

class CombatBehaviorTreeValueNodeT_float_CombatBehaviorExpression_CombatBehaviorFunc1_float___Actor__CombatBehaviorTree__CombatBehaviorAttacker___ : public CombatBehaviorTreeValueNode_float_  // 141696778
{
public:
	
	void* destroy(char need_freeself) override;
	float __unk10(void) override;

	float (*func)(RE::Actor* attacker);
	void* unk;
};
static_assert(sizeof(CombatBehaviorTreeValueNodeT_float_CombatBehaviorExpression_CombatBehaviorFunc1_float___Actor__CombatBehaviorTree__CombatBehaviorAttacker___) == 0x40);

// valuenode_10 8176B0
class CombatBehaviorTreeValueNodeT_unsignedint_CombatBehaviorExpression_CombatBehaviorMemberFunc_Actor_bool_Actor____void____ : public CombatBehaviorTreeValueNode_unsignedint_  // 14169CDC8
{
public:
	
	void* destroy(char need_freeself) override;
	uint32_t __unk10(void) override;

	bool (*func)(RE::Actor* a);
	uint64_t zero;
};
static_assert(sizeof(CombatBehaviorTreeValueNodeT_unsignedint_CombatBehaviorExpression_CombatBehaviorMemberFunc_Actor_bool_Actor____void____) == 0x40);

// ^ ValueNodes ^ //

DECLARE_CombatBehaviorTreeNodeObjectBase(CombatBehaviorIdle);  // 14162FCE8

class CombatBehaviorTreeNodeObject1_CombatBehaviorIdle_float_ : public CombatBehaviorTreeNodeObjectBase_CombatBehaviorIdle_  // 14162FD40
{
public:
	
	// override (CombatBehaviorTreeNodeObjectBase_CombatBehaviorIdle_)
	void* destroy(char need_freeself) override;
	CombatBehaviorTreeControl* act(CombatBehaviorTreeControl* control) override;

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
