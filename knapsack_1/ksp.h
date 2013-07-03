#pragma once
//#include <istream>
//#include <ostream>
#include <iosfwd>
#include <vector>
#include <deque>
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
  typedef std::pair<int, int> val_start_pair;
  //typedef std::vector<std::vector<int> > capacity_item_table;
  typedef std::vector<std::vector<val_start_pair> > capacity_item_table;
private:

  int	solve(int k, int j);

  void item_prepare_init( int j );

  void  init_table(int j, int k);
  void	prepare_output(int k, int j, std::stack<char> & output);
  int	get_val_table(int j, int k); 
  void	set_val_table(int j, int k, int val); 

  int                 m_capacity;
  int                 m_number_of_items;
  int				  m_result;
  items_array         m_items;
  capacity_item_table m_values;
  std::stack<char>    m_output;
  std::vector<int>    m_prev;
  std::vector<int>    m_curr;
};

