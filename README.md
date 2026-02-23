# octo

`octo` is a command-line utility designed to streamline work with multiple sibling Git repositories. Inspired by the Git "octopus merge," `octo` allows you to treat a collection of standalone repositories as a single, layered system. This is an ideal alternative to monolithic repositories (monorepos) when you need to maintain separate lifecycles for components like IPC, business logic, drivers, and utilities, while still performing uniform operations across them.

## Features

- **Uniform Git Operations**: Run `pull`, `push`, `status`, `checkout`, and `clone` across all repositories in a workspace with a single command.
- **Declarative Configuration**: Define your "universe" of projects and workspaces in a simple, human-readable text file.
- **Custom Command Execution**: Run any shell command across all repositories using `octo exec`.
- **Workspace Isolation**: Manage different sets of repositories (workspaces) and switch between them easily.
- **No-Bloat Architecture**: Written in C for performance and minimal dependencies.

## Installation

`octo` requires a C99-compliant compiler (such as `gcc` or `clang`) and a standard C library.

1. Clone the repository (or download the source).
2. Navigate to the project directory.
3. Build the utility:
   ```bash
   make
   ```
   This will create a `octo` binary in the root directory and use an `obj/` folder for intermediate build artifacts.

4. (Optional) Move the resulting `octo` binary to your `PATH`:
   ```bash
   mv octo /usr/local/bin/
   ```

## Testing

`octo` comes with a comprehensive test suite. To run the tests:

```bash
make test
```

This will build the `test_runner` and execute all test cases.

## Configuration

`octo` looks for a workspace definition file. By default, it expects this file at `~/.octo/workspaces`, but you can specify a custom file using the `--def` flag.

### The Definition File Format

The definition file consists of two main sections: `projects` (default repositories) and `workspace` definitions.

```text
# This is a sample workspace definition file

# Default projects to be included in workspaces
projects {
    utils
    ipc
    business
    trading
    analytics
    data
    ai
}

# Workspace definitions mapping an alias to a physical path
workspace w1 -> /Users/vlad/Documents/work/workspace {
    # You can add workspace-specific projects here
}

workspace w2 -> /Users/vlad/Documents/work/workspace2 {
}
```

- **`projects { ... }`**: Lists the subdirectory names of the Git repositories you want to manage.
- **`workspace <alias> -> <path> { ... }`**: Defines a workspace. The `<path>` is the parent directory where the projects reside. You can define additional projects inside the curly braces that are specific to that workspace.

## Usage

```bash
octo [options] command [arguments]
```

### Commands

| Command | Description |
| :--- | :--- |
| `pull` | Performs `git pull -p` in all repository directories. |
| `push` | Performs `git push` in all repository directories. |
| `status` | Checks `git status --porcelain` and reports any changes. |
| `checkout <branch>` | Switches all repositories to the specified branch. |
| `clone <url_prefix>` | Clones the repositories using the provided URL prefix (e.g., `octo clone git@github.com:myorg/`). |
| `list` | Lists the absolute paths of all repositories in the workspace. |
| `path <alias>/<project>` | Prints the full physical path to a specific project. |
| `exec <command>` | Executes an arbitrary shell command in each repository directory. |
| `version` | Prints the current version of `octo`. |

### Options

| Option | Description |
| :--- | :--- |
| `--def=<file>` | Path to the workspace definition file. |
| `--workspace=<name>`, `-w=<name>` | Target a specific workspace defined in your config. |
| `--verbose`, `-v` | Enable verbose output (shows full command execution details). |
| `--no-colour` | Disable ANSI color output. |

## Common Workflows

`octo` is designed to handle repetitive Git tasks across many repositories. Here are some common ways to use it:

### 1. Setting Up a New Workspace
Once you have defined your projects and workspace in the definition file, you can clone all repositories at once:
```bash
octo clone git@github.com:myorg/
```

### 2. Global Status Check
Quickly see which repositories have uncommitted changes or are out of sync with their remotes:
```bash
octo status

# Check status for a specific workspace only
octo -w=w1 status
```

### 3. Synchronising All Layers
Bring your entire system up to date with a single command:
```bash
octo pull
```

### 4. Branch Management
Switch the whole workspace to a new feature branch for coordinated development:
```bash
octo checkout feature/new-engine
```

### 5. Bulk Build and Cleanup
Execute standard shell commands across all projects:
```bash
# Build all components
octo exec make

# Perform a deep clean in all repositories
octo exec git clean -fdx
```

### 6. Quick Navigation
You can use `octo path` to quickly jump to workspace roots or specific projects:

```bash
# Go to the root of workspace 'w1'
cd `octo path w1/.`

# Go to the 'utils' project in workspace 'w1'
cd `octo path w1/utils`
```

## Why octo?
In a layered system, components are often developed in parallel. Managing them as separate Git repositories allows for cleaner boundaries and independent versioning. However, common tasks (like checking the status of all layers or pulling updates for the whole system) become tedious. `octo` automates these chores, providing the convenience of a monorepo with the flexibility of polyrepo architecture.

## License

`octo` is released under the FreeBSD 3-Clause License. See the [LICENSE](LICENSE) file for the full text.
