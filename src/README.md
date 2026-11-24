*This project has been created as part of the 42 curriculum by samercad.*

# get_next_line

## Description

`get_next_line` is a C function that returns one line at a time from a file
descriptor:

```c
char	*get_next_line(int fd);
```

Each call returns the next line, including its terminating `\n` (the last line
of a file has no `\n` if the file does not end with one). It returns `NULL`
when there is nothing left to read or when an error occurs. It works on
regular files, standard input, pipes and anything else `read()` accepts.

The point of the project is to learn about **static variables**: the function
has to remember what it has already read between two calls, without using a
global variable.

| File | Contents |
|---|---|
| `get_next_line.c` | `get_next_line` and its two static helpers |
| `get_next_line_utils.c` | small string helpers (`ft_strlen`, `ft_strchr`, `ft_strjoin_free`, `ft_substr`) |
| `get_next_line.h` | prototypes and the default `BUFFER_SIZE` |

## Instructions

Compile the files together with your own `main`, choosing the read size with
`-D BUFFER_SIZE=n` (it defaults to 42 when the flag is left out):

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

Example `main.c`:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

Every returned line is allocated with `malloc` and must be freed by the caller.

## Algorithm

### How it works

The function keeps a single static string, the **stash**: the bytes that have
already been read from the file descriptor but not yet returned.

1. **Do we already have a line?** If the stash contains a `\n`, no `read()`
   happens at all; go to step 3.
2. **Fill the stash** (`fill_stash`): read `BUFFER_SIZE` bytes at a time and
   append each chunk to the stash, stopping at the first chunk that contains a
   `\n`, or at end of file. Only the new chunk is searched for `\n`, never the
   whole stash again.
3. **Cut the line** (`take_line`): return everything up to and including the
   first `\n`, and keep the rest as the new stash. If nothing is left, the
   stash becomes `NULL`.
4. **End of file:** if the stash has no `\n` (last line without one), the
   stash itself is returned as the line and reset to `NULL`, with no extra
   copy. An empty stash at end of file means `NULL` is returned.

### Why this approach

- **It reads as little as possible.** Reading stops as soon as one line is
  complete, and a line already in the stash is returned without reading. This
  matters for standard input and pipes: reading further would block and wait
  for input the caller did not ask for yet.
- **It never reads the whole file at once.** Memory use depends on the length
  of the current line and on `BUFFER_SIZE`, not on the size of the file.
- **Any `BUFFER_SIZE` works.** The read buffer is allocated on the heap, so
  very large values (for example 10000000) do not overflow the stack, and a
  value of 1 still works, just with more `read()` calls.
- **Errors never leak or double-free.** `ft_strjoin_free` always frees the
  old stash, even when its own `malloc` fails, and on any `read()` or `malloc`
  error the stash is freed and set to `NULL` before `NULL` is returned. After
  the last line nothing is left allocated.
- **Only one static variable.** That keeps the state in one place and is what
  the bonus asks for; turning it into an array indexed by `fd` would let the
  function follow several file descriptors at once.

The trade-off: a very long line read with a tiny `BUFFER_SIZE` is copied once
per chunk while it is being built. A growing buffer would avoid those copies,
but it would make the code longer and harder to follow for a small gain.

## Resources

- `man 2 read`, `man 3 malloc`, `man 3 free`
- [Static variables in C (cppreference)](https://en.cppreference.com/w/c/language/storage_duration)
- [File descriptors (Wikipedia)](https://en.wikipedia.org/wiki/File_descriptor)
- [Valgrind quick start](https://valgrind.org/docs/manual/quick-start.html), used to check for leaks

### AI usage

Claude (Anthropic) was used to review and find bugs;

it found two bugs (a `read()` on a line that was already buffered, and a double
free when `read()` fails partway through a line), based on claude's findings and
pointed-out bad practices the code was redone.

prettified this README from a scratch to a presentable version.