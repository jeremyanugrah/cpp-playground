---
[Setup]
---

### 📄 **Chromebook Dev Setup - Cheat Sheet**


### 📄 **Chromebook Dev Setup - Cheat Sheet**

**Hardware:** Lenovo S340 (Intel Celeron N4000, 4GB RAM)
**OS:** ChromeOS + Debian 12 (Bookworm) Container
**Editor:** Neovim (v0.7.2)

---

### ⌨️ **Neovim Keybindings**

I configured `Space` as my **Leader Key**.

| Shortcut | Action | Command Runs |
| --- | --- | --- |
| **Space + r** | **Run C++** | `g++ main.cpp -o main && ./main` |
| **Space + p** | **Run Python** | `python3 script.py` |
| **Tab** | **Next File** | Switch to next open buffer |
| **Shift + Tab** | **Prev File** | Switch to previous open buffer |
| **:ls** | **List Files** | Show all open buffers |
| **Ctrl + o** | **Back Jump** | Go back to previous cursor location |
| **Ctrl + i** | **Fwd Jump** | Go forward to newest cursor location |

---

### ⚙️ **My Neovim Config (`init.lua`)**

*Location:* `~/.config/nvim/init.lua`

```lua
-- 1. Interface Settings
vim.opt.number = true          -- Show line numbers
vim.opt.mouse = 'a'            -- Enable mouse support
vim.opt.hlsearch = true        -- Highlight search results
vim.opt.incsearch = true       -- Jump to search result as you type
vim.opt.termguicolors = true   -- Better colors
vim.cmd('colorscheme desert')  -- Theme
vim.cmd('syntax on')           -- Syntax highlighting

-- 2. Indentation (4 spaces)
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true

-- 3. Key Mappings
vim.g.mapleader = " "          -- Set Space as Leader

-- Compile & Run C++ (Space + r)
vim.api.nvim_set_keymap('n', '<leader>r', ':!g++ % -o %< && ./%<<CR>', { noremap = true, silent = false })

-- Run Python (Space + p)
vim.api.nvim_set_keymap('n', '<leader>p', ':!python3 %<CR>', { noremap = true, silent = false })

-- Buffer Navigation (Tabs)
vim.api.nvim_set_keymap('n', '<Tab>', ':bnext<CR>', { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<S-Tab>', ':bprevious<CR>', { noremap = true, silent = true })

```

---

### 📦 **Git Workflow**

**1. Start a new feature:**

```bash
git pull origin main      # Get latest changes

```

**2. Save work:**

```bash
git add .                 # Stage all files
git commit -m "Update"    # Save with message

```

**3. Upload to GitHub:**

```bash
git push                  # Send to cloud

```

---

### 🛠 **Installed Tools**

* **Compilers:** `build-essential` (GCC/G++), `python3`, `python3-pip`, `python3-venv`
* **Editor:** `neovim`, `cmake`
* **Network:** `nmap`, `netcat-openbsd`, `tcpdump`, `tshark` (Wireshark CLI)

---

### How to push this file to your repo:

Once you have saved the file (`:wq`), run these three commands to update your GitHub repo with this documentation:

```bash
git add README.md
git commit -m "Add documentation and cheat sheet"
git push
```
