# JLua - Java Library for Embedding Lua Scripts

**Version**: 1.0  

JLua is a lightweight Java library that allows you to embed Lua scripting into your Java applications. It provides an intuitive and flexible way to integrate Lua scripts for extending your application's capabilities or embedding domain-specific logic.

---

## Features

- **Run Lua Functions from Java**: Call Lua functions with custom argument types and values directly from your Java code.
- **Dynamic Argument and Return Type Support**: Pass multiple arguments with type specifications and retrieve typed results.
- **Simple API**: Designed with a straightforward method to handle Lua scripts and interactions.

---

## Getting Started

### Installation

- **Step 1**: Go to the Directory you want to include JLua.
- **Step 2**: Copy the code below.

```sh
curl -sS -O test
```

- **Step 3**: Open the command prompt and paste the code.

---

### Usage

Here's an example of how to use JLua in your application:

#### Example Code
```java
// Initialize JLua with the Lua script filename
JLua lua = new JLua("test.lua");

// Call a Lua function named "add" with specified arguments and return type
int result = lua.run("add", "int|int", "10|10", 2, Integer.class);

// Print the result
System.out.println("Result from Lua: " + result);
```

#### Lua Script (`test.lua`)
```lua
function add(a, b)
    return a + b
end
```

---

## Class and Method Details

### Class: `JLua`

#### Constructor:
- `JLua(String fname)`
  - **Parameters**:
    - `fname`: The name of the Lua script file to load.
  - **Description**: Creates a JLua instance and loads the specified Lua script.

---

### Method: `run`

```java
public <T> T p_run(String func, String t_args, String args, int argc, Class<T> t_ret);
```

#### Parameters:
1. **`func`**: The name of the Lua function to call.
2. **`t_args`**: A `|`-separated string specifying the types of the arguments (e.g., `int|int`, `string|float`).
3. **`args`**: A `|`-separated string specifying the values of the arguments (e.g., `10|10`, `hello|2.5`).
4. **`argc`**: The number of arguments passed to the Lua function.
5. **`t_ret`**: The expected return type of the Lua function (e.g., `Integer.class`, `String.class`).

#### Returns:
- The result of the Lua function, cast to the specified `returnType`.

#### Example:
```java
// Call a Lua function and get an Integer result
int result = lua.run("add", "int|int", "10|10", 2, Integer.class);
```

---

## Properties

- **`scriptFileName`**: The name of the Lua script currently loaded.

---

## Future Plans

- Add support for asynchronous Lua function calls.
- Extend support for more complex return types (e.g., lists, tables).
- Provide detailed error handling and debugging information.

---

Start embedding Lua scripting into your Java applications today with JLua!