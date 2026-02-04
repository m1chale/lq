# LogSeq CLI Tool

A simple command-line tool that brings [Logseq](https://logseq.com/) to your terminal.

## 🧑‍💻 Why this tool exists

As a software developer, I often find myself jumping between terminals, code, and notes.  
I use Logseq to keep track of ideas, shortcuts, and project notes — but reaching for the mouse or switching out of the terminal always broke my flow.

So I built **`lq`**, a simple CLI companion tool.  
It lets me search, read, and open my notes directly from the terminal — no context switching, no mouse, just focus.

---

## ✨ Features overview

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

## Install `lq`

A helper script called `release.sh` is included in the project root.
It compiles the tool and moves the binary into `~/bin/`.

> Make sure that `~/bin/` is included in your **PATH**, so that the `lq` command is available in your terminal.

If you prefer, you can always compile the binary manually and move it wherever you like.

## Install tab completion script

> Currently there is only a config for zsh provided. It should be easy enough to adapt it to other shells.

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

# ⚙️ Configuration

The tool uses a simple configuration file located at:

```
~/.lq
```

It should define your Logseq graph path and name like this:

```bash
LOGSEQ_PATH=/Users/example/docs/loqseq/mygraph/
GRAPH_NAME=notes
```
