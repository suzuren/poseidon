// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2018, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_CONFIG_FILE_HPP_
#define POSEIDON_CONFIG_FILE_HPP_

#include "cxx_ver.hpp"
#include "optional_map.hpp"
#include <boost/lexical_cast.hpp>

namespace Poseidon {

class ConfigFile {
private:
	OptionalMap m_contents;

public:
	ConfigFile();
	explicit ConfigFile(const std::string &path);

public:
	void load(const std::string &path);
	int load_nothrow(const std::string &path);
	void save(const std::string &path);

	bool empty() const;
	std::size_t size() const;
	void clear();

	bool get_raw(std::string &val, const char *key) const;
	const std::string &get_raw(const char *key) const;

	std::size_t get_all_raw(boost::container::vector<std::string> &vals, const char *key, bool including_empty = false) const;
	boost::container::vector<std::string> get_all_raw(const char *key, bool including_empty = false) const;

	template<typename T>
	bool get(T &val, const char *key) const {
		const AUTO_REF(str, m_contents.get(key));
		if(str.empty()){
			return false;
		}
		val = boost::lexical_cast<T>(str);
		return true;
	}
	template<typename T>
	T get(const char *key) const {
		T val = VAL_INIT;
		get<T>(val, key);
		return val;
	}

	template<typename T, typename DefValT>
	bool get(T &val, const char *key, const DefValT &def_val) const {
		const AUTO_REF(str, m_contents.get(key));
		if(str.empty()){
			val = static_cast<T>(def_val);
			return false;
		}
		val = boost::lexical_cast<T>(str);
		return true;
	}
	template<typename T, typename DefValT>
	T get(const char *key, const DefValT &def_val) const {
		T val = VAL_INIT;
		get<T, DefValT>(val, key, def_val);
		return val;
	}

	template<typename T>
	std::size_t get_all(boost::container::vector<T> &vals, const char *key, bool including_empty = false) const {
		const AUTO(range, m_contents.range(key));
		vals.reserve(vals.size() + static_cast<std::size_t>(std::distance(range.first, range.second)));
		std::size_t ret = 0;
		for(AUTO(it, range.first); it != range.second; ++it){
			if(it->second.empty()){
				if(!including_empty){
					continue;
				}
				vals.emplace_back();
			} else {
				vals.emplace_back(boost::lexical_cast<T>(it->second));
			}
			++ret;
		}
		return ret;
	}
	template<typename T>
	boost::container::vector<T> get_all(const char *key, bool including_empty = false) const {
		boost::container::vector<T> vals;
		get_all(vals, key, including_empty);
		return vals;
	}

	void swap(ConfigFile &rhs) NOEXCEPT {
		using std::swap;
		swap(m_contents, rhs.m_contents);
	}
};

inline void swap(ConfigFile &lhs, ConfigFile &rhs) NOEXCEPT {
	lhs.swap(rhs);
}

}

#endif
