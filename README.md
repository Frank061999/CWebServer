<h1>Lightweight, Multithreaded C Web Server</h1>
<p>This project is a custom-built web server developed entirely from scratch in C. It includes:</p>
<ul>
  <li><strong>Custom Data Structures</strong>: thread-safe circular queue with semaphores, hash map, statically allocated hash set for MIME types and HTTP request types</li>
  <li><strong>Custom HTTP Parser</strong>: Built a lightweight parser from scratch to handle HTTP requests.</li>
  <li><strong>Multithreading</strong>: Implemented a thread pool architecture that distributes client requests across worker threads, ensuring high concurrency and performance.</li>
  <li><strong>Socket Programming</strong>: Handles socket creation, binding, and connection management with configurable timeouts for inactive connections.</li>
  <li><strong>Cross-Platform Support</strong>: Optimized for both Linux and macOS, utilizing platform-specific features like <code>sendfile()</code> for faster data transfers.</li>
  <li><strong>Scalable Design</strong>: Adjustable thread pool size and configurable parameters allow the server to scale based on application needs.</li>
</ul>
<p>This project is built from scratch, including custom data structures and efficient memory management, with a focus on performance.</p>
<h1>Usage</h1>
By default, the server looks for the website in a folder called "site" at the current working directory. If the folder doesn't exist, the server will create an empty folder.
Otherwise, you can specify the path to the website directory as the first argument <br/><br/>
<code> USAGE: CWebServer [DIR] </code>
