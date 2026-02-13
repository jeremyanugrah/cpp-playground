# 💻 Coding Playground (Chromebook Dev)

A centralized repository for my Computer Science experiments, algorithms, and daily coding tasks.  
This environment runs inside a **Debian 12 (Bookworm)** container on a **Lenovo S340 Chromebook**.

**🛠️ Environment & Tools**  
- Hardware: Lenovo S340 (Intel Celeron N4000, 4GB RAM)  
- OS: ChromeOS + Linux Container (Crostini)  
- Editor: Neovim (v0.7.2)  
- Languages: C++ (GCC/G++), Python 3.11  

## 📂 Project Structure

```text
coding-playground/
├── .gitignore             # Global ignore rules (binaries, venv, cache)
├── README.md              # Documentation & Cheat Sheet
│
├── cpp-playground/        # C++ Workspace
│   └── hello-world/       # Project: Basic C++ Tests
│       ├── main.cpp       # Source code
│       └── (binaries)     # app/main (Ignored by Git)
│
└── python-playground/     # Python Workspace
    ├── .venv/             # Virtual Environment (Ignored by Git)
    ├── script.py          # Python scripts
    └── test.py            # API testing scripts
```

---

## ⌨️ Neovim Setup & Crash Course

My configuration uses **Space** as the Leader Key. Full config at `~/.config/nvim/init.lua`.

### Neovim Config (`init.lua`)

```lua
-- UI Basics
vim.opt.number = true          -- Line numbers
vim.opt.mouse = 'a'            -- Mouse support
vim.opt.termguicolors = true   -- Better colors
vim.cmd('colorscheme desert')  -- Dark theme

-- Indentation (4 spaces for C++/Python)
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true

-- Leader key (Spacebar)
vim.g.mapleader = " "

-- Run Code
vim.keymap.set('n', '<leader>r', ':!g++ % -o %< && ./%<<CR>', { silent = false })  -- Space+r = C++ compile+run
vim.keymap.set('n', '<leader>p', ':!python3 %<CR>', { silent = false })            -- Space+p = Python run

-- Buffers (multiple files)
vim.keymap.set('n', '<Tab>', ':bnext<CR>', { silent = true })      -- Tab = next file
vim.keymap.set('n', '<S-Tab>', ':bprevious<CR>', { silent = true }) -- Shift+Tab = prev file

-- Clipboard (Chromebook fix)
vim.opt.clipboard:append('unnamedplus')  -- y/p syncs with Chrome/gedit

-- Search
vim.opt.ignorecase = true
vim.opt.smartcase = true
vim.opt.hlsearch = true
vim.opt.incsearch = true

-- Editing
vim.opt.autoindent = true
vim.opt.wrap = false
vim.opt.laststatus = 2
```

**Apply**: Save to `~/.config/nvim/init.lua`, restart Neovim.

### Essential Clipboard Fix (Crostini/Chromebook)

```bash
sudo apt update
sudo apt install wl-clipboard    # Wayland clipboard (NOT xclip)
```

**Test**: `:checkhealth provider` → "clipboard: OK"

### Keybindings

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| Space + r    | Run C++          | `g++ main.cpp -o app && ./app`      |
| Space + p    | Run Python       | `python3 script.py`                 |
| Tab          | Next File        | Switch to next open buffer          |
| Shift + Tab  | Prev File        | Switch to previous open buffer      |
| Ctrl + o     | Back Jump        | Go back to previous cursor location |
| Ctrl + i     | Fwd Jump         | Go forward to newest cursor location|
| `:ls`        | List Files       | Show all open buffers               |

### Core Text Operations

| Action | How | Keys |
|--------|-----|------|
| **Copy** | Select → copy | Mouse/keyboard select → `y` |
| **Paste** | Paste clipboard | `p` (after cursor) or `P` (before) |
| **Delete** | Select → delete | Mouse/keyboard select → `d` |
| **Undo** | Revert change | `u` |
| **Redo** | Undo the undo | `Ctrl+r` |
| **Line reset** | Clear current line | `U` |

**Select methods**: Mouse click+drag, `v` (chars) or `V` (lines) + arrow keys

### Delete Multiple Lines

```
3dd          → Delete 3 lines from cursor
Mouse select → d
V + down → d → Line select + delete
```

### Save/Quit Commands

| Situation | Command |
|-----------|---------|
| Save current | `:w` |
| Save ALL files | `:wa` |
| Quit current | `:q` |
| Quit ALL | `:qa` |
| **Save+Quit current** | `:wq` or `ZZ` |
| **Save+Quit ALL** | `:wqa` |
| Emergency exit | `:qa!` (lose changes) |

### Quick Reference

```
Space+r     → C++ compile & run
Space+p     → Python run
Tab         → Next file/buffer
Shift+Tab   → Previous file
y           → Copy selected
d           → Delete selected
p           → Paste
u           → Undo
Ctrl+r      → Redo
:wqa        → Save all + quit
```

---

## 🚀 Workflow Guide

### C++ Development

```
1. cd cpp-playground/project-name && nvim main.cpp
2. Space+r → compiles & runs
3. Edit → mouse select → y (copy) → Ctrl+Shift+V in Chrome
4. Copy from Chrome → p in Neovim
5. Delete wrong lines → mouse select → d
6. Oops → u (undo)
7. Done → :wqa
```

### Python Development

- Activate venv: `source python-playground/.venv/bin/activate`
- Install packages: `pip install package_name`
- Run: Open file in nvim and press **Space + p**
- Deactivate: `deactivate`

### 📦 Git Commands

```bash
git status                    # Check what changed
git add .                     # Stage all files
git commit -m "Message"       # Save snapshot
git push                      # Upload to GitHub
```

---

### 💡 Pro Tip: The `tree` Command

```bash
sudo apt install tree -y
tree -I '.git|.venv|__pycache__'
```

