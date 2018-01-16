// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2018, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_MYSQL_CONNECTION_HPP_
#define POSEIDON_MYSQL_CONNECTION_HPP_

#include "../cxx_ver.hpp"
#include "../cxx_util.hpp"
#include "../uuid.hpp"
#include <string>
#include <cstring>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

namespace Poseidon {
namespace MySql {

class Connection : NONCOPYABLE {
public:
	static boost::shared_ptr<Connection> create(const char *server_addr, boost::uint16_t server_port, const char *user_name, const char *password, const char *schema, bool use_ssl, const char *charset);

public:
	virtual ~Connection();

public:
	virtual void execute_sql_explicit(const char *sql, std::size_t len) = 0;
	virtual void discard_result() NOEXCEPT = 0;

	virtual boost::uint64_t get_insert_id() const = 0;
	virtual bool fetch_row() = 0;

	virtual boost::int64_t get_signed(const char *name) const = 0;
	virtual boost::uint64_t get_unsigned(const char *name) const = 0;
	virtual double get_double(const char *name) const = 0;
	virtual std::string get_string(const char *name) const = 0;
	virtual boost::uint64_t get_datetime(const char *name) const = 0;
	virtual Uuid get_uuid(const char *name) const = 0;
	virtual std::basic_string<unsigned char> get_blob(const char *name) const = 0;

	void execute_sql(const char *sql, std::size_t len){
		execute_sql_explicit(sql, len);
	}
	void execute_sql(const char *sql){
		execute_sql_explicit(sql, std::strlen(sql));
	}
	void execute_sql(const std::string &sql){
		execute_sql_explicit(sql.data(), sql.size());
	}
};

}
}

#endif
