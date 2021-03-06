//
// detail/posix_event.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_POSIX_EVENT_HPP
#define ASIO_DETAIL_POSIX_EVENT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "RCF/external/asio/asio/detail/config.hpp"

#if defined(BOOST_HAS_PTHREADS) && !defined(ASIO_DISABLE_THREADS)

#include <boost/assert.hpp>
#include <pthread.h>
#include "RCF/external/asio/asio/detail/noncopyable.hpp"

#include "RCF/external/asio/asio/detail/push_options.hpp"

namespace asio {
namespace detail {

class posix_event
  : private noncopyable
{
public:
  // Constructor.
  ASIO_DECL posix_event();

  // Destructor.
  ~posix_event()
  {
    ::pthread_cond_destroy(&cond_);
  }

  // Signal the event.
  template <typename Lock>
  void signal(Lock& lock)
  {
    BOOST_ASSERT(lock.locked());
    (void)lock;
    signalled_ = true;
    ::pthread_cond_signal(&cond_); // Ignore EINVAL.
  }

  // Signal the event and unlock the mutex.
  template <typename Lock>
  void signal_and_unlock(Lock& lock)
  {
    BOOST_ASSERT(lock.locked());
    signalled_ = true;
    lock.unlock();
    ::pthread_cond_signal(&cond_); // Ignore EINVAL.
  }

  // Reset the event.
  template <typename Lock>
  void clear(Lock& lock)
  {
    BOOST_ASSERT(lock.locked());
    (void)lock;
    signalled_ = false;
  }

  // Wait for the event to become signalled.
  template <typename Lock>
  void wait(Lock& lock)
  {
    BOOST_ASSERT(lock.locked());
    while (!signalled_)//加线程被唤醒，判断signalled,不是指定的就继续阻塞,signalled的目地好像是为了唤醒指定的线程.
      ::pthread_cond_wait(&cond_, &lock.mutex().mutex_); // Ignore EINVAL.
  }

private:
  ::pthread_cond_t cond_;
  bool signalled_;
};

} // namespace detail
} // namespace asio

#include "RCF/external/asio/asio/detail/pop_options.hpp"

#if defined(ASIO_HEADER_ONLY)
# include "RCF/external/asio/asio/detail/impl/posix_event.ipp"
#endif // defined(ASIO_HEADER_ONLY)

#endif // defined(BOOST_HAS_PTHREADS) && !defined(ASIO_DISABLE_THREADS)

#endif // ASIO_DETAIL_POSIX_EVENT_HPP
