//
// detail/impl/task_io_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_IMPL_TASK_IO_SERVICE_HPP
#define ASIO_DETAIL_IMPL_TASK_IO_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "RCF/external/asio/asio/detail/call_stack.hpp"
#include "RCF/external/asio/asio/detail/completion_handler.hpp"
#include "RCF/external/asio/asio/detail/fenced_block.hpp"
#include "RCF/external/asio/asio/detail/handler_alloc_helpers.hpp"
#include "RCF/external/asio/asio/detail/handler_invoke_helpers.hpp"

#include "RCF/external/asio/asio/detail/push_options.hpp"

namespace asio {
namespace detail {

template <typename Handler>
void task_io_service::dispatch(Handler handler)
{
  if (call_stack<task_io_service>::contains(this))
  {
    asio::detail::fenced_block b;
    asio_handler_invoke_helpers::invoke(handler, handler);
  }
  else
    post(handler);
}

template <typename Handler>
void task_io_service::post(Handler handler)
{
  // Allocate and construct an operation to wrap the handler.
  typedef completion_handler<Handler> op;
  //ptr 一个handlerptr,voidptr,completion_handlerptr
  typename op::ptr p = { boost::addressof(handler),
    asio_handler_alloc_helpers::allocate(
      sizeof(op), handler), 0 };//传入回调，如果回调还很很多相应的方法，封装成一个class
  p.p = new (p.v) op(handler);

  post_immediate_completion(p.p);//上面做了这么多，就是为了在合理的空间内创建一个completion_handler对象，将对象地址传入该函数
  p.v = p.p = 0;
  //Handler* h; \
  //void* v; \//保存op的空间
  //op* p; \//指向v的地址 
}

} // namespace detail
} // namespace asio

#include "RCF/external/asio/asio/detail/pop_options.hpp"

#endif // ASIO_DETAIL_IMPL_TASK_IO_SERVICE_HPP
