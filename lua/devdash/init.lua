local M = {}

-- Show a floating dashboard window
function M.open_dashboard()
  local buf = vim.api.nvim_create_buf(false, true) -- create new empty buffer

  local width = math.floor(vim.o.columns * 0.8)
  local height = math.floor(vim.o.lines * 0.6)
  local row = math.floor((vim.o.lines - height) / 2)
  local col = math.floor((vim.o.columns - width) / 2)

  vim.api.nvim_open_win(buf, true, {
    relative = "editor",
    width = width,
    height = height,
    row = row,
    col = col,
    style = "minimal",
    border = "rounded",
  })

  local content = {
    "🧪 DevDash",
    "-------------",
    "Welcome to your terminal dashboard!",
    "",
    "More cool stuff coming soon..."
  }

  vim.api.nvim_buf_set_lines(buf, 0, -1, false, content)
end

-- Define :Devdash command
vim.api.nvim_create_user_command("Devdash", function()
  M.open_dashboard()
end, {})

return M

