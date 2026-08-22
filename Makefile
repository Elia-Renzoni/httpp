compile_proof_server:
	g++ -I src proof/httpp_server.cc src/server/http_server.cc src/tcp/tcp.cc src/parsing/parser.cc src/parsing/scanner.cc src/parsing/tokens.cpp src/logger/logger.cc src/stream/network_stream.cc -o server 

run_proof_server:
	./server
