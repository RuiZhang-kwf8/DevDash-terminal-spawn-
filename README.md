# devdash.nvim 

A terminal-style developer dashboard for Neovim — lightweight, async, and built entirely in Lua.

> Show system info, Git status, service health, and logs — all in a floating window or split, right from your Neovim session.

---

##  Features

- CPU and memory usage (via `top`/`ps`)
- Git status (async `git diff`)
- Last failed test summary
- Running Docker/PostgreSQL services
- Live logs from `tail -f`

All rendered in a fast, async, minimalist floating window UI.

---

## Installation

**Using [packer.nvim](https://github.com/wbthomason/packer.nvim)**:

```lua
use("your-username/devdash.nvim")
