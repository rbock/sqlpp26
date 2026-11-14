#pragma once

/*
 * Copyright (c) 2026, Roland Bock
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

#include <sqlpp26/ranges/database/context.h>

#include <sqlpp26/sqlpp26.h>

namespace sqlpp {

// TODO Need to add tests
template <typename DataType, fixed_string Name>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<parameter_t<DataType, Name>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for parameter");
};

template <fixed_string Name>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<verbatim_table_as_t<Name>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for verbatim_table");
};

[[nodiscard]] inline constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<for_update_t>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for FOR UPDATE");
};

template <typename TableSpec, fixed_string Alias>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<table_as<TableSpec, Alias>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for <table> AS");
};

template <typename DataType>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<verbatim_t<DataType>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for verbatim");
};

template <typename Select>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<any_t<Select>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for any()");
};

template <typename _Table>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<using_t<_Table>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for USING");
};

template <typename Lhs, typename JoinType, typename Rhs, typename Condition>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>,type_v<join_t<Lhs, JoinType, Rhs, Condition>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for JOIN");
};

template <typename Expression>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<limit_t<Expression>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for LIMIT");
};

template <typename Expression>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<offset_t<Expression>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for OFFSET");
};

template <typename Expression>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<over_t<Expression>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for OVER");
};

template <typename... Columns>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<returning_t<Columns...>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for RETURNING");
};

template <typename Select, fixed_string Name>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<select_as<Select, Name>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for SELECT ... AS");
};

template <typename... Columns>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<on_conflict_t<Columns...>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for ON CONFLICT");
};

template <typename... Ctes>
[[nodiscard]] constexpr auto check_compatibility(type_v<ranges::context_t>, type_v<with_t<Ctes...>>) -> std::expected<void, std::string_view> {
  return std::unexpected("ranges: No support for WITH");
};

}  // namespace sqlpp

