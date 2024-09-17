<h1>Lightweight, Multithreaded C Web Server</h1>
<p>This project showcases a practical implementation of a lightweight web server built in C, designed to handle multiple client connections efficiently. Key features include:</p>
<ul>
  <li><strong>Multithreading</strong>: A thread pool architecture distributes incoming requests to worker threads, optimizing performance under various loads.</li>
  <li><strong>HTTP Parsing</strong>: Includes a custom HTTP request parser that efficiently handles headers and pipelined requests.</li>
  <li><strong>Socket Programming</strong>: Manages connection lifecycles, binding, listening, and accepting client connections, with configurable timeouts for inactive clients.</li>
  <li><strong>Cross-Platform Support</strong>: Compatible with Linux and macOS, incorporating platform-specific optimizations like <code>sendfile()</code> for efficient data transfers.</li>
  <li><strong>Scalability</strong>: The server is flexible, allowing thread pool size and other parameters to be adjusted for various deployment needs.</li>
</ul>
<p>This project reflects an exploration of low-level network programming and multithreading, offering a foundation for building server applications.</p>
