#include "stdafx.h"
#include "ksp.h"
#include <iostream>
#include <iterator>
#include <stack>

std::istream& operator>>(std::istream& strm, ksp::weight_value& val)
{
  strm >> val.first;
  strm >> val.second;
  return strm;
}

ksp::ksp(std::istream& input, std::ostream& output)
{
  input >> m_number_of_items;
  input >> m_capacity;
  input.ignore();

  weight_value wv;
  m_items.reserve(m_number_of_items);
  for(int i = m_number_of_items; i; --i)
  {
    input >> wv;
    m_items.push_back(wv);
  }

  m_values.resize(m_number_of_items);
  for(size_t sz = 0; sz < m_items.size(); ++sz)
  {
    int val = -1; 
    m_values[sz].push_back(std::make_pair(0,0));
	m_values[sz].push_back(std::make_pair(-1,1));
  }

  m_prev.resize(m_capacity+1, 0);
  m_curr.resize(m_capacity+1, -1);
  solve();

  m_result = get_val_table(m_number_of_items-1, m_capacity);
  output << m_result << ' ' << '1' << '\n';

  prepare_output(m_capacity,m_number_of_items-1, m_output);
  while(!m_output.empty())
  {
	  output << m_output.top() << ' ';
	  m_output.pop();
  }

  output << std::endl;
}

ksp::~ksp(void)
{
}

void ksp::solve()
{
  for(int j = 0; j < m_number_of_items; ++j)
  {
	item_prepare_init(j);
	int item_weight = m_items[j].second;
	for(int k = item_weight; k <= m_capacity; ++k)
	{
		init_table(k, j);
	}
	m_prev.swap(m_curr);
  }
  m_prev.clear();
  m_curr.clear();
}

void ksp::init_table(int k, int j)
{
	 m_curr[k] = std::max(m_prev.at(k - m_items[j].second) + m_items[j].first, m_prev[k]); 
	 set_val_table(j,k,m_curr[k]);
}

int ksp::solve( int k, int j )
{
  if(k < 0 || j < 0)
    return 0;

  if(get_val_table(j, k) != -1)
    return get_val_table(j, k);
  
  int result;

  if(m_items[j].second > k)
  {
    result= solve(k, j-1);
  }
  else
  {
    result = std::max(solve(k - m_items[j].second, j-1) + m_items[j].first, solve(k, j-1)); 
  }

  set_val_table(j, k, result);

  return result;
}

void ksp::prepare_output(int k, int j, std::stack<char> & output )
{
	if (j < 0) return;
	if(j==0)
	{
		output.push(get_val_table(j, k) ? '1' : '0');
		return;
	}

	int start = j;
	int prev = j-1;

	while (get_val_table(j, k) == get_val_table(prev, k))
	{
		output.push('0');
		if(prev-1 < 0) break;
		--j, --prev;
	}


	if (get_val_table(j, k) != get_val_table(prev, k))
	{
		output.push('1');
		prepare_output( k - m_items[j].second, j-1, output);
	}
	else
	{
		output.push(get_val_table(j, k) ? '1' : '0');
	}
}

int ksp::get_val_table( int j, int k )
{
	size_t start = m_values[j].size() - 1;
	for( ;m_values[j][start].second > k; --start);
	return m_values[j][start].first;
}

void ksp::set_val_table( int j, int k, int val )
{

	size_t last_elem_index = m_values[j].size() - 2;
	val_start_pair & last = m_values[j][last_elem_index];

	if(last.first != val)
	{
		m_values[j].insert( --m_values[j].end(), std::make_pair(val, k));
	}

	last_elem_index = m_values[j].size() - 2;
	++m_values[j][last_elem_index+1].second;
	if(m_values[j][last_elem_index+1].second > m_capacity)
		m_values[j].pop_back();
}

void ksp::item_prepare_init( int j )
{
	int item_weight = m_items[j].second;
	std::vector<int>::iterator end = m_prev.begin();

	if(item_weight > m_capacity)
	{
	   end = m_prev.end();
	}
	else
	{
		std::advance(end, item_weight);
	}
	
	std::copy(m_prev.begin(), end, m_curr.begin());

	for(int i = 0; i < item_weight && i <= m_capacity; ++i)
	{
		set_val_table(j, i, m_prev[i]);
	}
}
