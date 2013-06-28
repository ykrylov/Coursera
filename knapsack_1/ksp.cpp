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
    m_values[sz].resize(m_capacity + 1, val);
    m_values[sz][0] = 0;
  }
  solve();

  m_result = m_values[m_number_of_items-1][m_capacity];
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
    for(int k = 1; k <= m_capacity; ++k)
      solve(k, j);
}

int ksp::solve( int k, int j )
{
  if(k < 0 || j < 0)
    return 0;

  if(m_values[j][k] != -1)
    return m_values[j][k];
  
  int result;

  if(m_items[j].second > k)
  {
    result= solve(k, j-1);
  }
  else
  {
    result = std::max(solve(k - m_items[j].second, j-1) + m_items[j].first, solve(k, j-1)); 
  }

  m_values[j][k] = result;
  return result;
}

void ksp::prepare_output(int k, int j, std::stack<char> & output )
{
	if (j < 0) return;
	if(j==0)
	{
		output.push(m_values[j][k] ? '1' : '0');
		return;
	}

	int start = j;
	int prev = j-1;

	while (m_values[j][k] == m_values[prev][k] )
	{
		output.push('0');
		if(prev-1 < 0) break;
		--j, --prev;
	}


	if (m_values[j][k] != m_values[prev][k])
	{
		output.push('1');
		prepare_output( k - m_items[j].second, j-1, output);
	}
	else
	{
		output.push(m_values[j][k] ? '1' : '0');
	}


}
