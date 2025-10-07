# LogSeq CLI Tool

A simple command-line tool that brings [Logseq](https://logseq.com/) to your terminal.

## 🧑‍💻 Why This Tool Exists

As a software developer, I often find myself jumping between terminals, code, and notes.  
I use Logseq to keep track of ideas, shortcuts, and project notes — but reaching for the mouse or switching out of the terminal always broke my flow.

So I built **`lq`**, a simple CLI companion tool.  
It lets me search, read, and open my notes directly from the terminal — no context switching, no mouse, just focus.

---

## ✨ Features Overview

| Command                   | Description                               |
| ------------------------- | ----------------------------------------- |
| `lq pages`                | Print all pages                           |
| `lq journals`             | Print all journals                        |
| `lq sites`                | Print all pages and journals              |
| `lq cat <site-name>`      | Print the content of a page or journal    |
| `lq grep <search-string>` | Search through your notes                 |
| `lq open <site-name>`     | Open a page or journal directly in Logseq |

---

## 💡 Examples

TODO

# Installation

## install `lq`

A helper script called `release.sh` is included in the project root.
It compiles the tool and moves the binary into `~/bin/`.

> Make sure that `~/bin/` is included in your **PATH**, so that the `lq` command is available in your terminal.

If you prefer, you can always compile the binary manually and move it wherever you like.

## install tab completion script

To enable shell completions, copy the `_lq` completion script for your shell from
`shell-completions/<your-shell>/` into your local completions folder:

```
mkdir -p ~/.zsh/completions
cp shell-completions/zsh/_lq ~/.zsh/completions/
```

Then add the following lines to your `~/.zshrc` (if not already present):

```
echo 'fpath=("$HOME/.zsh/completions" $fpath)' >> ~/.zshrc
echo 'autoload -Uz compinit && compinit' >> ~/.zshrc
source ~/.zshrc
```
