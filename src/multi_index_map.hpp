// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2018, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_MULTI_INDEX_MAP_HPP_
#define POSEIDON_MULTI_INDEX_MAP_HPP_

/*

定义时需要指定索引。

typedef ::std::pair<int, ::std::string> Item;

MULTI_INDEX_MAP(Container, Item,
	UNIQUE_MEMBER_INDEX(first)
	MULTI_MEMBER_INDEX(second)
	SEQUENCED_INDEX()
);

基本用法和 ::std::map 类似，只是 find, count, lower_bound, upper_bound, equal_range 等
成员函数需要带一个非类型模板参数，指定使用第几个索引。

Container c;
c.insert(Item(1, "abc"));
c.insert(Item(2, "def"));
::std::cout <<c.find<0>(1)->second <<::std::endl;   // "abc";
assert(c.upper_bound<1>("zzz") == c.end<1>());  // 通过。

*/

#include "cxx_ver.hpp"
#include "cxx_util.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#define MULTI_INDEX_MAP(Class_name_, Value_type_, indices_)	\
	class Class_name_ {	\
	private:	\
		template<typename ValT>	\
		class KeySetter {	\
		private:	\
			ValT m_val;	\
			\
		public:	\
			KeySetter(::Poseidon::Move<ValT> val)	\
				: m_val(STD_MOVE_IDN(val))	\
			{ }	\
			\
		public:	\
			template<typename TargetT>	\
			void operator()(TargetT &target) NOEXCEPT {	\
				target = STD_MOVE(m_val);	\
			}	\
		};	\
		\
	public:	\
		typedef Value_type_ value_type;	\
		typedef ::boost::multi_index::multi_index_container<value_type, ::boost::multi_index::indexed_by<STRIP_FIRST(void indices_)> > base_container;	\
		\
		typedef typename base_container::const_iterator const_iterator;	\
		typedef typename base_container::iterator iterator;	\
		typedef typename base_container::const_reverse_iterator const_reverse_iterator;	\
		typedef typename base_container::reverse_iterator reverse_iterator;	\
		\
	private:	\
		base_container m_elements;	\
		\
	public:	\
		template<unsigned IndexIdT>	\
		const typename base_container::nth_index<IndexIdT>::type &get_index() const {	\
			return m_elements.get<IndexIdT>();	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type &get_index(){	\
			return m_elements.get<IndexIdT>();	\
		}	\
		\
		const_iterator begin() const {	\
			return m_elements.begin();	\
		}	\
		iterator begin(){	\
			return m_elements.begin();	\
		}	\
		const_iterator end() const {	\
			return m_elements.end();	\
		}	\
		iterator end(){	\
			return m_elements.end();	\
		}	\
		\
		const_reverse_iterator rbegin() const {	\
			return m_elements.rbegin();	\
		}	\
		reverse_iterator rbegin(){	\
			return m_elements.rbegin();	\
		}	\
		const_reverse_iterator rend() const {	\
			return m_elements.rend();	\
		}	\
		reverse_iterator rend(){	\
			return m_elements.rend();	\
		}	\
		\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::const_iterator begin() const {	\
			return get_index<IndexIdT>().begin();	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::iterator begin(){	\
			return get_index<IndexIdT>().begin();	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::const_iterator end() const {	\
			return get_index<IndexIdT>().end();	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::iterator end(){	\
			return get_index<IndexIdT>().end();	\
		}	\
		\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::const_reverse_iterator rbegin() const {	\
			return get_index<IndexIdT>().rbegin();	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::reverse_iterator rbegin(){	\
			return get_index<IndexIdT>().rbegin();	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::const_reverse_iterator rend() const {	\
			return get_index<IndexIdT>().rend();	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::reverse_iterator rend(){	\
			return get_index<IndexIdT>().rend();	\
		}	\
		\
		bool empty() const {	\
			return m_elements.empty();	\
		}	\
		::std::size_t size() const {	\
			return m_elements.size();	\
		}	\
		void clear(){	\
			m_elements.clear();	\
		}	\
		\
		void swap(Class_name_ &rhs) NOEXCEPT {	\
			m_elements.swap(rhs.m_elements);	\
		}	\
		\
		::std::pair<iterator, bool> insert(const value_type &val){	\
			return m_elements.insert(val);	\
		}	\
	ENABLE_IF_CXX11(	\
		::std::pair<iterator, bool> insert(value_type &&val){	\
			return m_elements.insert(::std::move(val));	\
		}	\
	)	\
		iterator insert(iterator pos, const value_type &val){	\
			return m_elements.insert(pos, val);	\
		}	\
	ENABLE_IF_CXX11(	\
		iterator insert(iterator pos, value_type &&val){	\
			return m_elements.insert(pos, ::std::move(val));	\
		}	\
	)	\
		\
		iterator erase(const_iterator pos){	\
			return m_elements.erase(pos);	\
		}	\
		iterator erase(const_iterator from, const_iterator to){	\
			return m_elements.erase(from, to);	\
		}	\
		\
		bool replace(iterator pos, const value_type &val){	\
			return m_elements.replace(pos, val);	\
		}	\
	ENABLE_IF_CXX11(	\
		bool replace(iterator pos, value_type &&val){	\
			return m_elements.replace(pos, ::std::move(val));	\
		}	\
	)	\
		\
		template<unsigned IndexIdT>	\
		::std::pair<typename base_container::nth_index<IndexIdT>::type::iterator, bool>	insert(const value_type &val){	\
			return get_index<IndexIdT>().insert(val);	\
		}	\
	ENABLE_IF_CXX11(	\
		template<unsigned IndexIdT>	\
		::std::pair<typename base_container::nth_index<IndexIdT>::type::iterator, bool> insert(value_type &&val){	\
			return get_index<IndexIdT>().insert(::std::move(val));	\
		}	\
	)	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::iterator insert(typename base_container::nth_index<IndexIdT>::type::iterator hint, const value_type &val){	\
			return get_index<IndexIdT>().insert(hint, val);	\
		}	\
	ENABLE_IF_CXX11(	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::iterator insert(typename base_container::nth_index<IndexIdT>::type::iterator hint, value_type &&val){	\
			return get_index<IndexIdT>().insert(hint, ::std::move(val));	\
		}	\
	)	\
		\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::iterator erase(typename base_container::nth_index<IndexIdT>::type::const_iterator pos){	\
			return get_index<IndexIdT>().erase(pos);	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::iterator erase(typename base_container::nth_index<IndexIdT>::type::const_iterator from, typename base_container::nth_index<IndexIdT>::type::const_iterator to){	\
			return get_index<IndexIdT>().erase(from, to);	\
		}	\
		template<unsigned IndexIdT>	\
		std::size_t erase(const typename base_container::nth_index<IndexIdT>::type::key_type &key){	\
			return get_index<IndexIdT>().erase(key);	\
		}	\
		\
		template<unsigned IndexIdT>	\
		bool replace(typename base_container::nth_index<IndexIdT>::type::iterator pos, const value_type &val){	\
			return get_index<IndexIdT>().replace(pos, val);	\
		}	\
	ENABLE_IF_CXX11(	\
		template<unsigned IndexIdT>	\
		bool replace(typename base_container::nth_index<IndexIdT>::type::iterator pos, value_type &&val){	\
			return get_index<IndexIdT>().replace(pos, ::std::move(val));	\
		}	\
	)	\
		\
		template<unsigned ToIndexIdT>	\
		typename base_container::nth_index<ToIndexIdT>::type::const_iterator project(const_iterator pos) const {	\
			return m_elements.project<ToIndexIdT>(pos);	\
		}	\
		template<unsigned ToIndexIdT>	\
		typename base_container::nth_index<ToIndexIdT>::type::iterator project(iterator pos){	\
			return m_elements.project<ToIndexIdT>(pos);	\
		}	\
		\
		template<unsigned ToIndexIdT, unsigned FromIndexIdT>	\
		typename base_container::nth_index<ToIndexIdT>::type::const_iterator project(typename base_container::nth_index<FromIndexIdT>::type::const_iterator pos) const {	\
			return m_elements.project<ToIndexIdT>(pos);	\
		}	\
		template<unsigned ToIndexIdT, unsigned FromIndexIdT>	\
		typename base_container::nth_index<ToIndexIdT>::type::iterator project(typename base_container::nth_index<FromIndexIdT>::type::iterator pos){	\
			return m_elements.project<ToIndexIdT>(pos);	\
		}	\
		\
		template<unsigned IndexIdToSetT>	\
		bool set_key(iterator pos, typename base_container::nth_index<IndexIdToSetT>::type::key_type key){	\
			return get_index<IndexIdToSetT>().modify_key(m_elements.project<IndexIdToSetT>(pos), KeySetter<VALUE_TYPE(key)>(STD_MOVE(key)));	\
		}	\
		template<unsigned IndexIdT, unsigned IndexIdToSetT>	\
		bool set_key(typename base_container::nth_index<IndexIdT>::type::iterator pos, typename base_container::nth_index<IndexIdToSetT>::type::key_type key){	\
			return get_index<IndexIdToSetT>().modify_key(m_elements.project<IndexIdToSetT>(pos), KeySetter<VALUE_TYPE(key)>(STD_MOVE(key)));	\
		}	\
		\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::const_iterator find(const typename base_container::nth_index<IndexIdT>::type::key_type &key) const {	\
			return get_index<IndexIdT>().find(key);	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::iterator find(const typename base_container::nth_index<IndexIdT>::type::key_type &key){	\
			return get_index<IndexIdT>().find(key);	\
		}	\
		\
		template<unsigned IndexIdT>	\
		::std::size_t count(const typename base_container::nth_index<IndexIdT>::type::key_type &key) const {	\
			return get_index<IndexIdT>().count(key);	\
		}	\
		\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::const_iterator lower_bound(const typename base_container::nth_index<IndexIdT>::type::key_type &key) const {	\
			return get_index<IndexIdT>().lower_bound(key);	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::iterator lower_bound(const typename base_container::nth_index<IndexIdT>::type::key_type &key){	\
			return get_index<IndexIdT>().lower_bound(key);	\
		}	\
		\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::const_iterator upper_bound(const typename base_container::nth_index<IndexIdT>::type::key_type &key) const {	\
			return get_index<IndexIdT>().upper_bound(key);	\
		}	\
		template<unsigned IndexIdT>	\
		typename base_container::nth_index<IndexIdT>::type::iterator upper_bound(const typename base_container::nth_index<IndexIdT>::type::key_type &key){	\
			return get_index<IndexIdT>().upper_bound(key);	\
		}	\
		\
		template<unsigned IndexIdT>	\
		::std::pair<typename base_container::nth_index<IndexIdT>::type::const_iterator, typename base_container::nth_index<IndexIdT>::type::const_iterator> equal_range(const typename base_container::nth_index<IndexIdT>::type::key_type &key) const {	\
			return get_index<IndexIdT>().equal_range(key);	\
		}	\
		template<unsigned IndexIdT>	\
		::std::pair<typename base_container::nth_index<IndexIdT>::type::iterator, typename base_container::nth_index<IndexIdT>::type::iterator> equal_range(const typename base_container::nth_index<IndexIdT>::type::key_type &key){	\
			return get_index<IndexIdT>().equal_range(key);	\
		}	\
	}

#define UNIQUE_INDEX(...)                   , ::boost::multi_index::ordered_unique< ::boost::multi_index::identity<value_type>, ## __VA_ARGS__>
#define UNIQUE_MEMBER_INDEX(member_, ...)   , ::boost::multi_index::ordered_unique< ::boost::multi_index::member<value_type, CV_VALUE_TYPE(DECLREF(value_type).member_), &value_type::member_>, ## __VA_ARGS__>
#define MULTI_INDEX(...)                    , ::boost::multi_index::ordered_non_unique< ::boost::multi_index::identity<value_type>, ## __VA_ARGS__>
#define MULTI_MEMBER_INDEX(member_, ...)    , ::boost::multi_index::ordered_non_unique< ::boost::multi_index::member<value_type, CV_VALUE_TYPE(DECLREF(value_type).member_), &value_type::member_>, ## __VA_ARGS__>
#define SEQUENCED_INDEX()                   , ::boost::multi_index::sequenced<>

#endif
