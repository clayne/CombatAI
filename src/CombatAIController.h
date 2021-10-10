#pragma once
#include <cstdint>

class CombatBehaviorTreeNode;

class AI_Tree_Control
{
public:
	struct AI_Tree_Control_ActionStack
	{
		struct ActionStack_entry
		{
			CombatBehaviorTreeNode* node_0;
			int num_8;
			int mb_pad_C;
		};

		uint32_t capacity_n_flags;
		int mb_pad_4;
		ActionStack_entry data[16];
		int size;
		int mb_pad_10C;
	};

	struct AI_Tree_Control_anotherControl
	{
		AI_Tree_Control* control;
		unsigned int c;
		char __pad_C[4];
	};

	enum Control_AI_States
	{
		switched_to_parent,
		processing,
		switched_to_children
	};

	char* ptr;
	unsigned int capacity;
	unsigned int size_C;
	AI_Tree_Control_ActionStack action_stack;
	unsigned int size_120;
	int field_124;
	AI_Tree_Control_anotherControl another_control;
	CombatBehaviorTreeNode* cur_node;
	CombatBehaviorTreeNode* node_another;
	int mb_notInterrupted;
	Control_AI_States current_state;
	char field_150;
};
