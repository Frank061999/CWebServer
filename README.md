<h1>Lightweight, Multithreaded C Web Server</h1>
<p>This project is a custom-built web server developed entirely from scratch in C, showcasing a deep understanding of network programming and systems design. It includes:</p>
<ul>
  <li><strong>Custom Data Structures</strong>: Developed unique data structures, including a custom HTTP parser and hash maps, tailored for efficient request handling and resource management.</li>
  <li><strong>Multithreading</strong>: Implemented a thread pool architecture that distributes client requests across worker threads, ensuring high concurrency and performance.</li>
  <li><strong>Socket Programming</strong>: Handles socket creation, binding, and connection management with configurable timeouts for inactive connections.</li>
  <li><strong>Cross-Platform Support</strong>: Optimized for both Linux and macOS, utilizing platform-specific features like <code>sendfile()</code> for faster data transfers.</li>
  <li><strong>Scalable Design</strong>: Adjustable thread pool size and configurable parameters allow the server to scale based on application needs.</li>
</ul>
<p>This project highlights a solid foundation in low-level programming, offering full control over memory management and performance, with everything built from the ground up.</p>
