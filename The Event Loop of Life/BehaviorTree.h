#ifndef BEHAVIORTREE_H
#define BEHAVIORTREE_H

#include <functional>
#include <vector>
#include "Grid.h"
#include "Entity.h"
#include "Enum.h"

class BehaviorTree
{
public:
	enum NodeType
	{
		DECORATOR,
		SELECTOR,
		SEQUENCER,
		LEAF
	};
	struct BehaviorNode
	{
		NodeType type;
		BehaviorNode(NodeType type_in);
		virtual ~BehaviorNode();
		virtual ReturnType Execute();
	};
	struct Decorator :public BehaviorNode
	{
		std::vector<BehaviorNode*> children;
		Decorator(std::vector<BehaviorNode*> children_in);
		~Decorator() override;
		ReturnType Execute() override;
	};
	struct Selector :public BehaviorNode
	{
		std::vector<BehaviorNode*> children;
		Selector(std::vector<BehaviorNode*> children_in);
		~Selector() override;
		ReturnType Execute() override;
	};
	struct Sequencer :public BehaviorNode
	{
		std::vector<BehaviorNode*> children;
		Sequencer(std::vector<BehaviorNode*> children_in);
		~Sequencer() override;
		ReturnType Execute() override;
	};
	struct Leaf :public BehaviorNode
	{
		std::function<ReturnType()> func;
		Leaf(std::function<ReturnType()> func_in);
		~Leaf() override;
		ReturnType Execute() override;
	};
	BehaviorNode* root;
	std::function<ReturnType()> action;
	BehaviorTree();
	BehaviorTree(BehaviorNode* node);

	void Execute();
};

#endif