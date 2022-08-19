#include "CombatBehaviorNodes.h"
#include "CombatBehaviorConditions.h"

struct TreeCtors_extradata
{
	void* func1;
	void* func2;
};

// Node* p ("ConditionalNode - _name_")
// addch(p, node)
// array = {node}
// node.name = _name_
NodeArray& wrap_to_conditional_2(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	// SkyrimSE.exe+7da060
	return _generic_foo<46732, NodeArray&, NodeArray&, const char*, void*, CombatBehaviorTreeNode*>(a, name, extradata, node);
}

CombatBehaviorTreeNode* tmp_wrap_to_conditional_2(const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	NodeArray cont;
	wrap_to_conditional_2(cont, name, extradata, node);
	return cont[0]->parent;
}

// Node* p ("RandomNode - _name_")
// addch(p, node)
// array = {node}
// node.name = _name_
NodeArray& wrap_to_randomnode_0(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	// SkyrimSE.exe+7d4a80
	return _generic_foo<46641, NodeArray&, NodeArray&, const char*, void*, CombatBehaviorTreeNode*>(a, name, extradata, node);
}

// Node<uint32, int>* p ("ValueNode - _name_", node, extradata.i)
// array = {node}
// node.name = _name_
NodeArray& wrap_to_valuenode_1(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	// SkyrimSE.exe+7DA250
	return _generic_foo<46734, NodeArray&, NodeArray&, const char*, void*, CombatBehaviorTreeNode*>(a, name, extradata, node);
}

// Node<uint32, bool (*)(CombatBehaviorContextCloseMovement *)>* p ("ValueNode - _name_", node)
// array = {node}
// node.name = _name_
NodeArray& wrap_to_valuenode_14(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	// SkyrimSE.exe+7DA270
	return _generic_foo<46735, NodeArray&, NodeArray&, const char*, void*, CombatBehaviorTreeNode*>(a, name, extradata, node);
}

// Node<uint32, bool (*)(CombatBehaviorContextCloseMovement *)>* p ("ValueNode - _name_", node)
// array = {node}
// node.name = _name_
NodeArray& wrap_to_valuenode_10(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	// SkyrimSE.exe+8176b0
	return _generic_foo<48444, NodeArray&, NodeArray&, const char*, void*, CombatBehaviorTreeNode*>(a, name, extradata, node);
}

NodeArray& init_withNode_withname(NodeArray& array, const char* name, CombatBehaviorTreeNode* node)
{
	return _generic_foo<46261, NodeArray&, NodeArray&, const char*, CombatBehaviorTreeNode*>(array, name, node);
}

NodeArray& pushback_parentof(NodeArray& array, NodeArray& cont_node)
{
	return _generic_foo<46255, NodeArray&, NodeArray&, NodeArray&>(array, cont_node);
}

// Node* root = new node "Root - _treename_"
// tree.root = root
// root.addch(root_node)
void CombatBehaviorTree__init_nodes(void* self, CombatBehaviorTreeNode* root)
{
	return _generic_foo<46263, void, void*, CombatBehaviorTreeNode*>(self, root);  // SkyrimSE.exe+7c7950
}

bool CombatBehaviorContextCloseMovement__smth_rotation(void* context) {
	return _generic_foo<46697, bool, void*>(context);  // SkyrimSE.exe+7D5EC0
}

// TODO: replace
CombatBehaviorTreeNode* tmp_wrap_to_randomnode_0(const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	NodeArray cont;
	wrap_to_randomnode_0(cont, name, extradata, node);
	return cont[0]->parent;
}

// TODO: replace
CombatBehaviorTreeNode* tmp_wrap_to_valuenode_1(const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	NodeArray cont;
	wrap_to_valuenode_1(cont, name, extradata, node);
	return cont[0];
}

// TODO: replace
CombatBehaviorTreeNode* tmp_wrap_to_valuenode_10(const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	NodeArray cont;
	wrap_to_valuenode_10(cont, name, extradata, node);
	return cont[0];
}

// TODO: replace
CombatBehaviorTreeNode* tmp_wrap_to_valuenode_14(const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node)
{
	NodeArray cont;
	wrap_to_valuenode_14(cont, name, extradata, node);
	return cont[0];
}

void CombatBehaviorTreeBlock__ctor(void* self)
{
	NodeArray BlockSelectorItems;
	TreeCtors_extradata extradata = { nullptr, nullptr };
	
	BlockSelectorItems.clear();
	extradata.func1 = Block::wantbash;
	BlockSelectorItems.push_back(tmp_wrap_to_valuenode_10("Bash", &extradata, NodeBlockBash::createnew()));
	extradata.func1 = Block::wantblock;
	BlockSelectorItems.push_back(tmp_wrap_to_valuenode_10("Block", &extradata, NodeBlockBlockAttack::createnew()));
	extradata.func1 = Block::get_BashAfterBlock_prop;
	BlockSelectorItems.back()->add_FirstChildren(tmp_wrap_to_randomnode_0("Conter Attack", &extradata, NodeBlockBash::createnew()));
	extradata.func1 = (void*)1;
	BlockSelectorItems.push_back(tmp_wrap_to_valuenode_1("Idle", &extradata, NodeIdle::createnew()));
	
	auto node_BlockContext = createnew_withname(NodeBlockContext::createnew(), "Block Context");
	
	node_BlockContext
		->add_FirstChildren(createnew_withname(NodeRepeat::createnew(), "Block Repeat"))
		->add_FirstChildren(createnew_withname(NodeConditionalChildSelector::createnew(), "Block Selector"))
		->init_childrens_parents(BlockSelectorItems);
	
	CombatBehaviorTree__init_nodes(self, node_BlockContext);
}

void CombatBehaviorTreeMelee__ctor(void* self)
{
	NodeArray AttackSelectorItems;
	TreeCtors_extradata extradata = { nullptr, nullptr };

	AttackSelectorItems.clear();
	extradata.func1 = Attack::dontwant;
	auto nodeIdle = NodeIdle::createnew();
	nodeIdle->flt_28 = 1.0f;
	AttackSelectorItems.push_back(tmp_wrap_to_valuenode_10("Idle", &extradata, nodeIdle));
	extradata.func1 = (void*)1;
	AttackSelectorItems.push_back(tmp_wrap_to_valuenode_1("Attack", &extradata, NodeMeleeAttack::createnew()));

	auto node_MeleeContext = createnew_withname(NodeMeleeContext::createnew(), "Melee Context");

	node_MeleeContext
		->add_FirstChildren(createnew_withname(NodeRepeat::createnew(), "Atack Repeat"))
		->add_FirstChildren(createnew_withname(NodeConditionalChildSelector::createnew(), "Atack Selector"))
		->init_childrens_parents(AttackSelectorItems);

	CombatBehaviorTree__init_nodes(self, node_MeleeContext);
}

void CombatBehaviorTreeCloseMovement__ctor_test(void* self)
{
	NodeArray ActionTypes, OutOfRange, OutOfposition, InRange;
	TreeCtors_extradata extradata = { nullptr, nullptr };

	extradata.func1 = Movement::Advance::should_fallback_to_ranged;
	OutOfRange.push_back(tmp_wrap_to_conditional_2("FallbackToRanged", &extradata, NodeCloseMovementFallbackToRanged::createnew()));
	OutOfRange.push_back(createnew_withname(NodeCloseMovementAdvance::createnew(), "Advance"));
	OutOfRange.push_back(createnew_withname(NodeIdle::createnew(), "Idle"));
	extradata.func1 = Movement::is_outofrange;
	ActionTypes.push_back(tmp_wrap_to_valuenode_14("Out of Range Movement Selector", &extradata, NodeNextChildSelector::createnew()));
	ActionTypes.back()->init_childrens_parents(OutOfRange);

	//OutOfposition.push_back(createnew_withname(NodeCloseMovementSurround::createnew(), "Surround"));
	//OutOfposition.push_back(createnew_withname(NodeCloseMovementReposition::createnew(), "Reposition"));
	OutOfposition.push_back(createnew_withname(NodeIdle::createnew(), "Idle"));
	extradata.func1 = CombatBehaviorContextCloseMovement__smth_rotation;
	ActionTypes.push_back(tmp_wrap_to_valuenode_14("Out of Position Movement Selector", &extradata, NodeNextChildSelector::createnew()));
	ActionTypes.back()->init_childrens_parents(OutOfposition);

	//InRange.push_back(createnew_withname(NodeCloseMovementBackoff::createnew(), "Backoff"));
	InRange.push_back(createnew_withname(NodeIdle::createnew(), "Idle"));
	extradata.func1 = (void*)1;
	ActionTypes.push_back(tmp_wrap_to_valuenode_1("In Range Movement Selector", &extradata, NodeNextChildSelector::createnew()));
	ActionTypes.back()->init_childrens_parents(InRange);

	auto node_CloseMovementContext = createnew_withname(NodeCloseMovementContext::createnew(), "Close Movement Context");

	node_CloseMovementContext
		->add_FirstChildren(createnew_withname(NodeRepeat::createnew(), "Movement Repeat"))
		->add_FirstChildren(createnew_withname(NodeConditionalChildSelector::createnew(), "Movement Selector"))
		->init_childrens_parents(ActionTypes);

	CombatBehaviorTree__init_nodes(self, node_CloseMovementContext);
}

void CombatBehaviorTreeCloseMovement__ctor(void* self)
{
	NodeArray MovementSelectorItems, SecondLevelItems;
	TreeCtors_extradata extradata = { nullptr, nullptr };

	SecondLevelItems.clear();
	extradata.func1 = Movement::Advance::should_fallback_to_ranged;
	SecondLevelItems.push_back(tmp_wrap_to_conditional_2("FallbackToRanged", &extradata, NodeCloseMovementFallbackToRanged::createnew()));
	SecondLevelItems.push_back(NodeCloseMovementAdvance::createnew()->set_name("Advance"));
	extradata.func1 = (void*)1;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_1("Idle", &extradata, NodeIdle::createnew()));
	extradata.func1 = Movement::is_outofrange;
	MovementSelectorItems.push_back(tmp_wrap_to_valuenode_14("Out of Range Movement Selector", &extradata, NodeNextChildSelector::createnew()));
	MovementSelectorItems.back()->init_childrens_parents(SecondLevelItems);

	SecondLevelItems.clear();
	extradata.func1 = Movement::Fallback::should;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_10("Fallback", &extradata, NodeCloseMovementFallback::createnew()));
	extradata.func1 = Movement::Circle::should_danger;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_10("Circle", &extradata, NodeCloseMovementCircle::createnew()));
	extradata.func1 = (void*)1;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_1("Idle", &extradata, NodeIdle::createnew()));
	extradata.func1 = Movement::is_indanger;
	MovementSelectorItems.push_back(tmp_wrap_to_valuenode_10("On danger Movement Selector", &extradata, NodeConditionalChildSelector::createnew()));
	MovementSelectorItems.back()->init_childrens_parents(SecondLevelItems);

	SecondLevelItems.clear();
	extradata.func1 = Movement::Surround::should;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_10("Surround", &extradata, NodeCloseMovementSurround::createnew()));
	extradata.func1 = (void*)1;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_1("Idle", &extradata, NodeIdle::createnew()));
	extradata.func1 = Movement::low_stamina;
	MovementSelectorItems.push_back(tmp_wrap_to_valuenode_10("Low stamina Movement Selector", &extradata, NodeConditionalChildSelector::createnew()));
	MovementSelectorItems.back()->init_childrens_parents(SecondLevelItems);

	SecondLevelItems.clear();
	extradata.func1 = Movement::Fallback::should_normal;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_10("Fallback", &extradata, NodeCloseMovementFallback::createnew()));
	extradata.func1 = Movement::Circle::should_normal;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_10("Circle", &extradata, NodeCloseMovementCircle::createnew()));
	extradata.func1 = Movement::Advance::should;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_10("Advance", &extradata, NodeCloseMovementAdvance::createnew()));
	extradata.func1 = (void*)1;
	SecondLevelItems.push_back(tmp_wrap_to_valuenode_1("Idle", &extradata, NodeIdle::createnew()));
	extradata.func1 = (void*)1;
	MovementSelectorItems.push_back(tmp_wrap_to_valuenode_1("Normal Movement Selector", &extradata, NodeConditionalChildSelector::createnew()));
	MovementSelectorItems.back()->init_childrens_parents(SecondLevelItems);


	auto node_CloseMovementContext = createnew_withname(NodeCloseMovementContext::createnew(), "Close Movement Context");

	node_CloseMovementContext
		->add_FirstChildren(createnew_withname(NodeRepeat::createnew(), "Movement Repeat"))
		->add_FirstChildren(createnew_withname(NodeConditionalChildSelector::createnew(), "Movement Selector"))
		->init_childrens_parents(MovementSelectorItems);

	CombatBehaviorTree__init_nodes(self, node_CloseMovementContext);
}

