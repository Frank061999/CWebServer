# Lightweight, Multithreaded C Web Server

This project is a custom-built web server developed entirely from scratch in C. It includes:

- **Custom Data Structures**: thread-safe circular queue with semaphores, hash map, statically allocated hash set for MIME types and HTTP request types.
- **Custom HTTP Parser**: Built a lightweight parser from scratch to handle HTTP requests.
- **Multithreading**: Implemented a thread pool architecture that distributes client requests across worker threads, ensuring high concurrency and performance.
- **Socket Programming**: Handles socket creation, binding, and connection management with configurable timeouts for inactive connections.
- **Cross-Platform Support**: Optimized for both Linux and macOS, utilizing platform-specific features like `sendfile()` for faster data transfers.
- **Scalable Design**: Adjustable thread pool size and configurable parameters allow the server to scale based on application needs.

This project is built from scratch, including custom data structures and efficient memory management, with a focus on performance.

# Usage

By default, the server looks for the website in a folder called "site" at the current working directory. If the folder doesn't exist, the server will create an empty folder.  
Otherwise, you can specify the path to the website directory as the first argument:

`USAGE: ./Server [DIR]`
