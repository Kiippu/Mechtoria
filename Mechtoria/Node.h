#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Setting.h"
#include "Transform.h"


class RendableNode;
class GameObjectDynamic;
class Node;
//extern std::map<node::renderLayer, std::vector<RendableNode*>> s_rendableNodes;
extern std::vector<std::weak_ptr<GameObjectDynamic>> s_collisionNodes;
static int Unique_ID = 0;

class Node : public std::enable_shared_from_this<Node>
{
public:
	Node(node::type _type = node::type::UNDEFINED)
		: m_uniqueID(Unique_ID++), m_type(_type), m_isInitialized(false), m_parent(nullptr)
	{}
	virtual ~Node() {};
	Node(const Node& _node) = delete;
	Node& operator=(const Node& _otherNode) = delete;

	virtual void Initialization() = 0;
	virtual void Deinitialization() = 0;

	void addChildNode(std::shared_ptr<Node> _node)
	{
		_node->SetParent(this);
		m_childNodes.insert(std::make_pair(_node->GetNodeType(), _node));
	};

	Node* GetParent() const { return m_parent; }

	void RemoveChildNode(std::shared_ptr<Node> _node, node::type _type)
	{
		const auto& itr = m_childNodes.find(_type);
		if (itr != m_childNodes.cend())
		{
			m_childNodes.erase(itr->first);
		}
	};

	const std::weak_ptr<Node> GetChildNode(node::type _type)
	{
		const auto& itr = m_childNodes.find(_type);
		if (itr != m_childNodes.cend())
		{
			return itr->second;
		}
		return std::weak_ptr<Node>();
	};

	const std::map<node::type, std::shared_ptr<Node>>& GetChildNodes() const
	{
		return m_childNodes;
	};

	node::type GetNodeType() const { return m_type; }

	void SetInitialized() { m_isInitialized = true; };
	bool IsInitialized() { return m_isInitialized; }

private:
	void SetParent(Node* _parent) { m_parent = _parent; };

	std::map<node::type,std::shared_ptr<Node>> m_childNodes;
	const uint32_t m_uniqueID;
	const node::type m_type;
	bool m_isInitialized;
	Node* m_parent;
};

class UpdateableNode
{
public:
	virtual void Update() = 0;
};

class RendableNode
{
public:
	RendableNode(node::renderLayer _renderLayer) 
	{
		//s_rendableNodes[_renderLayer].push_back(this);
	}
	virtual void Draw() = 0;
	Transform2D GetWorldTransform() { return m_transform; };
	virtual void SetWorldTransform(Transform2D _transform) { m_transform = _transform; };
private:
	Transform2D m_transform;
};

class DynamicNode : public Node, public  UpdateableNode
{
public:
	DynamicNode(node::type _type)
		: Node(_type) {};
	virtual void Update()
	{
		auto& nodes = GetChildNodes();
		for (auto& node : nodes)
		{
			if (auto update = std::dynamic_pointer_cast<UpdateableNode>(node.second))
			{
				if (!node.second->IsInitialized())
				{
					node.second->Initialization();
					node.second->SetInitialized();
				}
				update->Update();
			}
		}
	}
};

class Visual2DNode : public Node, public  RendableNode
{
public:
	Visual2DNode(node::type _type, node::renderLayer _renderLayer)
		: Node(_type), RendableNode(_renderLayer) {}
	virtual void Draw()
	{
		auto& nodes = GetChildNodes();
		for (auto& node : nodes)
		{
			if (auto visual = std::dynamic_pointer_cast<RendableNode>(node.second))
				visual->Draw();
		}
	}
};

class Visual2DDynamicNode : public Node, public RendableNode, public UpdateableNode
{
public:
	Visual2DDynamicNode(node::type _type, node::renderLayer _renderLayer)
		: Node(_type), RendableNode(_renderLayer) {};
	virtual void Update()
	{
		auto& nodes = GetChildNodes();
		for (auto& node : nodes)
		{
			if (auto update = std::dynamic_pointer_cast<UpdateableNode>(node.second))
			{
				if (!node.second->IsInitialized())
				{
					node.second->Initialization();
					node.second->SetInitialized();
				}
				update->Update();
			}
		}
	}
	virtual void Draw()
	{
		auto& nodes = GetChildNodes();
		for (auto& node : nodes)
		{
			if (auto visual = std::dynamic_pointer_cast<RendableNode>(node.second))
				visual->Draw();
		}
	}

};