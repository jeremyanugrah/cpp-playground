# 💻 Coding Playground (Chromebook Dev)

A centralized repository for my Computer Science experiments, algorithms, and daily coding tasks.  
This environment runs inside a **Debian 12 (Bookworm)** container on a **Lenovo S340 Chromebook**.

## 📂 Project Structure

This repository is organized by language to keep toolchains and dependencies isolated.

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

**🛠️ Environment & Tools**  
- Hardware: Lenovo S340 (Intel Celeron N4000, 4GB RAM)  
- OS: ChromeOS + Linux Container (Crostini)  
- Editor: Neovim (v0.7.2)  
- Languages: C++ (GCC/G++), Python 3.11  

## ⌨️ Neovim Cheat Sheet

My configuration uses **Space** as the Leader Key to trigger custom build commands.

| Shortcut     | Action       | Command Executed                  |
|--------------|--------------|-----------------------------------|
| Space + r    | Run C++      | `g++ main.cpp -o app && ./app`    |
| Space + p    | Run Python   | `python3 script.py`               |
| Tab          | Next File    | Switch to next open buffer        |
| Shift + Tab  | Prev File    | Switch to previous open buffer    |
| Ctrl + o     | Back Jump    | Go back to previous cursor location |
| Ctrl + i     | Fwd Jump     | Go forward to newest cursor location |
| `:ls`        | List Files   | Show all open buffers             |

### Configuration (`init.lua`)

Located at `~/.config/nvim/init.lua`:

```lua
-- Essential UI Settings
vim.opt.number = true          -- Line numbers
vim.opt.mouse = 'a'            -- Mouse support
vim.opt.termguicolors = true   -- Better colors
vim.cmd('colorscheme desert')  -- Theme

-- Indentation (4 Spaces)
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true

-- Key Mappings
vim.g.mapleader = " "

-- C++ Build & Run
vim.api.nvim_set_keymap('n', '<leader>r', ':!g++ % -o %< && ./%<<CR>', { noremap = true, silent = false })

-- Python Run
vim.api.nvim_set_keymap('n', '<leader>p', ':!python3 %<CR>', { noremap = true, silent = false })

-- Buffer Navigation
vim.api.nvim_set_keymap('n', '<Tab>', ':bnext<CR>', { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<S-Tab>', ':bprevious<CR>', { noremap = true, silent = true })
```

## 🚀 Workflow Guide

### Python Development
- Always use the virtual environment to prevent system conflicts.
- Activate: `source python-playground/.venv/bin/activate`
- Install: `pip install package_name`
- Run: Open file in nvim and press **Space + p**
- Deactivate: Type `deactivate` when finished.

### C++ Development
- Binaries (executables) are automatically ignored by `.gitignore` to keep the repo clean.
- Navigate: `cd cpp-playground/project-name`
- Code: `nvim main.cpp`
- Run: Press **Space + r** (Compiles to `app` and runs immediately)

### 📦 Git Commands
- Status: `git status` (Check what changed)
- Stage: `git add .` (Prepare all files)
- Commit: `git commit -m "Message"` (Save snapshot)
- Push: `git push` (Upload to GitHub)

***

### 💡 Pro Tip: The `tree` Command

To automatically generate that project structure tree in the future (instead of typing it manually), install the `tree` utility:

```bash
sudo apt install tree -y
```

Then, whenever you want to see your project layout (while ignoring the messy `.git` and `.venv` folders), run:

```bash
tree -I '.git|.venv|__pycache__'
```

***

**To use:** Save as `README.md` in your `~/coding-playground` repo root. This format avoids YAML parsing issues while keeping all your cheat sheet content intact.
