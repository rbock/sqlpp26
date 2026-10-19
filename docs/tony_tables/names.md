[**\< Differences**](/docs/differences_to_sqlpp23.md)

# Before and after: Names

<table>
<tr>
<th align="left">sqlpp11</th><th align="left">sqlpp23</th>
</tr>
</tr>
<tr><td colspan=2>

  **Names**

</td></tr>
<tr>
<td  valign="top">

```c++
// Define name at namespace level
SQLPP_CREATE_NAME_TAG(something);

// Use name elsewhere
select(sqlpp::value(7).as(something));
```

</td>
<td valign="top">

```c++
// Use string literals in place
select(sqlpp::value(7).as<"something">());
```

</td>
</tr>
</table>

[**\< Differences**](/docs/differences_to_sqlpp23.md)

