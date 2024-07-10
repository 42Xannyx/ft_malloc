# Sources

## Videos
- [But, what is Virtual Memory?](https://www.youtube.com/watch?v=A9WLYbE0p-I)
- [i wrote my own memory allocator in C to prove a point](https://www.youtube.com/watch?v=CulF4YQt6zA)

## Terms

### TLB

The Translation Lookaside Buffer is an essential component in modern CPUs, enhancing the performance of memory management by caching recent virtual-to-physical address translations. This helps in quickly resolving address translations and reduces the computational load on the memory management unit.

## Helpful Links

- [How to Write Your Own malloc() and free()](https://tharikasblogs.blogspot.com/p/how-to-write-your-own-malloc-and-free.html)
- [How to Create Your Own malloc() Library](https://medium.com/a-42-journey/how-to-create-your-own-malloc-library-b86fedd39b96)
- [mmap() man page](https://www.man7.org/linux/man-pages/man2/mmap.2.html)
- [Writing a Memory Allocator](http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/)

### Debugging Memory Allocations

To analyze the behavior of your custom memory allocator, particularly its use of `mmap()`, you can use the `strace` tool. This powerful utility allows you to trace system calls and signals, providing insights into how your program interacts with the kernel.

#### Basic Command:
```bash
strace -f -e mmap ./run.sh ./tests/alloc_tester
```

#### Command Breakdown:
- `strace`: The system call tracer utility.
- `-f`: Follows child processes as they are created. This is crucial if your program forks or uses multiple threads.
- `-e mmap`: Filters the output to show only `mmap()` calls. This helps focus on memory allocation behavior.

