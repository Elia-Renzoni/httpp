# httpp
A lightweight educational HTTP framework written in C++

## Overview
httpp is a minimal HTTP framework developed in C++, heavily inspired by the architecture and APIs of Golang's `net/http` standard library.

The project was built purely for educational purposes to explore low-level networking concepts, multithreading, protocol parsing, and TCP socket management in C++. It is strictly not intended or ready for production use.

## Features
- Go-style Interface: Route handling with an expressive API similar to Golang (e.g., `handleFunc`, `Request`, and `Response` objects).
- Multithreaded Processing: Concurrent handling of underlying TCP connections using dedicated threads.
- Integrated Logging: Non-blocking, thread-safe logging system for real-time request tracking.

## Limitations
- Limited Header Parsing: The main limitation stems from the scanner and parser, which cannot yet process various categories of complex HTTP headers.
- HTTP/1.1 Only: The framework currently supports only the HTTP/1.1 protocol.
- Incomplete RFC Coverage: Does not implement the full set of specifications and security guarantees of the official HTTP standard.
