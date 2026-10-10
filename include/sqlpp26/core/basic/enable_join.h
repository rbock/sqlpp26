#pragma once

/*
 * Copyright (c) 2024, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sqlpp26/core/basic/join_fwd.h>
#include <sqlpp26/core/type_traits.h>
#include <utility>

namespace sqlpp {
struct enable_join {
 public:
  template <typename Table, typename Rhs>
  requires(can_be_joined_v<std::decay_t<Table>, Rhs>)
  auto join(this Table&& self, Rhs rhs)
      -> pre_join_t<table_ref_t<std::decay_t<Table>>,
                    inner_join_t,
                    table_ref_t<Rhs>> {
    return ::sqlpp::join(std::forward<Table>(self), std::move(rhs));
  }

  template <typename Table, typename Rhs>
  requires(can_be_joined_v<std::decay_t<Table>, Rhs>)
  auto inner_join(this Table&& self, Rhs rhs)
      -> pre_join_t<table_ref_t<std::decay_t<Table>>,
                    inner_join_t,
                    table_ref_t<Rhs>> {
    return ::sqlpp::inner_join(std::forward<Table>(self), std::move(rhs));
  }

  template <typename Table, typename Rhs>
  requires(can_be_joined_v<std::decay_t<Table>, Rhs>)
  auto left_outer_join(this Table&& self, Rhs rhs)
      -> pre_join_t<table_ref_t<std::decay_t<Table>>,
                    left_outer_join_t,
                    table_ref_t<Rhs>> {
    return ::sqlpp::left_outer_join(std::forward<Table>(self), std::move(rhs));
  }

  template <typename Table, typename Rhs>
  requires(can_be_joined_v<std::decay_t<Table>, Rhs>)
  auto right_outer_join(this Table&& self, Rhs rhs)
      -> pre_join_t<table_ref_t<std::decay_t<Table>>,
                    right_outer_join_t,
                    table_ref_t<Rhs>> {
    return ::sqlpp::right_outer_join(std::forward<Table>(self), std::move(rhs));
  }

  template <typename Table, typename Rhs>
  requires(can_be_joined_v<std::decay_t<Table>, Rhs>)
  auto full_outer_join(this Table&& self, Rhs rhs)
      -> pre_join_t<table_ref_t<std::decay_t<Table>>,
                    full_outer_join_t,
                    table_ref_t<Rhs>> {
    return ::sqlpp::full_outer_join(std::forward<Table>(self), std::move(rhs));
  }

  template <typename Table, typename Rhs>
  requires(can_be_joined_v<std::decay_t<Table>, Rhs>)
  auto cross_join(this Table&& self, Rhs rhs)
      -> join_t<table_ref_t<std::decay_t<Table>>,
                cross_join_t,
                table_ref_t<Rhs>,
                unconditional_t> {
    return ::sqlpp::cross_join(std::forward<Table>(self), std::move(rhs));
  }
};

template <typename T>
struct has_enabled_join : public std::is_base_of<enable_join, T> {};

}  // namespace sqlpp
