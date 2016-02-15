#ifndef UTIL_SFUNCTIONAL_H_
#define UTIL_SFUNCTIONAL_H_

#include "PCH.h"

//--------------------------------------------------
// Functional programming?
//--------------------------------------------------
  template <typename Collection, typename UnOp>
void FForEach(Collection col, UnOp op)
{
  std::for_each(col.begin(),col.end(),op);
}

  template <typename Collection>
void FReverse(Collection col)
{
  std::reverse(col.begin(),col.end());
}

  template <typename Collection, typename UnOp>
Collection FMap(Collection col,UnOp op)
{
  Collection col2 = col;
  std::transform(col2.begin(),col2.end(),col2.begin(),op);
  return col2;
}

  template <typename Collection, typename Predicate>
Collection FFilterNot(Collection col,Predicate predicate)
{
  Collection col2 = col;
  col2.erase(std::remove_if(col2.begin(),col2.end(),predicate),col2.end());
  return col2;
}

  template <typename Collection, typename Predicate>
Collection FFilter(Collection col,Predicate predicate)
{
  //capture the predicate in order to be used inside function
  Collection col2 = col;
  return FFilterNot(col2,[predicate](typename Collection::value_type i) {return ! predicate(i);});
  return col2;
}

  template <typename Collection, typename InitValType, typename BinOp>
InitValType FFold(Collection col,InitValType init,BinOp op)
{
  // template< class InputIt, class T, class BinaryOperation >
  //
  // T accumulate( InputIt first, InputIt last, T init, BinaryOperation op );
  //
  //   binary operation function object that will be applied.
  //
  //   The signature of the function should be equivalent to the following:
  //    Ret fun(const Type1 &a, const Type2 &b);
  //
  //    The signature does not need to have const &.
  //    The type Type1 must be such that an object of type T can be implicitly converted to Type1.
  //    The type Type2 must be such that an object of type InputIt can be dereferenced and then implicitly converted to Type2.
  //    The type Ret must be such that an object of type T can be assigned a value of type Ret.

  return std::accumulate(col.begin(),col.end(),init,op);
}

//--------------------------------------------------
// Immutable Option class
//--------------------------------------------------
template <typename T> 
class Option { 
  public:
    Option(const T & content)
    {
      _hasContent = true;
      _content = content;
    }
    Option()
    {
      _hasContent = false;
    }
    bool IsNone() { return !_hasContent; }
    bool IsSome() { return _hasContent; }
    T GetOrElse(const T defaultval)
    {
      if (_hasContent) return _content;
      else return defaultval;
    }
    bool operator==(const Option<T>& o)
    {
      if (this->_hasContent && !o._hasContent) return false;
      if (!this->_hasContent && o._hasContent) return false;
      if (!this->_hasContent && !o._hasContent) return true;

      //--------------------------------------------------
      // both have content
      //--------------------------------------------------
      return (this->_content == o._content);
    }
  private:
    bool _hasContent;
    T    _content;
}; 

template <typename TK, typename TV> 
class SMap {
  private:
    map<TK,TV> _map;
  public:
    Option<TV> Get(const TK k)
    {
      typename map<TK,TV>::iterator it = _map.find(k);
      if (it == _map.end()) return Option<TV>();
      return Option<TV>(it->second);
    }
    TV GetOrElse(const TK k, const TV defaultval)
    {
      return Get(k).GetOrElse(defaultval);
    }
    typename map<TK,TV>::iterator GetIterBegin()
    {
      return _map.begin();
    }
    typename map<TK,TV>::iterator GetIterEnd()
    {
      return _map.end();
    }
    void Add(const TK k, const TV v)
    {
      _map[k] = v;
    }
    bool Contains(const TK k)
    {
      typename map<TK,TV>::iterator it = _map.find(k);
      if (it == _map.end()) return false;
      else return true;
    }
};

template <typename TV> 
class SSet {
  private:
    set<TV> _set;
  public:
    typename set<TV>::iterator GetIterBegin()
    {
      return _set.begin();
    }
    typename set<TV>::iterator GetIterEnd()
    {
      return _set.end();
    }
    void Add(const TV v)
    {
      _set.insert(v);
    }
    void Remove(const TV v)
    {
      _set.erase(v);
    }
    bool Contains(const TV v)
    {
      typename set<TV>::iterator it = _set.find(v);
      if (it == _set.end()) return false;
      else return true;
    }
};


template <typename TK1, typename TK2, typename TV> 
class SMapOfMap {
  private:
    map<TK1,map<TK2,TV> > _map;
  public:
    Option<TV> Get(const TK1 k1, const TK2 k2)
    {
      typename map<TK1,map<TK2,TV> >::iterator it = _map.find(k1);
      if (it == _map.end()) return Option<TV>();

      map<TK2,TV> & m2 = it->second;
      typename map<TK2,TV>::iterator it2 = m2.find(k2);
      if (it2 == m2.end()) return Option<TV>();
      return Option<TV>(it2->second);
    }
    TV GetOrElse(const TK1 k1, const TK2 k2, TV defaultval)
    {
      return Get(k1,k2).GetOrElse(defaultval);
    }
    void Add(const TK1 k1, const TK2 k2, const TV v)
    {
      typename map<TK1,map<TK2,TV> >::iterator it = _map.find(k1);
      if (it == _map.end())
      {
        _map[k1] = map<TK2,TV>();
        it = _map.find(k1);
      }

      map<TK2,TV> & m2 = it->second;
      m2[k2] = v;
    }
    bool Contains(const TK1 k1, const TK2 k2)
    {
      typename map<TK1,map<TK2,TV> >::iterator it = _map.find(k1);
      if (it == _map.end()) return false;

      map<TK2,TV> & m2 = it->second;
      if (m2.find(k2) == m2.end()) return false;
      return true;
    }
};

template <typename TK, typename TV> 
class SMapPersistVal {
  private:
    SMap<TK,TV> _map;
  public:
    Option<TV> Get(const TK k)
    {
      return _map.Get(k);
    }
    TV GetOrElse(const TK k, TV defaultval)
    {
      return _map.GetOrElse(k,defaultval);
    }
    void Add(const TK k, const TV v)
    {
      if (!_map.Contains(k)) _map.Add(k,v);
    }
    bool Contains(const TK k)
    {
      return _map.Contains(k);
    }
};

template <typename TK1, typename TK2, typename TV> 
class SMapOfMapPersistVal {
  private:
    SMapOfMap<TK1,TK2,TV> _map;
  public:
    Option<TV> Get(const TK1 k1, const TK2 k2)
    {
      return _map.Get(k1,k2);
    }
    TV GetOrElse(const TK1 k1, const TK2 k2, TV defaultval)
    {
      return _map.GetOrElse(k1,k2,defaultval);
    }
    void Add(const TK1 k1, const TK2 k2, const TV v)
    {
      if (!_map.Contains(k1,k2)) _map.Add(k1,k2,v);
    }
    bool Contains(const TK1 k1, const TK2 k2)
    {
      return _map.Contains(k1,k2);
    }
};

#endif /* UTIL_SFUNCTIONAL_H_ */
