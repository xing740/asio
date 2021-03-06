//
// detail/service_registry.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_SERVICE_REGISTRY_HPP
#define ASIO_DETAIL_SERVICE_REGISTRY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "RCF/external/asio/asio/detail/config.hpp"
#include <typeinfo>
#include "RCF/external/asio/asio/detail/mutex.hpp"
#include "RCF/external/asio/asio/detail/noncopyable.hpp"
#include "RCF/external/asio/asio/io_service.hpp"

#if defined(BOOST_NO_TYPEID)
# if !defined(ASIO_NO_TYPEID)
#  define ASIO_NO_TYPEID
# endif // !defined(ASIO_NO_TYPEID)
#endif // defined(BOOST_NO_TYPEID)

#include "RCF/external/asio/asio/detail/push_options.hpp"

namespace asio {
namespace detail {

#if defined(__GNUC__)
# if (__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)
#  pragma GCC visibility push (default)
# endif // (__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)
#endif // defined(__GNUC__)

template <typename T>
class typeid_wrapper {};

#if defined(__GNUC__)
# if (__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)
#  pragma GCC visibility pop
# endif // (__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)
#endif // defined(__GNUC__)

class service_registry
  : private noncopyable
{
public:
  // Constructor.
  ASIO_DECL service_registry(asio::io_service& o);

  // Destructor.
  ASIO_DECL ~service_registry();

  // Get the service object corresponding to the specified service type. Will
  // create a new service object automatically if no such object already
  // exists. Ownership of the service object is not transferred to the caller.
  template <typename Service>
  Service& use_service();

  // Add a service object. Throws on error, in which case ownership of the
  // object is retained by the caller.
  template <typename Service>
  void add_service(Service* new_service);

  // Check whether a service object of the specified type already exists.
  template <typename Service>
  bool has_service() const;

private:
  // Initialise a service's key based on its id.
  ASIO_DECL static void init_key(
      asio::io_service::service::key& key,
      const asio::io_service::id& id);

#if !defined(ASIO_NO_TYPEID)
  // Initialise a service's key based on its id.
  template <typename Service>
  static void init_key(asio::io_service::service::key& key,
      const asio::detail::service_id<Service>& /*id*/);
#endif // !defined(ASIO_NO_TYPEID)

  // Check if a service matches the given id.
  ASIO_DECL static bool keys_match(
      const asio::io_service::service::key& key1,
      const asio::io_service::service::key& key2);

  // The type of a factory function used for creating a service instance.
  typedef asio::io_service::service* (*factory_type)(asio::io_service&);

  // Factory function for creating a service instance.
  template <typename Service>
  static asio::io_service::service* create(
      asio::io_service& owner);

  // Destroy a service instance.
  ASIO_DECL static void destroy(
      asio::io_service::service* service);

  // Helper class to manage service pointers.
  struct auto_service_ptr;
  friend struct auto_service_ptr;
  struct auto_service_ptr
  {
    asio::io_service::service* ptr_;
    ~auto_service_ptr() { destroy(ptr_); }
  };

  // Get the service object corresponding to the specified service key. Will
  // create a new service object automatically if no such object already
  // exists. Ownership of the service object is not transferred to the caller.
  ASIO_DECL asio::io_service::service* do_use_service(
      const asio::io_service::service::key& key,
      factory_type factory);

  // Add a service object. Returns false on error, in which case ownership of
  // the object is retained by the caller.
  ASIO_DECL void do_add_service(
      const asio::io_service::service::key& key,
      asio::io_service::service* new_service);

  // Check whether a service object with the specified key already exists.
  ASIO_DECL bool do_has_service(
      const asio::io_service::service::key& key) const;

  // Mutex to protect access to internal data.
  mutable asio::detail::mutex mutex_;

  // The owner of this service registry and the services it contains.
  asio::io_service& owner_;

  // The first service in the list of contained services.
  asio::io_service::service* first_service_;
};

} // namespace detail
} // namespace asio

#include "RCF/external/asio/asio/detail/pop_options.hpp"

#include "RCF/external/asio/asio/detail/impl/service_registry.hpp"
#if defined(ASIO_HEADER_ONLY)
# include "RCF/external/asio/asio/detail/impl/service_registry.ipp"
#endif // defined(ASIO_HEADER_ONLY)

#endif // ASIO_DETAIL_SERVICE_REGISTRY_HPP
