// weighted_completion_times_1.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>


class wct
{
public:
  wct(const char* input_file);
  long long calc_wct();
  struct weight_len
  {
    int weight;
    int len;
    bool operator<(const weight_len& rhs)
    {
      return (((float)weight/len) < ((float)rhs.weight/rhs.len) || 
        (((float)weight/len) == ((float)rhs.weight/rhs.len) && weight < rhs.weight));
    }    
  };

private:
  int m_jobs_size;
  std::vector<weight_len> m_data;
};

std::istream & operator>>(std::istream & strm, wct::weight_len& data)
{
  strm >> data.weight >> data.len;
  return strm;
}

wct::wct( const char* input_file )
{

  std::fstream fin(input_file);
  if(!fin)
    return;

  fin >> m_jobs_size;
  m_data.reserve(m_jobs_size);
  std::copy(std::istream_iterator<weight_len>(fin), 
    std::istream_iterator<weight_len>(), 
    std::back_inserter(m_data));

  std::make_heap(m_data.begin(), m_data.end());
}

long long wct::calc_wct()
{
  long long weighted_completion_times = 0;
  unsigned  completion_time = 0;
  while(!m_data.empty())
  {
    std::pop_heap(m_data.begin(), m_data.end());
    weight_len wl = m_data.back();
    m_data.pop_back();
    completion_time += wl.len;
    weighted_completion_times += wl.weight * completion_time;
  }
  return weighted_completion_times;
}

int main(int argc, char* argv[])
{
  wct  the_wct(argv[1]);
  std::cout << "weighted_completion_times:" << the_wct.calc_wct() << '\n';
  return 0;
}

