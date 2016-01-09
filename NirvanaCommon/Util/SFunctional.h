#ifndef UTIL_SFUNCTIONAL_H_
#define UTIL_SFUNCTIONAL_H_

#include "PCH.h"

//--------------------------------------------------
// Functional programming?
//--------------------------------------------------
  template <typename Collection, typename unop>
void for_each(Collection col, unop op)
{
  std::for_each(col.begin(),col.end(),op);
}

  template <typename Collection, typename unop>
Collection FMap(Collection col,unop op)
{
  std::transform(col.begin(),col.end(),col.begin(),op);
  return col;
}

  template <typename Collection, typename Predicate>
Collection FFilterNot(Collection col,Predicate predicate)
{
  col.erase(std::remove_if(col.begin(),col.end(),predicate),col.end());
  return col;
}

  template <typename Collection, typename Predicate>
Collection FFilter(Collection col,Predicate predicate)
{
  //capture the predicate in order to be used inside function
  return FFilterNot(col,[predicate](typename Collection::value_type i) {return ! predicate(i);});
}

  template <typename Collection, typename InitValType, typename Predicate>
InitValType FFold(Collection col,InitValType init,Predicate predicate)
{
  std::accumulate(col.begin(),col.end(),init,predicate);
  return init;
}


#endif /* UTIL_SFUNCTIONAL_H_ */
