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

# 8. Install Foot terminal (lightweight, native Wayland, Nerd Font support)
sudo apt install foot -y

# 9. Install a Nerd Font for icons (JetBrains Mono)
mkdir -p ~/.local/share/fonts
cd ~/.local/share/fonts
curl -fLO https://github.com/ryanoasis/nerd-fonts/releases/latest/download/JetBrainsMono.tar.xz
tar -xf JetBrainsMono.tar.xz
rm JetBrainsMono.tar.xz
fc-cache -fv
cd -
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
vim.g.mapleader = " "
vim.g.maplocalleader = " "

vim.opt.number = true
vim.opt.relativenumber = true
vim.opt.mouse = 'a'
vim.opt.termguicolors = true
vim.opt.signcolumn = 'yes'
vim.opt.cursorline = true

vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true
vim.opt.autoindent = true
vim.opt.smartindent = true

vim.opt.clipboard:append('unnamedplus')
vim.opt.wrap = false
vim.opt.laststatus = 2
vim.opt.scrolloff = 8
vim.opt.updatetime = 250

vim.opt.shortmess:append("IsI")
vim.opt.ignorecase = true
vim.opt.smartcase = true
vim.opt.hlsearch = true
vim.opt.incsearch = true

vim.opt.splitright = true
vim.opt.splitbelow = true
vim.opt.undofile = true

--------------------------------------------------------------------------------
-- 2. BOOTSTRAP LAZY.NVIM
--------------------------------------------------------------------------------
local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not (vim.uv or vim.loop).fs_stat(lazypath) then
    vim.fn.system({ "git", "clone", "--filter=blob:none", "https://github.com/folke/lazy.nvim.git", "--branch=stable", lazypath })
end
vim.opt.rtp:prepend(lazypath)

--------------------------------------------------------------------------------
-- 3. PLUGINS
--------------------------------------------------------------------------------
require("lazy").setup({
    -- THEME
    { "folke/tokyonight.nvim", lazy = false, priority = 1000, config = function() vim.cmd.colorscheme("tokyonight-night") end },
    
    -- STATUS BAR (ICONS ENABLED)
    { 
        "nvim-lualine/lualine.nvim", 
        dependencies = { "nvim-tree/nvim-web-devicons" }, 
        config = function() 
            require("lualine").setup({ 
                options = { 
                    theme = "tokyonight",
                    -- These are the "Powerline" symbols you wanted
                    component_separators = { left = '', right = ''},
                    section_separators = { left = '', right = ''},
                    disabled_filetypes = { 'dapui_scopes', 'dapui_breakpoints', 'dapui_stacks', 'dapui_watches' },
                } 
            }) 
        end 
    },

    -- FILE TREE (ICONS ENABLED)
    { 
        "nvim-neo-tree/neo-tree.nvim", 
        branch = "v3.x", 
        dependencies = { "nvim-lua/plenary.nvim", "nvim-tree/nvim-web-devicons", "MunifTanjim/nui.nvim" }, 
        config = function()
            -- Defaults automatically use the cool folder/file icons
            vim.keymap.set('n', '<leader>t', ':Neotree toggle<CR>', { silent = true })
        end 
    },

    {
        "nvim-treesitter/nvim-treesitter",
        build = ":TSUpdate",
        config = function()
            local status, configs = pcall(require, "nvim-treesitter.configs")
            if not status then configs = require("nvim-treesitter.config") end
            configs.setup({
                ensure_installed = { "c", "cpp", "python", "lua", "bash", "markdown" },
                highlight = { enable = true },
                indent = { enable = true },
            })
        end,
    },
    {
        "neovim/nvim-lspconfig",
        dependencies = { "hrsh7th/cmp-nvim-lsp" },
        config = function()
            local capabilities = require("cmp_nvim_lsp").default_capabilities()
            vim.lsp.enable('clangd')
            vim.lsp.enable('pyright')
            vim.lsp.config('clangd', { capabilities = capabilities, cmd = { "clangd", "--background-index", "--clang-tidy" } })
            vim.lsp.config('pyright', { capabilities = capabilities })
            vim.api.nvim_create_autocmd("LspAttach", {
                callback = function(ev)
                    local opts = { buffer = ev.buf }
                    vim.keymap.set('n', 'gd', vim.lsp.buf.definition, opts)
                    vim.keymap.set('n', 'K', vim.lsp.buf.hover, opts)
                    vim.keymap.set('n', '<leader>rn', vim.lsp.buf.rename, opts)
                    vim.keymap.set('n', '<leader>ca', vim.lsp.buf.code_action, opts)
                end,
            })
        end,
    },
    {
        "hrsh7th/nvim-cmp",
        dependencies = { "hrsh7th/cmp-nvim-lsp", "hrsh7th/cmp-buffer", "hrsh7th/cmp-path", "L3MON4D3/LuaSnip", "saadparwaiz1/cmp_luasnip", "rafamadriz/friendly-snippets" },
        config = function()
            local cmp = require("cmp")
            local luasnip = require("luasnip")
            require("luasnip.loaders.from_vscode").lazy_load()
            cmp.setup({
                snippet = { expand = function(args) luasnip.lsp_expand(args.body) end },
                mapping = cmp.mapping.preset.insert({
                    ['<C-Space>'] = cmp.mapping.complete(),
                    ['<CR>'] = cmp.mapping.confirm({ select = true }),
                    ['<Tab>'] = cmp.mapping(function(fallback)
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
                sources = cmp.config.sources({ { name = "nvim_lsp" }, { name = "luasnip" } }, { { name = "buffer" }, { name = "path" } }),
            })
        end,
    },
    { "nvim-telescope/telescope.nvim", branch = "0.1.x", dependencies = { "nvim-lua/plenary.nvim" }, config = function()
        local builtin = require("telescope.builtin")
        vim.keymap.set('n', '<leader>ff', builtin.find_files, {})
        vim.keymap.set('n', '<leader>fg', builtin.live_grep, {})
    end },
    { "stevearc/conform.nvim", config = function()
        require("conform").setup({
            formatters_by_ft = { cpp = { "clang-format" }, c = { "clang-format" }, python = { "black" } },
            format_on_save = { timeout_ms = 500, lsp_format = "fallback" },
        })
    end },
    
    { "windwp/nvim-autopairs", event = "InsertEnter", config = true },
    { "lewis6991/gitsigns.nvim", config = true },
    
    -- DEBUGGER (FIXED PATH)
    {
        "mfussenegger/nvim-dap",
        dependencies = { 
            "rcarriga/nvim-dap-ui", 
            "nvim-neotest/nvim-nio", 
            "mfussenegger/nvim-dap-python",
            "williamboman/mason.nvim",
        },
        config = function()
            local dap, dapui = require("dap"), require("dapui")
            require("mason").setup()
            dapui.setup({
                layouts = {
                    {
                        elements = {
                            { id = "scopes", size = 0.35 },
                            { id = "stacks", size = 0.35 },
                            { id = "breakpoints", size = 0.15 },
                            { id = "watches", size = 0.15 },
                        },
                        size = 30,
                        position = "left",
                    },
                    {
                        elements = { { id = "repl", size = 0.5 }, { id = "console", size = 0.5 } },
                        size = 8,
                        position = "bottom",
                    },
                },
            })

            -- Direct path to the debugger binary we verified
            local cmd = os.getenv("HOME") .. "/.local/share/nvim/mason/bin/codelldb"
            dap.adapters.codelldb = {
                type = 'server',
                port = "${port}",
                executable = { command = cmd, args = {"--port", "${port}"} }
            }

            require('dap-python').setup('/home/mundo/.local/pipx/venvs/debugpy/bin/python')

            dap.configurations.cpp = {
                {
                    name = "Launch",
                    type = "codelldb",
                    request = "launch",
                    program = function()
                        local exe = vim.fn.getcwd() .. '/' .. vim.fn.expand('%:t:r')
                        return vim.fn.executable(exe) == 1 and exe or vim.fn.input('Path: ', vim.fn.getcwd() .. '/', 'file')
                    end,
                    cwd = '${workspaceFolder}',
                    stopOnEntry = false,
                },
            }
            dap.configurations.c = dap.configurations.cpp

            dap.listeners.before.attach.dapui_config = function() dapui.open() end
            dap.listeners.before.launch.dapui_config = function() dapui.open() end
            dap.listeners.before.event_terminated.dapui_config = function() dapui.close() end
            dap.listeners.before.event_exited.dapui_config = function() dapui.close() end
            
            vim.keymap.set('n', '<leader>db', dap.toggle_breakpoint)
            vim.keymap.set('n', '<leader>dc', dap.continue)
            vim.keymap.set('n', '<leader>dx', dapui.toggle)
        end,
    },
}, { rocks = { enabled = false, hererocks = false } })

--------------------------------------------------------------------------------
-- 4. CUSTOM KEYBINDINGS
--------------------------------------------------------------------------------
vim.keymap.set('n', '<leader>r', ':w<CR>:!g++ -g % -o %< && ./%<<CR>', { desc = "C++ Run" })
vim.keymap.set('n', '<leader>p', ':w<CR>:!python3 %<CR>', { desc = "Python Run" })
vim.keymap.set('n', '<Esc>', ':nohlsearch<CR>', { silent = true })
vim.keymap.set('n', '<leader>dq', function() require("dap").terminate() require("dapui").close() end, { desc = "Debug Quit" })
```

**Apply**: Back up your old config, then save this to `~/.config/nvim/init.lua`.  
On first launch, lazy.nvim will auto-install all plugins. Run `:checkhealth` to verify.

### Keybindings

**Run Code**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| Space + r    | Run C++          | Saves, compiles with `-g` & runs   |
| Space + p    | Run Python       | Saves & runs with python3           |
| Esc          | Clear Search     | Remove search highlighting          |

**LSP (C++ intelligence)**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| gd           | Go to Definition | Jump to where function is defined   |
| K            | Hover Docs       | Show documentation popup            |
| Space + ca   | Code Action      | Quick fixes & refactors             |
| Space + rn   | Rename           | Rename symbol across files          |

**Telescope (fuzzy finder)**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| Space + ff   | Find Files       | Fuzzy search all files              |
| Space + fg   | Live Grep        | Search text across project          |

**Debug (DAP)**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| Space + db   | Breakpoint       | Toggle breakpoint on line           |
| Space + dc   | Continue         | Start or continue debugging         |
| Space + dx   | Debug UI         | Toggle debug panel                  |
| Space + dq   | Debug Quit       | Kills process and closes UI         |

**Other**

| Shortcut     | Action           | Detail                              |
|--------------|------------------|-------------------------------------|
| Space + t    | File Tree        | Toggle Neo-tree sidebar             |
| Tab / S-Tab  | Completion       | Cycle autocomplete forward/backward |
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
RUN CODE
  Space+r     → C++ compile & run with -g (auto-saves)
  Space+p     → Python run (auto-saves)
  Esc         → Clear search highlight

LSP (C++ INTELLIGENCE)
  gd          → Go to definition
  K           → Hover docs
  Space+ca    → Code action (quick fix)
  Space+rn    → Rename symbol

TELESCOPE (SEARCH)
  Space+ff    → Find files
  Space+fg    → Grep in project

DEBUG
  Space+db    → Toggle breakpoint
  Space+dc    → Start/continue
  Space+dx    → Toggle debug UI
  Space+dq    → Quit debug session

OTHER
  Space+t     → Toggle file tree
  Tab/S-Tab   → Cycle completions forward/backward
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

## �️ Foot Terminal Setup

Foot is a lightweight, native Wayland terminal — perfect for low-spec Chromebooks. It renders faster than the default Crostini terminal and supports Nerd Fonts out of the box.

### Foot Config (`~/.config/foot/foot.ini`)

```bash
mkdir -p ~/.config/foot
nvim ~/.config/foot/foot.ini
```

Paste this configuration:

```ini
# ~/.config/foot/foot.ini

[main]
font=JetBrainsMono Nerd Font:size=11
pad=10x10

[colors]
alpha=0.90
background=1a1b26
foreground=c0caf5

## Normal Colors (Tokyo Night)
regular0=15161e
regular1=f7768e
regular2=9ece6a
regular3=e0af68
regular4=7aa2f7
regular5=bb9af7
regular6=7dcfff
regular7=a9b1d6

## Bright Colors
bright0=414868
bright1=f7768e
bright2=9ece6a
bright3=e0af68
bright4=7aa2f7
bright5=bb9af7
bright6=7dcfff
bright7=c0caf5
```

Launch Foot from the ChromeOS Launcher (Search Key → type "foot"), then pin it to your shelf.

---

## �🚀 Workflow Guide

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

