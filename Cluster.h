#pragma once
#include <vector>
#include "Host.h"

// Contains all Nodes
class Cluster
{
public:
	std::vector<Host> get_all_nodes() const { return nodes_; }
	void add_node(Host& node) { nodes_.push_back(node); }
private:
	std::vector<Host> nodes_;
};

namespace ClusterSimulator
{
	class Cluster
	{
	public:
		const std::vector<Host> get_all_nodes() const { return nodes_; }
		auto begin() { return nodes_.begin(); }
		auto end() { return nodes_.end(); }
		void add_node(const Host& node) { nodes_.push_back(node); }
		int count() const { return nodes_.size(); }
	private:
		std::vector<Host> nodes_;
	};
}

