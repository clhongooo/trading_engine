#ifndef UTIL_SFUNCTIONAL_H_
#define UTIL_SFUNCTIONAL_H_

#include "PCH.h"
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

//--------------------------------------------------
// Functional programming?
//--------------------------------------------------
  template <typename Collection, typename UnOp>
void FForEach(const Collection & col, UnOp op)
{
  std::for_each(col.begin(),col.end(),op);
}

  template <typename Collection>
void FReverse(Collection & col)
{
  std::reverse(col.begin(),col.end());
}

//   template <typename CollectionIn, typename CollectionOut, typename InType, typename UnOp>
// void FMap(CollectionIn colIn, CollectionOut colOut, UnOp op)
// {
//   // //--------------------------------------------------
//   // // CAUTION: UNTESTED
//   // //--------------------------------------------------
//   // std::transform(colIn.begin(),colIn.end(),back_inserter(colOut),op);
//   colOut.clear();
//   FForEach(colIn, [&](const InType i) {
//     colOut.push_back(op(i));
//   });
// }

  template <typename Collection, typename Predicate>
Collection FFilterNot(const Collection & col,Predicate predicate)
{
  Collection col2 = col;
  col2.erase(std::remove_if(col2.begin(),col2.end(),predicate),col2.end());
  return col2;
}

  template <typename Collection, typename Predicate>
Collection FFilter(const Collection & col,Predicate predicate)
{
  //capture the predicate in order to be used inside function
  Collection col2 = col;
  return FFilterNot(col2,[predicate](typename Collection::value_type i) {return ! predicate(i);});
  return col2;
}

  template <typename Collection, typename InitValType, typename BinOp>
InitValType FFold(const Collection & col,InitValType init,BinOp op)
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


  template <typename Collection>
double FSum(const Collection & col)
{
  // return std::accumulate(col.begin(),col.end(),0.0);
  double d = 0.0;
  for (typename Collection::const_iterator it = col.begin(); it != col.end(); ++it) d += (double)(*it);
  return d;
}

template <typename T>
T GetOrElse_(boost::optional<T> o, T val) {
  if (o) return o.get();
  else return val;
}

template <typename TK, typename TV>
class SMap {
  private:
    map<TK,TV> _map;
  public:
    boost::optional<TV> Get(const TK k) const
    {
      typename map<TK,TV>::const_iterator it = _map.find(k);
      if (it == _map.end()) return boost::optional<TV>();
      return boost::optional<TV>(it->second);
    }
    TV GetOrElse(const TK k, const TV defaultval)
    {
      return GetOrElse_(Get(k),defaultval);
    }
    typename map<TK,TV>::iterator GetIterBegin()
    {
      return _map.begin();
    }
    typename map<TK,TV>::iterator GetIterEnd()
    {
      return _map.end();
    }
    void AddOrUpdate(const TK k, const TV v)
    {
      _map[k] = v;
    }
    void Remove(const TK k)
    {
      typename map<TK,TV>::iterator it = _map.find(k);
      if (it == _map.end()) return;
      _map.erase(it);
    }
    bool Contains(const TK k) const
    {
      typename map<TK,TV>::const_iterator it = _map.find(k);
      if (it == _map.end()) return false;
      else return true;
    }
    vector<std::pair<TK,TV> > ToVector() const
    {
      vector<std::pair<TK,TV> > vOut;
      FForEach(_map,[&](const std::pair<TK,TV> p) { vOut.push_back(p); });
      return vOut;
    }
    void FromVectorReplaceOrAdd(vector<std::pair<TK,TV> > vIn)
    {
      FForEach(vIn,[&](const std::pair<TK,TV> p) { _map[p.first] = p.second; });
    }
    void FromVectorTotalReplace(vector<std::pair<TK,TV> > vIn)
    {
      _map.clear();
      FromVectorReplaceOrAdd(vIn);
    }
};

template <typename TK, typename TV>
class SMapThreadSafe {
  private:
    map<TK,TV> _map;
    boost::shared_mutex _mutex;
    void FromVectorReplaceOrAddNoLock(vector<std::pair<TK,TV> > vIn)
    {
      FForEach(vIn,[&](const std::pair<TK,TV> p) { _map[p.first] = p.second; });
    }
  public:
    boost::optional<TV> Get(const TK k)
    {
      boost::shared_lock<boost::shared_mutex> lock(_mutex);
      typename map<TK,TV>::const_iterator it = _map.find(k);
      if (it == _map.end()) return boost::optional<TV>();
      return boost::optional<TV>(it->second);
    }
    TV GetOrElse(const TK k, const TV defaultval)
    {
      boost::shared_lock<boost::shared_mutex> lock(_mutex);
      return GetOrElse_(Get(k),defaultval);
    }
    void AddOrUpdate(const TK k, const TV v)
    {
      boost::unique_lock<boost::shared_mutex> lock(_mutex);
      _map[k] = v;
    }
    void Remove(const TK k)
    {
      boost::unique_lock<boost::shared_mutex> lock(_mutex);
      typename map<TK,TV>::iterator it = _map.find(k);
      if (it == _map.end()) return;
      _map.erase(it);
    }
    bool Contains(const TK k)
    {
      boost::shared_lock<boost::shared_mutex> lock(_mutex);
      typename map<TK,TV>::iterator it = _map.find(k);
      if (it == _map.end()) return false;
      else return true;
    }
    vector<std::pair<TK,TV> > ToVector()
    {
      boost::shared_lock<boost::shared_mutex> lock(_mutex);
      vector<std::pair<TK,TV> > vOut;
      FForEach(_map,[&](const std::pair<TK,TV> p) { vOut.push_back(p); });
      return vOut;
    }
    void FromVectorReplaceOrAdd(vector<std::pair<TK,TV> > vIn)
    {
      boost::unique_lock<boost::shared_mutex> lock(_mutex);
      FForEach(vIn,[&](const std::pair<TK,TV> p) { _map[p.first] = p.second; });
    }
    void FromVectorTotalReplace(vector<std::pair<TK,TV> > vIn)
    {
      boost::unique_lock<boost::shared_mutex> lock(_mutex);
      _map.clear();
      FromVectorReplaceOrAddNoLock(vIn);
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
    boost::optional<TV> Get(const TK1 k1, const TK2 k2)
    {
      typename map<TK1,map<TK2,TV> >::iterator it = _map.find(k1);
      if (it == _map.end()) return boost::optional<TV>();

      map<TK2,TV> & m2 = it->second;
      typename map<TK2,TV>::iterator it2 = m2.find(k2);
      if (it2 == m2.end()) return boost::optional<TV>();
      return boost::optional<TV>(it2->second);
    }
    TV GetOrElse(const TK1 k1, const TK2 k2, TV defaultval)
    {
      return GetOrElse_(Get(k1,k2),defaultval);
    }
    void AddOrUpdate(const TK1 k1, const TK2 k2, const TV v)
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
    boost::optional<TV> Get(const TK k)
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


template <typename A, typename B>
class Tuple2 {
  private:
    A m_a;
    B m_b;
  public:
    Tuple2(A a, B b)
    {
      m_a = a;
      m_b = b;
    }
    bool operator<(const Tuple2 & rhs) const { return this->m_a < rhs.m_a; }
    Tuple2 & operator=(const Tuple2 & tup)
    { if (this == &tup) return *this;
      m_a = tup.m_a;
      m_b = tup.m_b;
      return *this;
    }
    A _1() const { return m_a; }
    B _2() const { return m_b; }
    string ToString() const
    {
      return "(" + boost::lexical_cast<string>(m_a) + "," + boost::lexical_cast<string>(m_b) + ")";
    }
};

template <typename A, typename B, typename C>
class Tuple3 {
  private:
    A m_a;
    B m_b;
    C m_c;
  public:
    Tuple3(A a, B b, C c)
    {
      m_a = a;
      m_b = b;
      m_c = c;
    }
    bool operator<(const Tuple3 & rhs) const { return this->m_a < rhs.m_a; }
    Tuple3 & operator=(const Tuple3 & tup)
    { if (this == &tup) return *this;
      m_a = tup.m_a;
      m_b = tup.m_b;
      m_c = tup.m_c;
      return *this;
    }
    A _1() const { return m_a; }
    B _2() const { return m_b; }
    C _3() const { return m_c; }
    string ToString() const
    {
      return "(" + boost::lexical_cast<string>(m_a) + "," + boost::lexical_cast<string>(m_b) + "," + boost::lexical_cast<string>(m_c) + ")";
    }
};

template <typename A, typename B, typename C, typename D>
class Tuple4 {
  private:
    A m_a;
    B m_b;
    C m_c;
    D m_d;
  public:
    Tuple4(A a, B b, C c, D d)
    {
      m_a = a;
      m_b = b;
      m_c = c;
      m_d = d;
    }
    bool operator<(const Tuple4 & rhs) const { return this->m_a < rhs.m_a; }
    Tuple4 & operator=(const Tuple4 & tup)
    { if (this == &tup) return *this;
      m_a = tup.m_a;
      m_b = tup.m_b;
      m_c = tup.m_c;
      m_d = tup.m_d;
      return *this;
    }
    A _1() const { return m_a; }
    B _2() const { return m_b; }
    C _3() const { return m_c; }
    D _4() const { return m_d; }
    string ToString() const
    {
      return "(" + boost::lexical_cast<string>(m_a) + "," + boost::lexical_cast<string>(m_b) + "," + boost::lexical_cast<string>(m_c) + "," + boost::lexical_cast<string>(m_d) + ")";
    }
};

#endif /* UTIL_SFUNCTIONAL_H_ */
