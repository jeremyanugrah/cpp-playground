# 💻 Coding Playground (Chromebook Dev)

A centralized repository for my Computer Science experiments, algorithms, and daily coding tasks.  
This environment runs inside a **Debian 12 (Bookworm)** container on a **Lenovo S340 Chromebook**.

**🛠️ Environment & Tools**  
- Hardware: Lenovo S340 (Intel Celeron N4000, 4GB RAM)  
- OS: ChromeOS + Linux Container (Crostini)  
- Editor: Neovim (v0.9+) with LSP, Treesitter, Telescope  
- Languages: C++ (GCC/G++ + clangd), Python 3.11  

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

### Prerequisites (run once)

```bash
# 1. Install FUSE (required for AppImage on Debian/Crostini)
sudo apt update && sudo apt install fuse libfuse2 -y

# 2. Download latest stable Neovim AppImage
curl -LO https://github.com/neovim/neovim/releases/latest/download/nvim-linux-x86_64.appimage
chmod u+x nvim-linux-x86_64.appimage
sudo mkdir -p /opt/nvim
sudo mv nvim-linux-x86_64.appimage /opt/nvim/nvim

# 3. Create symbolic link so `nvim` works from anywhere
sudo ln -s /opt/nvim/nvim /usr/local/bin/nvim

# 4. Verify (should show v0.10+)
nvim --version

# 5. Install C++ language server & tools
sudo apt install clangd clang-format -y

# 6. Install clipboard support (Chromebook/Crostini)
sudo apt install wl-clipboard -y

# 7. Install ripgrep (needed by Telescope live grep)
sudo apt install ripgrep -y
```

<details>
<summary>Command meanings</summary>

- **curl -LO**: `-L` follows redirects (required for GitHub), `-O` saves with original filename
- **chmod u+x**: grants the **u**ser (owner) e**x**ecute permissions
- **mkdir -p**: creates **p**arent directories if they don't exist; no error if existing
- **ln -s**: creates a **s**ymbolic (soft) link rather than a hard link
</details>

### Neovim Config (`init.lua`)

Replace `~/.config/nvim/init.lua` with this entire file:

```lua
--------------------------------------------------------------------------------
-- 1. CORE SETTINGS
--------------------------------------------------------------------------------
vim.g.mapleader = " "                   -- Spacebar as leader key
vim.g.maplocalleader = " "

vim.opt.number = true                   -- Line numbers
vim.opt.relativenumber = true           -- Relative line numbers (fast jumps)
vim.opt.mouse = 'a'                     -- Mouse support
vim.opt.termguicolors = true            -- True color support
vim.opt.signcolumn = 'yes'              -- Always show sign column (no layout shift)
vim.opt.cursorline = true               -- Highlight current line

vim.opt.tabstop = 4                     -- 4-space tabs
vim.opt.shiftwidth = 4
vim.opt.expandtab = true
vim.opt.autoindent = true
vim.opt.smartindent = true              -- Smart C-style indentation

vim.opt.clipboard:append('unnamedplus') -- System clipboard (y/p syncs with Chrome)
vim.opt.wrap = false                    -- No line wrapping
vim.opt.laststatus = 2                  -- Always show status line
vim.opt.scrolloff = 8                   -- Keep 8 lines visible above/below cursor
vim.opt.updatetime = 250                -- Faster CursorHold events (diagnostics)

vim.opt.ignorecase = true               -- Case-insensitive search...
vim.opt.smartcase = true                -- ...unless you use uppercase
vim.opt.hlsearch = true
vim.opt.incsearch = true

vim.opt.splitright = true               -- Open vertical splits to the right
vim.opt.splitbelow = true               -- Open horizontal splits below
vim.opt.undofile = true                 -- Persistent undo across sessions

--------------------------------------------------------------------------------
-- 2. BOOTSTRAP LAZY.NVIM (Plugin Manager)
--------------------------------------------------------------------------------
local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not vim.loop.fs_stat(lazypath) then
    vim.fn.system({
        "git", "clone", "--filter=blob:none",
        "https://github.com/folke/lazy.nvim.git",
        "--branch=stable", lazypath,
    })
end
vim.opt.rtp:prepend(lazypath)

--------------------------------------------------------------------------------
-- 3. PLUGINS
--------------------------------------------------------------------------------
require("lazy").setup({

    -- Colorscheme
    {
        "folke/tokyonight.nvim",
        lazy = false,
        priority = 1000,
        config = function()
            vim.cmd.colorscheme("tokyonight-night")
        end,
    },

    -- Status line
    {
        "nvim-lualine/lualine.nvim",
        dependencies = { "nvim-tree/nvim-web-devicons" },
        config = function()
            require("lualine").setup({ options = { theme = "tokyonight" } })
        end,
    },

    -- Treesitter (syntax highlighting & code understanding)
    {
        "nvim-treesitter/nvim-treesitter",
        build = ":TSUpdate",
        config = function()
            require("nvim-treesitter.configs").setup({
                ensure_installed = { "c", "cpp", "python", "lua", "bash", "markdown" },
                highlight = { enable = true },
                indent = { enable = true },
            })
        end,
    },

    -- LSP (Language Server Protocol)
    {
        "neovim/nvim-lspconfig",
        dependencies = {
            "hrsh7th/cmp-nvim-lsp",    -- LSP source for nvim-cmp
        },
        config = function()
            local capabilities = require("cmp_nvim_lsp").default_capabilities()
            local lspconfig = require("lspconfig")

            -- C++ (clangd)
            lspconfig.clangd.setup({
                capabilities = capabilities,
                cmd = { "clangd", "--background-index", "--clang-tidy", "--header-insertion=iwyu" },
            })

            -- Python (pyright) — install: npm i -g pyright
            lspconfig.pyright.setup({
                capabilities = capabilities,
            })

            -- LSP Keybindings (activate when LSP attaches to a buffer)
            vim.api.nvim_create_autocmd("LspAttach", {
                callback = function(ev)
                    local opts = { buffer = ev.buf }
                    vim.keymap.set('n', 'gd', vim.lsp.buf.definition, opts)          -- Go to definition
                    vim.keymap.set('n', 'gD', vim.lsp.buf.declaration, opts)         -- Go to declaration
                    vim.keymap.set('n', 'gr', vim.lsp.buf.references, opts)          -- Find all references
                    vim.keymap.set('n', 'K', vim.lsp.buf.hover, opts)                -- Show docs on hover
                    vim.keymap.set('n', '<leader>ca', vim.lsp.buf.code_action, opts) -- Code actions
                    vim.keymap.set('n', '<leader>rn', vim.lsp.buf.rename, opts)      -- Rename symbol
                    vim.keymap.set('n', '<leader>e', vim.diagnostic.open_float, opts)-- Show error details
                    vim.keymap.set('n', '[d', vim.diagnostic.goto_prev, opts)        -- Previous error
                    vim.keymap.set('n', ']d', vim.diagnostic.goto_next, opts)        -- Next error
                end,
            })
        end,
    },

    -- Autocompletion
    {
        "hrsh7th/nvim-cmp",
        dependencies = {
            "hrsh7th/cmp-nvim-lsp",     -- LSP completions
            "hrsh7th/cmp-buffer",        -- Buffer word completions
            "hrsh7th/cmp-path",          -- File path completions
            "L3MON4D3/LuaSnip",         -- Snippet engine
            "saadparwaiz1/cmp_luasnip",  -- Snippet completions
            "rafamadriz/friendly-snippets", -- Pre-built snippet collection
        },
        config = function()
            local cmp = require("cmp")
            local luasnip = require("luasnip")
            require("luasnip.loaders.from_vscode").lazy_load() -- Load friendly-snippets

            cmp.setup({
                snippet = {
                    expand = function(args) luasnip.lsp_expand(args.body) end,
                },
                mapping = cmp.mapping.preset.insert({
                    ['<C-b>'] = cmp.mapping.scroll_docs(-4),
                    ['<C-f>'] = cmp.mapping.scroll_docs(4),
                    ['<C-Space>'] = cmp.mapping.complete(),        -- Trigger completion
                    ['<CR>'] = cmp.mapping.confirm({ select = true }), -- Accept selected
                    ['<C-e>'] = cmp.mapping.abort(),               -- Close menu
                    ['<Tab>'] = cmp.mapping(function(fallback)     -- Tab to cycle
                        if cmp.visible() then cmp.select_next_item()
                        elseif luasnip.expand_or_jumpable() then luasnip.expand_or_jump()
                        else fallback() end
                    end, { 'i', 's' }),
                    ['<S-Tab>'] = cmp.mapping(function(fallback)
                        if cmp.visible() then cmp.select_prev_item()
                        elseif luasnip.jumpable(-1) then luasnip.jump(-1)
                        else fallback() end
                    end, { 'i', 's' }),
                }),
                sources = cmp.config.sources({
                    { name = "nvim_lsp" },   -- LSP suggestions first
                    { name = "luasnip" },    -- Then snippets
                }, {
                    { name = "buffer" },     -- Then buffer words
                    { name = "path" },       -- Then file paths
                }),
            })
        end,
    },

    -- Telescope (fuzzy finder)
    {
        "nvim-telescope/telescope.nvim",
        branch = "0.1.x",
        dependencies = { "nvim-lua/plenary.nvim" },
        config = function()
            local builtin = require("telescope.builtin")
            vim.keymap.set('n', '<leader>ff', builtin.find_files, { desc = "Find files" })
            vim.keymap.set('n', '<leader>fg', builtin.live_grep, { desc = "Grep in project" })
            vim.keymap.set('n', '<leader>fb', builtin.buffers, { desc = "Open buffers" })
            vim.keymap.set('n', '<leader>fh', builtin.help_tags, { desc = "Help tags" })
            vim.keymap.set('n', '<leader>fs', builtin.lsp_document_symbols, { desc = "Symbols in file" })
        end,
    },

    -- Auto-format on save
    {
        "stevearc/conform.nvim",
        config = function()
            require("conform").setup({
                formatters_by_ft = {
                    cpp = { "clang-format" },
                    c = { "clang-format" },
                    python = { "black" },
                },
                format_on_save = {
                    timeout_ms = 500,
                    lsp_format = "fallback",
                },
            })
        end,
    },

    -- File explorer (sidebar)
    {
        "nvim-neo-tree/neo-tree.nvim",
        branch = "v3.x",
        dependencies = {
            "nvim-lua/plenary.nvim",
            "nvim-tree/nvim-web-devicons",
            "MunifTanjim/nui.nvim",
        },
        config = function()
            vim.keymap.set('n', '<leader>t', ':Neotree toggle<CR>', { silent = true })
        end,
    },

    -- Autopairs (auto-close brackets)
    {
        "windwp/nvim-autopairs",
        event = "InsertEnter",
        config = true,
    },

    -- Git signs in gutter
    {
        "lewis6991/gitsigns.nvim",
        config = true,
    },

    -- Debugging (DAP)
    {
        "mfussenegger/nvim-dap",
        dependencies = {
            "rcarriga/nvim-dap-ui",
            "nvim-neotest/nvim-nio",
        },
        config = function()
            local dap = require("dap")
            local dapui = require("dapui")
            dapui.setup()

            -- C++ / C debugger using GDB
            dap.adapters.cppdbg = {
                id = "cppdbg",
                type = "executable",
                command = "gdb",
                args = { "-i", "dap" },  -- GDB 14+ supports DAP natively
            }
            dap.configurations.cpp = {
                {
                    name = "Launch",
                    type = "cppdbg",
                    request = "launch",
                    program = function()
                        return vim.fn.input("Path to executable: ", vim.fn.getcwd() .. "/", "file")
                    end,
                    cwd = "${workspaceFolder}",
                    stopAtEntry = false,
                },
            }
            dap.configurations.c = dap.configurations.cpp

            -- Auto open/close debug UI
            dap.listeners.before.attach.dapui_config = function() dapui.open() end
            dap.listeners.before.launch.dapui_config = function() dapui.open() end
            dap.listeners.before.event_terminated.dapui_config = function() dapui.close() end
            dap.listeners.before.event_exited.dapui_config = function() dapui.close() end

            -- Debug keybindings
            vim.keymap.set('n', '<leader>db', dap.toggle_breakpoint, { desc = "Toggle breakpoint" })
            vim.keymap.set('n', '<leader>dc', dap.continue, { desc = "Start/continue debug" })
            vim.keymap.set('n', '<leader>di', dap.step_into, { desc = "Step into" })
            vim.keymap.set('n', '<leader>do', dap.step_over, { desc = "Step over" })
            vim.keymap.set('n', '<leader>dO', dap.step_out, { desc = "Step out" })
            vim.keymap.set('n', '<leader>dr', dap.repl.open, { desc = "Open REPL" })
            vim.keymap.set('n', '<leader>dx', dapui.toggle, { desc = "Toggle debug UI" })
        end,
    },

})

--------------------------------------------------------------------------------
-- 4. CUSTOM KEYBINDINGS (preserved from original config)
--------------------------------------------------------------------------------

-- Run code
vim.keymap.set('n', '<leader>r', ':w<CR>:!g++ % -o %< && ./%<<CR>', { silent = false, desc = "C++ compile & run" })
vim.keymap.set('n', '<leader>p', ':w<CR>:!python3 %<CR>', { silent = false, desc = "Python run" })

-- Buffers
vim.keymap.set('n', '<leader>bn', ':bnext<CR>', { silent = true, desc = "Next buffer" })
vim.keymap.set('n', '<leader>bp', ':bprevious<CR>', { silent = true, desc = "Previous buffer" })
vim.keymap.set('n', '<leader>bd', ':bdelete<CR>', { silent = true, desc = "Close buffer" })

-- Clear search highlight
vim.keymap.set('n', '<Esc>', ':nohlsearch<CR>', { silent = true })
```

**Apply**: Back up your old config, then save this to `~/.config/nvim/init.lua`.  
On first launch, lazy.nvim will auto-install all plugins. Run `:checkhealth` to verify.

### Keybindings

**Original (preserved)**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| Space + r    | Run C++          | Saves, compiles & runs              |
| Space + p    | Run Python       | Saves & runs with python3           |
| Space + bn   | Next Buffer      | Switch to next open buffer          |
| Space + bp   | Prev Buffer      | Switch to previous open buffer      |
| Space + bd   | Close Buffer     | Close current buffer                |
| Ctrl + o     | Back Jump        | Previous cursor location            |
| Ctrl + i     | Fwd Jump         | Next cursor location                |
| Esc          | Clear Search     | Remove search highlighting          |

**LSP (C++ intelligence)**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| gd           | Go to Definition | Jump to where function is defined   |
| gD           | Go to Declaration| Jump to declaration                 |
| gr           | References       | Find all usages of symbol           |
| K            | Hover Docs       | Show documentation popup            |
| Space + ca   | Code Action      | Quick fixes & refactors             |
| Space + rn   | Rename           | Rename symbol across files          |
| Space + e    | Error Details    | Show full error in floating window  |
| [d / ]d      | Prev/Next Error  | Jump between diagnostics            |

**Telescope (fuzzy finder)**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| Space + ff   | Find Files       | Fuzzy search all files              |
| Space + fg   | Live Grep        | Search text across project          |
| Space + fb   | Buffers          | Switch open buffers                 |
| Space + fs   | Symbols          | Search functions/classes in file    |

**Debug (DAP)**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| Space + db   | Breakpoint       | Toggle breakpoint on line           |
| Space + dc   | Continue         | Start or continue debugging         |
| Space + di   | Step Into        | Step into function                  |
| Space + do   | Step Over        | Step over line                      |
| Space + dO   | Step Out         | Step out of function                |
| Space + dx   | Debug UI         | Toggle debug panel                  |

**Other**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| Space + t    | File Tree        | Toggle Neo-tree sidebar             |
| Tab / S-Tab  | Completion       | Cycle autocomplete suggestions      |
| Ctrl + Space | Trigger Complete | Open completion menu manually       |
| Enter        | Accept           | Confirm selected completion         |

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
ORIGINAL
  Space+r     → C++ compile & run (auto-saves)
  Space+p     → Python run (auto-saves)
  Space+bn/bp → Next/prev buffer
  Space+bd    → Close buffer
  Esc         → Clear search highlight

LSP (C++ INTELLIGENCE)
  gd          → Go to definition
  gr          → Find all references
  K           → Hover docs
  Space+ca    → Code action (quick fix)
  Space+rn    → Rename symbol
  Space+e     → Error details
  [d / ]d     → Prev/next error

TELESCOPE (SEARCH)
  Space+ff    → Find files
  Space+fg    → Grep in project
  Space+fb    → Switch buffers
  Space+fs    → Symbols in file

DEBUG
  Space+db    → Toggle breakpoint
  Space+dc    → Start/continue
  Space+di/do → Step into/over

OTHER
  Space+t     → Toggle file tree
  Tab/S-Tab   → Cycle completions
  Ctrl+Space  → Open completion menu
  Enter       → Accept completion

TEXT EDITING
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

