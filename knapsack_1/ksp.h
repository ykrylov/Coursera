#pragma once
//#include <istream>
//#include <ostream>
#include <iosfwd>
#include <vector>
#include <algorithm>
#include <stack>

class ksp
{

public:
	ksp(std::istream& input, std::ostream& output);
	~ksp(void);

  void solve();

  typedef std::pair<int, int> weight_value;
  typedef std::vector<weight_value> items_array;
  typedef std::vector<std::vector<int> > capacity_item_table;
private:

  int solve(int k, int j);
  void prepare_output(int k, int j, std::stack<char> & output);

  int                 m_capacity;
  int                 m_number_of_items;
  int				  m_result;
  items_array         m_items;
  capacity_item_table m_values;
  std::stack<char>     m_output;
};

