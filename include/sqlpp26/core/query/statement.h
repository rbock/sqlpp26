#pragma once

/*
 * Copyright (c) 2013-2015, Roland Bock
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

#include <stdexcept>

#include <sqlpp26/core/basic/value.h>
#include <sqlpp26/core/database/parameter_list.h>
#include <sqlpp26/core/detail/get_first.h>
#include <sqlpp26/core/detail/get_last.h>
#include <sqlpp26/core/detail/pick_arg.h>
#include <sqlpp26/core/hidden.h>
#include <sqlpp26/core/noop.h>
#include <sqlpp26/core/query/statement_constructor_arg.h>
#include <sqlpp26/core/query/statement_fwd.h>
#include <sqlpp26/core/result.h>
#include <sqlpp26/core/result_type_provider.h>
#include <sqlpp26/core/to_sql_string.h>
#include <type_traits>
#include <sqlpp26/core/detail/type_vector.h>
#include <sqlpp26/core/indices.h>

namespace sqlpp {
// TODO: Need to write a type test!
template <typename... Clauses>
consteval auto get_provided_ctes_of_statement(type_v<statement_t<Clauses...>>)
    -> detail::type_info_set {
  return detail::make_joined_type_info_set(
      get_provided_ctes_of(type_v<Clauses>{})...);
}

template <typename... Clauses>
consteval auto get_provided_static_ctes_of_statement(
    type_v<statement_t<Clauses...>>) -> detail::type_info_set {
  return detail::make_joined_type_info_set(
      get_provided_static_ctes_of(type_v<Clauses>{})...);
}

template <typename... Clauses>
consteval auto get_provided_tables_of_statement(type_v<statement_t<Clauses...>>)
    -> detail::type_info_set {
  return detail::make_joined_type_info_set(
      get_provided_tables_of(type_v<Clauses>{})...);
}

template <typename... Clauses>
consteval auto get_provided_static_tables_of_statement(
    type_v<statement_t<Clauses...>>) -> detail::type_info_set {
  return detail::make_joined_type_info_set(
      get_provided_static_tables_of(type_v<Clauses>{})...);
}

template <typename... Clauses>
consteval auto get_provided_optional_tables_of_statement(
    type_v<statement_t<Clauses...>>) -> detail::type_info_set {
  return detail::make_joined_type_info_set(
      get_provided_optional_tables_of(type_v<Clauses>{})...);
}

template <typename... Clauses>
consteval auto get_known_aggregate_columns_of_statement(
    type_v<statement_t<Clauses...>>) -> detail::type_info_set {
  return detail::make_joined_type_info_set(
      get_known_aggregate_columns_of(type_v<Clauses>{})...);
}

template <typename... Clauses>
consteval auto get_known_static_aggregate_columns_of_statement(
    type_v<statement_t<Clauses...>>) -> detail::type_info_set {
  return detail::make_joined_type_info_set(
      get_known_static_aggregate_columns_of(type_v<Clauses>{})...);
}

template <typename Clause, fixed_string Name, typename... Clauses>
[[nodiscard]] constexpr auto check_cte_consistency(type_v<statement_t<Clauses...>>) -> std::expected<void, std::string_view> {
  using Statement = statement_t<Clauses...>;
  static constexpr auto required_ctes =
      std::define_static_array(get_required_ctes_of(type_v<Clause>{}));
  template for (constexpr auto& info : required_ctes) {
    // TODO: Use .contains() when it is supported in consteval
    if (not std::ranges::contains(
            get_provided_ctes_of_statement(type_v<Statement>{}), info)) {
      using cte = typename[:info:];
      return std::unexpected{std::define_static_string(std::format(
          "The {}-clause requires cte {} which is not known "
          "in the statement",
          std::string_view{Name}, std::string_view{name_of_v<cte>}))};
    }
  }
  return {};
}

template <typename Clause, fixed_string Name, typename... Clauses>
[[nodiscard]] constexpr auto check_static_cte_consistency(type_v<statement_t<Clauses...>>) -> std::expected<void, std::string_view> {
  using Statement = statement_t<Clauses...>;
  static constexpr auto required_static_ctes =
      std::define_static_array(get_required_static_ctes_of(type_v<Clause>{}));
  template for (constexpr auto& info : required_static_ctes) {
    // TODO: Use .contains() when it is supported in consteval
    if (std::ranges::contains(
            get_provided_ctes_of_statement(type_v<Statement>{}), info) and
        not std::ranges::contains(
            get_provided_static_ctes_of_statement(type_v<Statement>{}), info)) {
      using cte = typename[:info:];
      return std::unexpected{std::define_static_string(std::format(
          "The {}-clause statically requires cte {} which is "
          "only known dynamically in the statement",
          std::string_view{Name}, std::string_view{name_of_v<cte>}))};
    }
  }
  return {};
}

template <typename Clause, fixed_string Name, typename... Clauses>
[[nodiscard]] constexpr auto check_table_consistency(type_v<statement_t<Clauses...>>) -> std::expected<void, std::string_view> {
  using Statement = statement_t<Clauses...>;
  static constexpr auto required_tables =
      std::define_static_array(get_required_tables_of(type_v<Clause>{}));
  template for (constexpr auto& info : required_tables) {
    // TODO: Use .contains() when it is supported in consteval
    if (not std::ranges::contains(
            get_provided_tables_of_statement(type_v<Statement>{}), info)) {
      using table = typename[:info:];
      return std::unexpected{std::define_static_string(std::format(
          "The {}-clause requires table {} which is not known "
          "in the statement",
          std::string_view{Name}, std::string_view{name_of_v<table>}))};
    }
  }
  return {};
}

template <typename Clause, fixed_string Name, typename... Clauses>
[[nodiscard]] constexpr auto check_static_table_consistency(type_v<statement_t<Clauses...>>) -> std::expected<void, std::string_view> {
  using Statement = statement_t<Clauses...>;
  static constexpr auto required_static_tables =
      std::define_static_array(get_required_static_tables_of(type_v<Clause>{}));
  template for (constexpr auto& info : required_static_tables) {
    // TODO: Use .contains() when it is supported in consteval
    if (std::ranges::contains(
            get_provided_tables_of_statement(type_v<Statement>{}), info) and
        not std::ranges::contains(
            get_provided_static_tables_of_statement(type_v<Statement>{}),
            info)) {
      using table = typename[:info:];
      return std::unexpected{std::define_static_string(std::format(
          "The {}-clause statically requires table {} which is "
          "only known dynamically in the statement",
          std::string_view{Name}, std::string_view{name_of_v<table>}))};
    }
  }
  return {};
}

template <typename... Clauses>
[[nodiscard]] constexpr auto check_basic_consistency(type_v<statement_t<Clauses...>>) -> std::expected<void, std::string_view> {
  using Statement = statement_t<Clauses...>;
  auto check = std::expected<void, std::string_view>{};
  ((check = basic_consistency_check<Statement, Clauses>::verify()) && ...);
  if (not check) {
    return check;
  }
  std::flat_set<std::string_view> all_provided_tables;
  template for (constexpr auto index :
                std::views::iota(size_t{}, sizeof...(Clauses))) {
    static constexpr auto provided_tables = std::define_static_array(
        get_provided_tables_of(type_v<Clauses... [index]> {}));
    template for (constexpr auto& info : provided_tables) {
      using Table = typename[:info:];
      const auto [_, unique] = all_provided_tables.insert(std::string_view{name_of_v<Table>});
      if (not unique) {
        return std::unexpected{std::define_static_string(
            std::format("Table(s) of name {} provided twice in the statement",
                        std::string_view{name_of_v<Table>}))};
      }
    }
  }
  return {};
}

template <typename... Clauses>
[[nodiscard]] constexpr auto check_prepare_consistency(type_v<statement_t<Clauses...>>) -> std::expected<void, std::string_view> {
  using Statement = statement_t<Clauses...>;
  auto check = check_basic_consistency(type_v<Statement>{});
  if (sizeof...(Clauses)) {
    (check && ... && (check = prepare_check<Statement, Clauses>::verify()));
  }
  return check;
}

template <typename... Clauses>
[[nodiscard]] constexpr auto check_run_consistency(type_v<statement_t<Clauses...>>) -> std::expected<void, std::string_view> {
  using Statement = statement_t<Clauses...>;
  auto check = check_prepare_consistency(type_v<Statement>{});
  if (sizeof...(Clauses)) {
    (check && ... && (check = run_check<Statement, Clauses>::verify()));
  }
  if (not check) {
    return check;
  }
  using _parameters = detail::type_vector_cat_t<parameters_of_t<Clauses>...>;
  if constexpr (not _parameters::empty()) {
    return std::unexpected{std::string_view{"cannot execute statements with parameters "
                            "directly, use prepare instead"}};
  }
  return {};
}

template <typename... Clauses>
using result_methods_t =
    result_methods_of_t<result_type_provider_t<Clauses...>>;

template <typename... Clauses>
struct statement_t : public Clauses..., public result_methods_t<Clauses...> {
  // Constructors
  statement_t() = default;

  template <typename... Fragments>
  constexpr statement_t(statement_constructor_arg<Fragments...> arg)
      : Clauses{arg}... {}

  statement_t(const statement_t& r) = default;
  statement_t(statement_t&& r) = default;
  statement_t& operator=(const statement_t& r) = default;
  statement_t& operator=(statement_t&& r) = default;
  ~statement_t() = default;
};

template <typename... Clauses>
struct no_of_result_columns<statement_t<Clauses...>>
    : public no_of_result_columns<result_type_provider_t<Clauses...>> {};

template <typename... Clauses>
struct result_methods_of<statement_t<Clauses...>> {
  using type = result_methods_t<Clauses...>;
};

template <typename... Clauses>
struct is_statement<statement_t<Clauses...>> : public std::true_type {};

template <typename... Clauses>
struct contains_order_by<statement_t<Clauses...>>
{
  static constexpr bool value = (false or ... or contains_order_by_v<Clauses>);
};

template <typename... Clauses>
struct contains_limit<statement_t<Clauses...>>
{
  static constexpr bool value = (false or ... or contains_limit_v<Clauses>);
};

template <typename... Clauses>
struct contains_offset<statement_t<Clauses...>>
{
  static constexpr bool value = (false or ... or contains_offset_v<Clauses>);
};

template <typename... Clauses>
struct contains_for_update<statement_t<Clauses...>>
{
  static constexpr bool value = (false or ... or contains_for_update_v<Clauses>);
};

template <typename... Clauses>
struct has_result_row<statement_t<Clauses...>>
    : public has_result_row<result_type_provider_t<Clauses...>> {};

template <typename... Clauses>
struct get_result_row<statement_t<Clauses...>> {
  using type = result_row_of_t<statement_t<Clauses...>,
                               result_type_provider_t<Clauses...>>;
};

// No data_type_of for statements. 
// * sqlpp::any and sqlpp::exists operators can handle statements directly.
// * wrap the statement in sqlpp::value() to use it as a value.
template <typename... Clauses>
struct statement_data_type_of<statement_t<Clauses...>> {
  using type =
      data_type_of_t<result_type_provider_t<Clauses...>>;
};

// statements explicitly do not expose any nodes as most recursive traits
// should not traverse into sub queries, e.g.
//   - contains_aggregates
//   - known_aggregate_columns_of
template <typename... Clauses>
struct nodes_of<statement_t<Clauses...>> : public no_nodes {
};

template <typename Context, typename... Clauses>
[[nodiscard]] constexpr auto check_compatibility(type_v<Context>, type_v<statement_t<Clauses...>>) -> std::expected<void, std::string_view> {
  return check_compatibility(type_v<Context>{}, detail::type_vector<Clauses...>{});
}

template <typename... Clauses>
consteval detail::type_info_set get_required_insert_columns_of(type_v<statement_t<Clauses...>>) {
  return detail::make_joined_type_info_set(
      get_required_insert_columns_of(type_v<Clauses>{})...);
}

template <typename... Clauses>
struct parameters_of<statement_t<Clauses...>> {
  using type = detail::type_vector_cat_t<parameters_of_t<Clauses>...>;
};

template<typename... Clauses>
consteval detail::type_info_set get_required_tables_of(type_v<statement_t<Clauses...>>) {
  return detail::make_type_info_set_difference(
      detail::make_joined_type_info_set(
          get_required_tables_of(type_v<Clauses>{})...),
      detail::make_joined_type_info_set(
          get_provided_tables_of(type_v<Clauses>{})...));
}

template<typename... Clauses>
consteval detail::type_info_set get_required_static_tables_of(type_v<statement_t<Clauses...>>) {
  return detail::make_type_info_set_difference(
      detail::make_joined_type_info_set(
          get_required_static_tables_of(type_v<Clauses>{})...),
      detail::make_joined_type_info_set(
          get_provided_static_tables_of(type_v<Clauses>{})...));
}

template <typename... Clauses>
consteval detail::type_info_set get_required_ctes_of(type_v<statement_t<Clauses...>>) {
  return detail::make_type_info_set_difference(
      detail::make_joined_type_info_set(get_required_ctes_of(type_v<Clauses>{})...),
      detail::make_joined_type_info_set(get_provided_static_ctes_of(type_v<Clauses>{})...));
}

template <typename... Clauses>
consteval detail::type_info_set get_required_static_ctes_of(type_v<statement_t<Clauses...>>) {
  return detail::make_type_info_set_difference(
      detail::make_joined_type_info_set(
          get_required_static_ctes_of(type_v<Clauses>{})...),
      detail::make_joined_type_info_set(
          get_provided_static_ctes_of(type_v<Clauses>{})...));
}

template <typename... Clauses>
struct requires_parentheses<statement_t<Clauses...>> : public std::true_type {};

template <typename... Clauses>
[[nodiscard]] auto check_table_consistency(const statement_t<Clauses...>&) {
  using S = statement_t<Clauses...>;
  return typename S::_table_check{} and typename S::_cte_check{};
}

template <typename... Clauses>
[[nodiscard]] auto check_parameter_consistency(const statement_t<Clauses...>&) {
  using S = statement_t<Clauses...>;
  return typename S::_parameter_check{};
}

template <typename OldClause, typename... Clauses, typename NewClause>
constexpr auto new_statement(statement_t<Clauses...> oldStatement, NewClause newClause)
    -> statement_t<std::conditional_t<std::is_same<Clauses, OldClause>::value,
                                      NewClause,
                                      Clauses>...> {
  return statement_t<std::conditional_t<std::is_same<Clauses, OldClause>::value,
                                        NewClause, Clauses>...>{
      statement_constructor_arg(oldStatement, newClause)};
}

template <typename T>
struct core_statement;
template <typename... Clauses>
struct core_statement<detail::type_vector<Clauses...>> {
  using type = statement_t<Clauses...>;
};

template <typename... Clauses>
using core_statement_t = typename core_statement<
    detail::copy_if_t<detail::type_vector<Clauses...>, is_clause>>::type;

template <typename Statement>
struct statement_has_unique_clauses;

template <typename... Clauses>
struct statement_has_unique_clauses<statement_t<Clauses...>>
    : public detail::are_unique<Clauses...> {};

template <typename... LClauses, typename... RClauses>
constexpr auto operator<<(statement_t<LClauses...> l,
                          statement_t<RClauses...> r)
    -> core_statement_t<LClauses..., RClauses...> {
  using _core_statement = core_statement_t<LClauses..., RClauses...>;
  static_assert(statement_has_unique_clauses<_core_statement>::value,
                      "statements must contain unique clauses only");
  return _core_statement(statement_constructor_arg(std::move(l), std::move(r)));
}

template <typename... LClauses, typename Clause>
constexpr auto operator<<(statement_t<LClauses...> l, Clause r)
    -> core_statement_t<LClauses..., Clause> {
  static_assert(
      is_clause<Clause>::value,
      "statement_t::operator<< requires statements or clauses as parameters");
  using _core_statement = core_statement_t<LClauses..., Clause>;
  static_assert(statement_has_unique_clauses<_core_statement>::value,
                      "statements must contain unique clauses only");
  return _core_statement(statement_constructor_arg(std::move(l), std::move(r)));
}

template <typename Context, typename... Clauses>
auto to_sql_string(Context& context, const statement_t<Clauses...>& t)
    -> std::string {
  auto result = std::string{};
  auto first = true;
  template for (constexpr auto Idx : indices<sizeof...(Clauses)>) {
    using Clause = Clauses...[Idx];
    if constexpr (is_clause<Clause>::value and not is_hidden_clause<Clause>::value) {
      if (not first) {
        result += " ";
      }
      result += to_sql_string(context, static_cast<const Clause&>(t));
      first = false;
    }
  }

  return result;
}

}  // namespace sqlpp
