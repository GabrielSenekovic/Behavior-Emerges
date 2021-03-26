#include "BehaviorTree.h"

BehaviorTree::BehaviorTree()
{
}

BehaviorTree::BehaviorTree(BehaviorNode* node): root(node)
{
}

void BehaviorTree::Execute()
{
	root->Execute();
}

BehaviorTree::BehaviorNode::BehaviorNode(NodeType type_in):type(type_in){}

BehaviorTree::BehaviorNode::~BehaviorNode()
{
}

ReturnType BehaviorTree::BehaviorNode::Execute()
{
	return ReturnType();
}

BehaviorTree::Decorator::Decorator(std::vector<BehaviorNode*> children_in): children(children_in), BehaviorNode(NodeType::DECORATOR)
{
}

BehaviorTree::Decorator::~Decorator()
{
}

ReturnType BehaviorTree::Decorator::Execute()
{
	return ReturnType();
}

BehaviorTree::Selector::Selector(std::vector<BehaviorNode*> children_in): children(children_in), BehaviorNode(NodeType::SELECTOR)
{
}

BehaviorTree::Selector::~Selector()
{
}

ReturnType BehaviorTree::Selector::Execute()
{
	for (int i = 0; i < children.size(); i++)
	{
		ReturnType temp = children[i]->Execute();
		if (temp == ReturnType::R_SUCCESS || temp == ReturnType::R_RUNNING)
		{
			return temp;
		}
	}
	return ReturnType::R_FAILURE;
}

BehaviorTree::Leaf::Leaf(std::function<ReturnType()> func_in): func(func_in), BehaviorNode(NodeType::LEAF)
{
}

BehaviorTree::Leaf::~Leaf()
{
}

ReturnType BehaviorTree::Leaf::Execute()
{
	return func();
}

BehaviorTree::Sequencer::Sequencer(std::vector<BehaviorNode*> children_in): children(children_in), BehaviorNode(NodeType::SEQUENCER)
{
}

BehaviorTree::Sequencer::~Sequencer()
{
}

ReturnType BehaviorTree::Sequencer::Execute()
{
	for (int i = 0; i < children.size(); i++)
	{
		ReturnType temp = children[i]->Execute();
		if (temp != ReturnType::R_SUCCESS)
		{
			return temp;
		}
	}
	return ReturnType::R_SUCCESS;
}
