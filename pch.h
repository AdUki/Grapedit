////
/// Precompiled header
///

#ifdef __cplusplus

	#include <cstring>
	#include <stdexcept>
	#include <assert.h>
	#include <stdint.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <math.h>
	#include <sys/types.h>

	#include <memory>
	#include <iomanip>
	#include <list>
	#include <map>
	#include <vector>
	#include <set>
	#include <array>
	#include <functional>
	#include <unordered_map>
	#include <unordered_set>
	#include <sstream>
	#include <string>
	#include <limits>

	#include <boost/any.hpp>
	#include <boost/bind.hpp>
	#include <boost/format.hpp>
	#include <boost/lexical_cast.hpp>
	#include <boost/noncopyable.hpp>
	#include <boost/optional.hpp>
	#include <boost/shared_ptr.hpp>
	#include <boost/make_shared.hpp>
	#include <boost/algorithm/string/predicate.hpp>


	#include <QApplication>
	#include <QWidget>
	#include <QObject>
	#include <QDebug>

	#include <lua.hpp>

	#define DECLARE_CLASS_PTR(type) \
	class type; \
	typedef boost::shared_ptr<type> type##Ptr; \
	typedef const boost::shared_ptr<type> type##CPtr; \
	typedef std::weak_ptr<type> type##WPtr;

	#define DECLARE_NS_CLASS_PTR(ns, type) \
	namespace ns { \
		class type; \
		typedef boost::shared_ptr<type> type##Ptr; \
		typedef const boost::shared_ptr<type> type##CPtr; \
		typedef std::weak_ptr<type> type##WPtr; \
	}

	#ifdef _WIN32
		
		// MSVC compiler
		#ifndef NOMINMAX
			#define NOMINMAX
		#endif

		#include <windows.h>
	#endif

#endif
