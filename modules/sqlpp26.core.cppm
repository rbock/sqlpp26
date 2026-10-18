/*
 * Copyright (c) 2025-2026, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
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

module;

#include <sqlpp26/sqlpp26.h>
#include <sqlpp26/core/database/connection_pool.h>
#include <sqlpp26/core/detail/parse_date_time.h>
export module sqlpp26.core;

export namespace sqlpp {
// basics:
using ::sqlpp::column;
using ::sqlpp::column_spec;
using ::sqlpp::column_spec_of;
using ::sqlpp::column_spec_of_t;
using ::sqlpp::table;
using ::sqlpp::table_generator;
using ::sqlpp::table_as;
using ::sqlpp::enable_table_as;
using ::sqlpp::table_spec_of;
using ::sqlpp::table_spec_of_t;
using ::sqlpp::verbatim_clause_t;
using ::sqlpp::verbatim_t;
using ::sqlpp::verbatim_table_t;
using ::sqlpp::verbatim_table_as_t;
using ::sqlpp::all_of;
using ::sqlpp::enable_join;
using ::sqlpp::join;
using ::sqlpp::inner_join;
using ::sqlpp::left_outer_join;
using ::sqlpp::right_outer_join;
using ::sqlpp::full_outer_join;
using ::sqlpp::cross_join;
using ::sqlpp::parameter;
using ::sqlpp::parameter_t;
using ::sqlpp::verbatim;
using ::sqlpp::verbatim_clause;
using ::sqlpp::verbatim_table;
using ::sqlpp::parameterized_verbatim;
using ::sqlpp::star;
using ::sqlpp::value;
using ::sqlpp::value_t;
using ::sqlpp::no_value_t;
using ::sqlpp::noop;
using ::sqlpp::fixed_string;
using ::sqlpp::type_v;

// data types
using ::sqlpp::boolean;
using ::sqlpp::integral;
using ::sqlpp::unsigned_integral;
using ::sqlpp::floating_point;
using ::sqlpp::text;
using ::sqlpp::blob;
using ::sqlpp::date;
using ::sqlpp::timestamp;
using ::sqlpp::time_of_day;

// operators:arithmetic
using ::sqlpp::plus;
using ::sqlpp::minus;
using ::sqlpp::multiplies;
using ::sqlpp::divides;
using ::sqlpp::negate;
using ::sqlpp::modulus;
using ::sqlpp::arithmetic_data_type;
using ::sqlpp::arithmetic_expression;
using ::sqlpp::operator+;
using ::sqlpp::operator-;
using ::sqlpp::operator*;
using ::sqlpp::operator/;
using ::sqlpp::operator%;

// operators:as
using ::sqlpp::as;
using ::sqlpp::enable_as;
using ::sqlpp::as_expression;

// operators:assign
using ::sqlpp::assign;

// operators:bit
using ::sqlpp::operator&;
using ::sqlpp::operator|;
using ::sqlpp::operator^;
using ::sqlpp::operator~;
using ::sqlpp::operator<<;
using ::sqlpp::operator>>;

// operators:comparison
using ::sqlpp::operator<;
using ::sqlpp::operator<=;
using ::sqlpp::operator==;
using ::sqlpp::operator!=;
using ::sqlpp::operator>=;
using ::sqlpp::operator>;
using ::sqlpp::is_null;
using ::sqlpp::is_not_null;
using ::sqlpp::is_distinct_from;
using ::sqlpp::is_not_distinct_from;
using ::sqlpp::like;
using ::sqlpp::enable_comparison;

// operators:logical
using ::sqlpp::operator&&;
using ::sqlpp::operator||;
using ::sqlpp::operator!;

// operators:misc
using ::sqlpp::any;
using ::sqlpp::between;
using ::sqlpp::case_when;
using ::sqlpp::cast;
using ::sqlpp::cast_as_t;
using ::sqlpp::exists;
using ::sqlpp::in;
using ::sqlpp::not_in;
using ::sqlpp::asc;
using ::sqlpp::desc;
using ::sqlpp::sort_type; // enum
using ::sqlpp::null_position; // enum
using ::sqlpp::order;
using ::sqlpp::when_then_pair_t;

// clauses:
using ::sqlpp::cte;
using ::sqlpp::default_value;
using ::sqlpp::default_value_t;
using ::sqlpp::delete_from;
using ::sqlpp::for_update;
using ::sqlpp::from;
using ::sqlpp::group_by;
using ::sqlpp::having;
using ::sqlpp::insert;
using ::sqlpp::insert_columns;
using ::sqlpp::insert_default_values;
using ::sqlpp::insert_into;
using ::sqlpp::insert_set;
using ::sqlpp::into;
using ::sqlpp::limit;
using ::sqlpp::offset;
using ::sqlpp::on_conflict;
using ::sqlpp::order_by;
using ::sqlpp::returning;
using ::sqlpp::select;
using ::sqlpp::select_columns;
using ::sqlpp::simple_column;
using ::sqlpp::single_table;
using ::sqlpp::star;
using ::sqlpp::star_t;
using ::sqlpp::statement_t;
using ::sqlpp::truncate;
using ::sqlpp::union_all;
using ::sqlpp::union_all_t;
using ::sqlpp::union_distinct;
using ::sqlpp::union_order_by;
using ::sqlpp::blank_update_t;
using ::sqlpp::update;
using ::sqlpp::update_set;
using ::sqlpp::using_;
using ::sqlpp::where;
using ::sqlpp::with;
using ::sqlpp::with_result_type_of;
using ::sqlpp::no_insert_value_list_t;
using ::sqlpp::having_t;
using ::sqlpp::no_having_t;
using ::sqlpp::no_limit_t;
using ::sqlpp::no_offset_t;
using ::sqlpp::no_into_t;
using ::sqlpp::no_from_t;
using ::sqlpp::no_order_by_t;
using ::sqlpp::no_group_by_t;
using ::sqlpp::no_where_t;
using ::sqlpp::no_select_column_list_t;
using ::sqlpp::no_single_table_t;
using ::sqlpp::no_returning_t;
using ::sqlpp::no_on_conflict_t;
using ::sqlpp::no_with_t;
using ::sqlpp::no_union_t;
using ::sqlpp::no_union_order_by_t;
using ::sqlpp::no_using_t;
using ::sqlpp::no_update_set_list_t;
using ::sqlpp::cte_ref_t;
using ::sqlpp::select_ref_t;
using ::sqlpp::cte_union_t;
using ::sqlpp::with_t;

// select flags
using ::sqlpp::all;
using ::sqlpp::all_t;
using ::sqlpp::distinct;
using ::sqlpp::distinct_t;

// aggregate functions
using ::sqlpp::star;
using ::sqlpp::avg;
using ::sqlpp::count;
using ::sqlpp::max;
using ::sqlpp::min;
using ::sqlpp::sum;
using ::sqlpp::over;

// functions
using ::sqlpp::coalesce;
using ::sqlpp::tuple_operand;
using ::sqlpp::concat;
using ::sqlpp::current_date;
using ::sqlpp::current_time;
using ::sqlpp::current_timestamp;
using ::sqlpp::flatten;
using ::sqlpp::name_of;
using ::sqlpp::name_of_v;
using ::sqlpp::sql_name_of;
using ::sqlpp::sql_name_of_v;
using ::sqlpp::lower;
using ::sqlpp::make_table_ref;
using ::sqlpp::trim;
using ::sqlpp::upper;

// database
using ::sqlpp::bind_parameter;
using ::sqlpp::read_field;
using ::sqlpp::bind_field;
using ::sqlpp::isolation_level;
using ::sqlpp::start_transaction;
using ::sqlpp::exception;
using ::sqlpp::connection_check;
using ::sqlpp::normal_connection;
using ::sqlpp::pooled_connection;

// query
using ::sqlpp::dynamic;
using ::sqlpp::dynamic_t;

// serialization
using ::sqlpp::to_sql_string;

// logging
using ::sqlpp::log_category;
using ::sqlpp::log_function_t;
using ::sqlpp::debug_logger;

// type_traits
using ::sqlpp::no_of_result_columns;
using ::sqlpp::can_be_null;
using ::sqlpp::data_type_of;
using ::sqlpp::data_type_of_t;
using ::sqlpp::get_result_row_t;
using ::sqlpp::has_data_type;
using ::sqlpp::has_data_type_v;
using ::sqlpp::has_name;
using ::sqlpp::has_name_v;
using ::sqlpp::has_statement_data_type;
using ::sqlpp::is_aggregate_neutral;
using ::sqlpp::has_result_row;
using ::sqlpp::is_boolean;
using ::sqlpp::is_boolean_v;
using ::sqlpp::is_dynamic;
using ::sqlpp::is_dynamic_v;
using ::sqlpp::is_floating_point;
using ::sqlpp::is_floating_point_v;
using ::sqlpp::is_integral;
using ::sqlpp::is_integral_v;
using ::sqlpp::is_arithmetic;
using ::sqlpp::is_arithmetic_v;
using ::sqlpp::is_optional;
using ::sqlpp::is_prepared_statement;
using ::sqlpp::is_prepared_statement_v;
using ::sqlpp::is_raw_select_flag;
using ::sqlpp::is_table;
using ::sqlpp::is_text;
using ::sqlpp::is_text_v;
using ::sqlpp::is_unsigned_integral;
using ::sqlpp::is_unsigned_integral_v;
using ::sqlpp::make_parameter_list_t;
using ::sqlpp::have_correct_static_cte_dependencies;
using ::sqlpp::parameter_data_type;
using ::sqlpp::parameters_of_t;
using ::sqlpp::remove_as_t;
using ::sqlpp::requires_parentheses;
using ::sqlpp::remove_any_t;
using ::sqlpp::remove_dynamic_t;
using ::sqlpp::remove_optional_t;
using ::sqlpp::is_aggregate_function;
using ::sqlpp::is_as_expression;
using ::sqlpp::is_data_type;
using ::sqlpp::is_data_type_v;
using ::sqlpp::contains_aggregate_function;
using ::sqlpp::is_aggregate_expression;
using ::sqlpp::is_non_aggregate_expression;
using ::sqlpp::is_time_of_day;
using ::sqlpp::is_time_of_day_v;
using ::sqlpp::values_are_assignable;
using ::sqlpp::values_are_comparable;
using ::sqlpp::nodes_of;
using ::sqlpp::nodes_of_t;
using ::sqlpp::wrong;
using ::sqlpp::result_data_type_of;
using ::sqlpp::is_timestamp;
using ::sqlpp::is_timestamp_v;
using ::sqlpp::is_blob;
using ::sqlpp::is_blob_v;
using ::sqlpp::is_date;
using ::sqlpp::is_date_v;
using ::sqlpp::is_date_or_timestamp;
using ::sqlpp::select_column_has_name;
using ::sqlpp::has_enabled_over;
using ::sqlpp::has_enabled_comparison;
using ::sqlpp::has_enabled_as;
using ::sqlpp::has_enabled_join;
using ::sqlpp::has_default;
using ::sqlpp::select_column_data_type_of_t;
using ::sqlpp::is_raw_table;
using ::sqlpp::is_result_clause;
using ::sqlpp::is_column;
using ::sqlpp::is_cte;
using ::sqlpp::is_recursive_cte;
using ::sqlpp::have_correct_cte_dependencies;
using ::sqlpp::table_of;
using ::sqlpp::table_of_t;

// consistency checks
using ::sqlpp::check_compatibility;
using ::sqlpp::check_basic_consistency;
using ::sqlpp::check_prepare_consistency;
using ::sqlpp::check_run_consistency;
using ::sqlpp::basic_consistency_check;
using ::sqlpp::prepare_check;
using ::sqlpp::run_check;

// reflection-based get functions
using ::sqlpp::get_required_tables_of;
using ::sqlpp::get_required_static_tables_of;
using ::sqlpp::get_provided_tables_of;
using ::sqlpp::get_provided_static_tables_of;
using ::sqlpp::get_provided_optional_tables_of;
using ::sqlpp::get_required_ctes_of;
using ::sqlpp::get_required_insert_columns_of;
using ::sqlpp::get_required_static_ctes_of;
using ::sqlpp::get_provided_ctes_of;
using ::sqlpp::get_provided_static_ctes_of;
using ::sqlpp::get_known_aggregate_columns_of;
using ::sqlpp::get_known_static_aggregate_columns_of;
}

export namespace sqlpp::chrono {
using ::sqlpp::chrono::sys_microseconds;
}

export namespace sqlpp::detail {
// detail
using ::sqlpp::detail::are_same;
using ::sqlpp::detail::are_same_v;
using ::sqlpp::detail::are_unique;
using ::sqlpp::detail::are_unique_v;
using ::sqlpp::detail::circular_buffer;
using ::sqlpp::detail::parse_date;
using ::sqlpp::detail::parse_time;
using ::sqlpp::detail::parse_timestamp;
using ::sqlpp::detail::type_info_set;
using ::sqlpp::detail::type_info_less;
using ::sqlpp::detail::make_type_info_set;
using ::sqlpp::detail::type_vector;
using ::sqlpp::detail::get_last_if_t;
}

export namespace sqlpp::logic {
using ::sqlpp::logic::logic_helper;
using ::sqlpp::logic::all;
using ::sqlpp::logic::any;
using ::sqlpp::logic::none;
}
