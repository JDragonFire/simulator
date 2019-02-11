#pragma once
#include <memory>
#include <stdexcept>

class NodeSpec
{
public:
	NodeSpec() = default;

	NodeSpec(const double (&table)[29]) : value_table{table} {}

	NodeSpec(int specNum)
	{
		switch (specNum)
		{
		case 1:
			value_table = spec1_table;
			break;
		case 2:
			value_table = spec2_table;
			break;
		default:
			throw std::out_of_range("");
		}
	}

	const double* value_table;

	NodeSpec(const NodeSpec& other) : value_table{ other.value_table }
	{
	}

	NodeSpec& operator =(const NodeSpec&)
	{
		return *this;
	}
	NodeSpec& operator =(NodeSpec&&) noexcept
	{
		return *this;
	}

	static const NodeSpec spec1;
	static const NodeSpec spec2;
private:
	static const double spec1_table[29];
	static const double spec2_table[29];
};



