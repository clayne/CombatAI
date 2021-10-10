#include "CombatBehaviorNodes.h"
#include "CombatBehaviorConditions.h"

struct TreeCtors_extradata
{
	void* func1;
	void* func2;
};

// Node* p ("RandomNode - name")
// addch(p, node)
// array.push_bach(node);
NodeArray& wrap_to_randomnode(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	return _generic_foo<46641, NodeArray&, NodeArray&, const char*, void*, CombatBehaviorTreeNode*>(a, name, extradata, node);
}

NodeArray& init_withNode_withname(NodeArray& array, const char* name, CombatBehaviorTreeNode* node)
{
	return _generic_foo<46261, NodeArray&, NodeArray&, const char*, CombatBehaviorTreeNode*>(array, name, node);
}

NodeArray& pushback_parentof(NodeArray& array, NodeArray& cont_node)
{
	return _generic_foo<46255, NodeArray&, NodeArray&, NodeArray&>(array, cont_node);
}

void CombatBehaviorTree__init_nodes(void* self, CombatBehaviorTreeNode* root)
{
	return _generic_foo<46263, void, void*, CombatBehaviorTreeNode*>(self, root);
}

// TODO: replace
CombatBehaviorTreeNode* tmp_wrap_to_randomnode(const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	NodeArray cont;
	wrap_to_randomnode(cont, name, extradata, node);
	return cont[0]->parent;
}

void CombatBehaviorTreeBlock__ctor(void* self)
{
	NodeArray ActionTypes, BlockTypes;
	TreeCtors_extradata extradata;

	extradata = { get_OffensiveBash_prop, nullptr };
	wrap_to_randomnode(ActionTypes, "Offensive Bash", &extradata, NodeBlockBash::createnew());

	extradata = { get_DefensiveBash_prop, nullptr };
	wrap_to_randomnode(BlockTypes, "Defensive Bash", &extradata, NodeBlockBash::createnew());

	extradata = { get_Block_prop, nullptr };
	BlockTypes.push_back(tmp_wrap_to_randomnode("Block Attack", &extradata, NodeBlockBlockAttack::createnew()));
	extradata = { get_BashAfterAttack_prop, nullptr };
	BlockTypes[1]->childs[0]->add_FirstChildren(tmp_wrap_to_randomnode("Conter Attack", &extradata, NodeBlockBash::createnew()));

	extradata = { get_BlockType_prop, nullptr };
	ActionTypes.push_back(tmp_wrap_to_randomnode("Block Type Selector", &extradata, NodeSelector::createnew()));
	ActionTypes[ActionTypes.size() - 1]->childs[0]->init_childrens_parents(BlockTypes);

	ActionTypes.push_back(createnew_withname(NodeIdle::createnew(), "Block Idle"));

	auto node_BlockContext = createnew_withname(NodeBlockContext::createnew(), "Block Context");

	node_BlockContext
		->add_FirstChildren(createnew_withname(NodeRepeat::createnew(), "Block Repeat"))
		->add_FirstChildren(createnew_withname(NodeSelector::createnew(), "Block Selector"))
		->init_childrens_parents(ActionTypes);

	CombatBehaviorTree__init_nodes(self, node_BlockContext);
}

void CombatBehaviorTreeMelee__ctor(void* self)
{
	NodeArray ActionTypes;
	TreeCtors_extradata extradata;

	extradata = { get_SpecialAttack_prop, nullptr };
	wrap_to_randomnode(ActionTypes, "Special Attack", &extradata, NodeMeleeSpecialAttack::createnew());

	extradata = { get_Attack_prop, nullptr };
	ActionTypes.push_back(tmp_wrap_to_randomnode("Attack", &extradata, NodeMeleeAttack::createnew()));
	extradata = { get_Dummy_prop, nullptr };
	ActionTypes[ActionTypes.size() - 1]->childs[0]->add_FirstChildren(tmp_wrap_to_randomnode("Dummy", &extradata, NodeBlockBlock::createnew()));

	ActionTypes.push_back(createnew_withname(NodeIdle::createnew(), "Attack Idle"));

	auto node_MeleeContext = createnew_withname(NodeMeleeContext::createnew(), "Melee Context");

	node_MeleeContext
		->add_FirstChildren(createnew_withname(NodeRepeat::createnew(), "Atack Repeat"))
		->add_FirstChildren(createnew_withname(NodeSelector::createnew(), "Atack Selector"))
		->init_childrens_parents(ActionTypes);

	CombatBehaviorTree__init_nodes(self, node_MeleeContext);
}
