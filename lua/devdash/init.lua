local M = {}

-- Store state
local dashboard_win = nil
local dashboard_buf = nil

-- Run a shell command and get output as a list of lines
local function run_command(cmd)
  local handle = io.popen(cmd)
  if not handle then return {"[command failed]"} end
  local result = handle:read("*a")
  handle:close()
  return vim.split(vim.trim(result), "\n")
end

-- Update dashboard with current data
local function update_dashboard()
  if not dashboard_buf or not vim.api.nvim_buf_is_valid(dashboard_buf) then
    return
  end

  local cpu_info = run_command("ps -A -o %cpu | awk '{s+=$1} END {print \"CPU Usage: \" s \"%\"}'")
  local mem_info = run_command("ps -A -o %mem | awk '{s+=$1} END {print \"Mem Usage: \" s \"%\"}'")

  local lines = {
    "🧪 DevDash — Your Floating Terminal Dashboard",
    "──────────────────────────────────────────────",
    "",
    unpack(cpu_info),
    unpack(mem_info),
    "",
    "(Auto-refreshing every 5s)  |  Press 'q' to close"
  }

  vim.api.nvim_buf_set_lines(dashboard_buf, 0, -1, false, lines)
end

-- Open the dashboard window
function M.open_dashboard()
  if dashboard_win and vim.api.nvim_win_is_valid(dashboard_win) then
    vim.api.nvim_set_current_win(dashboard_win)
    return
  end

  dashboard_buf = vim.api.nvim_create_buf(false, true)

  local width = math.floor(vim.o.columns * 0.8)
  local height = math.floor(vim.o.lines * 0.6)
  local row = math.floor((vim.o.lines - height) / 2)
  local col = math.floor((vim.o.columns - width) / 2)

  dashboard_win = vim.api.nvim_open_win(dashboard_buf, true, {
    relative = "editor",
    width = width,
    height = height,
    row = row,
    col = col,
    style = "minimal",
    border = "rounded",
  })

  vim.api.nvim_buf_set_option(dashboard_buf, "bufhidden", "wipe")
  vim.api.nvim_buf_set_keymap(dashboard_buf, "n", "q", ":bd!<CR>", { noremap = true, silent = true })

  update_dashboard() -- initial render

  -- Auto-refresh every 5s
  vim.fn.timer_start(5000, function()
    if dashboard_buf and vim.api.nvim_buf_is_valid(dashboard_buf) then
      update_dashboard()
    end
  end, { ['repeat'] = -1 })
end

vim.api.nvim_create_user_command("Devdash", function()
  M.open_dashboard()
end, {})

return M
