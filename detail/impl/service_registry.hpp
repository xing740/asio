//
// detail/impl/service_registry.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_IMPL_SERVICE_REGISTRY_HPP
#define ASIO_DETAIL_IMPL_SERVICE_REGISTRY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "RCF/external/asio/asio/detail/push_options.hpp"

namespace asio {
namespace detail {

template <typename Service>
Service& service_registry::use_service()
{
  asio::io_service::service::key key;
  init_key(key, Service::id);
  factory_type factory = &service_registry::create<Service>; //取静态函数地址
  return *static_cast<Service*>(do_use_service(key, factory));
}

template <typename Service>
void service_registry::add_service(Service* new_service)
{
  asio::io_service::service::key key;
  init_key(key, Service::id);
  return do_add_service(key, new_service);
}

template <typename Service>
bool service_registry::has_service() const
{
  asio::io_service::service::key key;
  init_key(key, Service::id);
  return do_has_service(key);
}

#if !defined(ASIO_NO_TYPEID)
template <typename Service>
void service_registry::init_key(asio::io_service::service::key& key,
    const asio::detail::service_id<Service>& /*id*/)
{
  key.type_info_ = &typeid(typeid_wrapper<Service>);
  key.id_ = 0;
}
#endif // !defined(ASIO_NO_TYPEID)

template <typename Service>
asio::io_service::service* service_registry::create(
    asio::io_service& owner)
{
  return new Service(owner);
}

} // namespace detail
} // namespace asio

#include "RCF/external/asio/asio/detail/pop_options.hpp"

#endif // ASIO_DETAIL_IMPL_SERVICE_REGISTRY_HPP
