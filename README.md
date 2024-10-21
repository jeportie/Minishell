<h1 align="center">Minishell Project</h1>

<p align="center">
  <img src="https://img.shields.io/github/last-commit/jeportie/Minishell" alt="GitHub last commit">
  <img src="https://img.shields.io/github/issues/jeportie/Minishell" alt="GitHub issues">
  <img src="https://github.com/jeportie/minishell/actions/workflows/ci.yml/badge.svg?branch=main" alt="CI - Main">
</p>

<h3 align="center">A passionate team of developers from 42 School 🇫🇷</h3>

<p align="center">
  <a href="https://profile.intra.42.fr/users/gmarquis">
    <img src="https://badge.mediaplus.ma/greenbinary/gmarquis?1337Badge=off&UM6P=off" alt="Greg's 42 Badge" />
  </a>
  <a href="https://profile.intra.42.fr/users/jeportie">
    <img src="https://badge.mediaplus.ma/greenbinary/jeportie?1337Badge=off&UM6P=off" alt="Jerome's 42 Badge" />
  </a>
</p>

---

## Table of Contents

- [Introduction](#1-introduction)
- [Features](#2-features)
- [Getting Started](#3-getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#4-usage)
- [Project Structure](#5-project-structure)
- [Development Strategy](#6-development-strategy)
- [Contributing](#7-contributing)
- [License](#8-license)
- [Acknowledgments](#9-acknowledgments)
- [Connect with Us](#connect-with-us-)

---

## 1. Introduction

Minishell is a 42 school project that involves building a simple Unix shell from scratch. The goal is to replicate core functionalities of `bash` or `sh`, providing users with an interactive prompt where they can execute commands, handle redirection, and manage pipes, signals, and environment variables. Additionally, we extend the project with unit tests and a strong development methodology based on extreme programming principles.

---

## 2. Features

- Interactive command prompt
- Execution of built-in and external commands
- Handling of environment variables
- Input/output redirection
- Pipe management
- Signal handling
- Command history
- Unit tests for all functionalities

---

## 3. Getting Started

### Prerequisites

- **Operating System:** Unix-like OS (Linux, macOS)
- **Compiler:** GCC
- **Make:** Build automation tool
- **Git:** Version control system

### Installation

1. **Clone the Repository**

   ```bash
   git clone https://github.com/jeportie/Minishell.git
   cd Minishell
   ```

2. **Build the Project**

   ```bash
   make
   ```

3. **Run Minishell**

   ```bash
   ./minishell
   ```

---

## 4. Usage

Once the Minishell is running, you can use it just like a standard shell:

- Execute commands:

  ```bash
  minishell> ls -la
  ```

- Use pipes:

  ```bash
  minishell> ls -la | grep minishell
  ```

- Redirect input/output:

  ```bash
  minishell> echo "Hello World" > output.txt
  ```

- Manage environment variables:

  ```bash
  minishell> export VAR=value
  minishell> echo $VAR
  ```

- Access command history using the up/down arrow keys.

---

## 5. Project Structure

```plaintext
Minishell/
├── include/
│   └── project.h
├── lib/
│   ├── libft/
│   └── libgc/
├── src/
│   ├── builtins/
│   │   ├── cd.c
│   │   └── echo.c
│   └── main.c
├── test_src/
│   ├── builtins/
│   │   ├── test_cd.c
│   │   └── test_echo.c
│   └── test_main.c
├── docs/
│   └── ...
├── .github/
│   ├── ISSUE_TEMPLATE/
│   │   └── bug_report.md
│   ├── PULL_REQUEST_TEMPLATE.md
│   └── workflows/
│       └── ci.yml
├── Makefile
├── README.md
├── LICENSE
├── CHANGELOG.md
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
└── SECURITY.md
```

---

## 6. Development Strategy

We follow an extreme programming methodology with pair programming, test-driven development, and continuous integration.

### Branching Strategy

- **main**: Latest stable version with CI pipeline running global tests.
- **develop**: Main development branch with CI pipeline running unit tests.
- **pair_* branches**: Feature branches for pair programming (e.g., `pair_init`).
- **individual branches**: Personal branches for individual tasks (e.g., `greg_init`).

### Daily Routine

- **Coding Session (3 hours)**: Individual work on assigned tasks.
- **Review Session (1 hour)**: Code and test review with pair programmer.

### Continuous Integration

- **GitHub Actions**: Automated testing on push and pull requests.
- **Code Coverage**: Integrated with Codecov for code coverage analysis.

---

## 7. Contributing

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for more details.

---

## 8. License

This project is licensed under the [MIT License](LICENSE).

---

## 9. Acknowledgments

- **42 School** for providing the project guidelines.
- **Contributors**: [Gregory-Marquiset](https://github.com/Gregory-Marquiset), [jeportie](https://github.com/jeportie)

---

## Connect with Us 🤝

- [Greg's GitHub Profile](https://github.com/Gregory-Marquiset)
- [Jerome's GitHub Profile](https://github.com/jeportie)

### Languages and Tools:

<p align="left">
  <img src="https://github.com/devicons/devicon/blob/master/icons/c/c-original.svg" height="40" alt="C" />
  <img width="12" />
  <img src="https://github.com/devicons/devicon/blob/master/icons/bash/bash-original.svg" height="40" alt="Bash" />
  <img width="12" />
  <img src="https://github.com/devicons/devicon/blob/master/icons/git/git-original.svg" height="40" alt="Git" />
  <img width="12" />
</p>

<div>
  <img height="150" src="https://github-readme-stats.vercel.app/api?username=Gregory-Marquiset&show_icons=true&theme=default" alt="Greg's GitHub stats" />
  <img height="150" src="https://github-readme-stats.vercel.app/api?username=jeportie&show_icons=true&theme=default" alt="Jerome's GitHub stats" />
</div>

